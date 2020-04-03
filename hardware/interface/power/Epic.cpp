/*
 * Copyright (C) 2018-2020 The LineageOS Project
 * Copyright (C) 2020      Andreas Schneider <asn@cryptomilk.org>
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "power@1.0-exynos9820"
//#define LOG_NDEBUG 0

#include <android-base/logging.h>
#include <android-base/file.h>
#include <cutils/properties.h>

#include <dlfcn.h>

#include "Epic.h"

using ::android::hardware::power::V1_0::PowerHint;

bool Epic::Init(void)
{
    void *handle = NULL;

    handle = dlopen("libepic_helper.so", RTLD_NOW);
    if (handle == nullptr) {
        LOG(ERROR) << "Failed to load libepic_helper.so";
        return false;
    }

    _epic_alloc_request = reinterpret_cast<typeof(_epic_alloc_request)>(dlsym(handle, "epic_alloc_request"));
    _epic_free_request = reinterpret_cast<typeof(_epic_free_request)>(dlsym(handle, "epic_free_request"));
    _epic_acquire = reinterpret_cast<typeof(_epic_acquire)>(dlsym(handle, "epic_acquire"));
    _epic_release = reinterpret_cast<typeof(_epic_release)>(dlsym(handle, "epic_release"));

    if (_epic_alloc_request == nullptr ||
        _epic_free_request == nullptr ||
        _epic_acquire == nullptr ||
        _epic_release == nullptr) {
        LOG(ERROR) << "Failed to bind symbols from libepic_helper.so";
        return false;
    }

    LOG(INFO) << "Epic is ready";

    mEpicAvailable = true;
    return true;
}

void Epic::videoEncode(PowerHint hint)
{
    int preview;
    void *request = NULL;

    if (!mEpicAvailable) {
        return;
    }

    request = _epic_alloc_request((uint32_t)hint);

    preview = property_get_int32("persist.vendor.sys.camera.preview", 0);
    if (preview != 2) {
        _epic_acquire(request);
    }
}
