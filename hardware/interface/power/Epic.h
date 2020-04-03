/*
 * Copyright (C) 2017 The Android Open Source Project
 * Copyright (C) 2018-2020 The LineageOS Project
 * Copyright (C) 2020      Andreas Schneider <asn@cryptomilk.org>
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef EPIC_H
#define EPIC_H

#include <android/hardware/power/1.0/IPower.h>

using ::android::hardware::power::V1_0::PowerHint;

struct Epic {
    bool Init(void);
    void videoEncode(PowerHint hint);

 private:
    void *(*_epic_alloc_request)(uint32_t hint);
    void (*_epic_free_request)(void);
    void (*_epic_acquire)(void *request);
    void (*_epic_release)(void);

    bool mEpicAvailable;
};

#endif /* EPIC_H */
