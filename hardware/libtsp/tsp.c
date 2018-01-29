/*
 * Copyright (C) 2018 The LineageOS Project
 * Copyright (C) 2020 Andreas Schneider <asn@cryptomilk.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "libtsp"

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <log/log.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "include/tsp.h"

#define TSP_PATH "/sys/class/sec/tsp/"

#define TSP_CMD_EXEC_FILE "cmd"
#define TSP_CMD_LIST_FILE "cmd_list"
#define TSP_CMD_RES_FILE "cmd_result"
#define TSP_CMD_STATE_FILE "cmd_status"

#define TSP_CMD_LEN 64
/* cmd_result is of the format 'command:result' */
#define TSP_CMD_RES_LEN ((TSP_CMD_LEN)+8)

/* aot_enable <1|0> */
#define TSP_CMD_DT2W_CTL "aot_enable"

#define CMD_STATE_LEN 16
#define CMD_STATE_NA "NOT_APPLICABLE"
#define CMD_STATE_WAITING "WAITING"
#define CMD_STATE_RUNNING "RUNNING"
#define CMD_STATE_OK "OK"
#define CMD_STATE_FAIL "FAIL"

static bool double_tap_supported;

/**********************************************************
 *** HELPER FUNCTIONS
 **********************************************************/

static int sysfs_read(char *path, char *s, int num_bytes)
{
    char errno_str[64];
    int len;
    int ret = 0;
    int fd;

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        strerror_r(errno, errno_str, sizeof(errno_str));
        ALOGE("Error opening %s: %s", path, errno_str);

        return -1;
    }

    len = read(fd, s, num_bytes - 1);
    if (len < 0) {
        strerror_r(errno, errno_str, sizeof(errno_str));
        ALOGE("Error reading from %s: %s", path, errno_str);

        ret = -1;
    } else {
        // do not store newlines, but terminate the string instead
        if (s[len-1] == '\n') {
            s[len-1] = '\0';
        } else {
            s[len] = '\0';
        }
    }

    close(fd);

    return ret;
}

static void sysfs_write(const char *path, char *s)
{
    char errno_str[64];
    int len;
    int fd;

    fd = open(path, O_WRONLY);
    if (fd < 0) {
        strerror_r(errno, errno_str, sizeof(errno_str));
        ALOGE("Error opening %s: %s", path, errno_str);
        return;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        strerror_r(errno, errno_str, sizeof(errno_str));
        ALOGE("Error writing to %s: %s", path, errno_str);
    }

    close(fd);
}

/**********************************************************
 *** TSP FUNCTIONS
 **********************************************************/

static bool check_tsp_has_cmd(const char *cmd)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    bool found = false;
    FILE *fp = NULL;

    fp = fopen(TSP_PATH TSP_CMD_LIST_FILE, "r");
    if (fp == NULL) {
        ALOGE("Failed to open TSP command list: %s", strerror(errno));
        return false;
    }

    while ((nread = getline(&line, &len, fp)) != -1) {
        if (strncmp(line, cmd, strlen(cmd)) == 0) {
            ALOGI("tsp has cmd: %s", cmd);
            found = true;
            break;
        }
    }

    if (!found)
        ALOGI("tsp doesn't ahve cmd: %s", cmd);

    free(line);
    fclose(fp);
    return found;
}

static bool tsp_wait_for_cmd(const char *cmd)
{
    char buf[CMD_STATE_LEN];
    int count = 0;

    while (count++ < 10) {
        if (sysfs_read(TSP_PATH TSP_CMD_STATE_FILE, buf, CMD_STATE_LEN) < 0) {
            ALOGE("%s: sysfs read failed", cmd);
            return false;
        }

        ALOGI("state: %s - want %s", buf, CMD_STATE_OK);
        /* TSP enters "ok" state once command result is ready to be read */
        if (!strncmp(buf, CMD_STATE_OK, strlen(CMD_STATE_OK)))
            return true;
        /* some commands skip the OK state and go straight to waiting. */
        if (!strncmp(buf, CMD_STATE_WAITING, strlen(CMD_STATE_WAITING)))
            return true;

        usleep(100);
    }

    ALOGE("%s: TSP timeout expired!", cmd);
    /* FIXME: if the TSP ends up in a weird state,
     * we should read the result file to cause it to
     * exit the state */
    return false;
}

char *tsp_get_result(void)
{
    char inbuf[TSP_CMD_RES_LEN];
    char *res;
    char *retbuf;

    if (sysfs_read(TSP_PATH TSP_CMD_RES_FILE, inbuf, TSP_CMD_RES_LEN) < 0) {
        ALOGI("tsp get result failed");
        return NULL;
    }

    ALOGI("Got result: %s", inbuf);

    if ((res = strstr(inbuf, ":")) == NULL) {
        ALOGI("no strstr");
        return NULL;
    }

    retbuf = calloc(strlen(res)+1, sizeof(char));
    if (retbuf == NULL)
        return NULL;

    strcpy(retbuf, res+1);

    ALOGD("res: %s", retbuf);

    return retbuf;
}


bool tsp_init(void)
{
    double_tap_supported = check_tsp_has_cmd(TSP_CMD_DT2W_CTL);

    return true;
}

static bool tsp_set_doubletap(bool state)
{
    char buf[TSP_CMD_LEN] = {0};
    bool ok;

    if (!double_tap_supported) {
        return false;
    }

    /* set DT2W en/disable */
    snprintf(buf, TSP_CMD_LEN, "%s,%d", TSP_CMD_DT2W_CTL, !!state);

    ALOGI("%s", buf);

    sysfs_write(TSP_PATH TSP_CMD_EXEC_FILE, buf);
    ok = tsp_wait_for_cmd(TSP_CMD_DT2W_CTL);
    if (!ok) {
        return false;
    }

    ALOGV("dt2w %s", state ? "enabled" : "disabled");

    return true;
}

bool tsp_enable_doubletap(void)
{
    return tsp_set_doubletap(true);
}

bool tsp_disable_doubletap(void)
{
    return tsp_set_doubletap(false);
}
