/*
 * Copyright (C) 2017 The Android Open Source Project
 * Copyright (C) 2018 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ANDROID_HARDWARE_POWER_V1_0_POWER_H
#define ANDROID_HARDWARE_POWER_V1_0_POWER_H

#include <android/hardware/power/1.0/IPower.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <hardware/power.h>

#include "InteractionHandler.h"
#include "Epic.h"

#include <fstream>

namespace android {
namespace hardware {
namespace power {
namespace V1_0 {
namespace implementation {

using ::android::hardware::power::V1_0::Feature;
using ::android::hardware::power::V1_0::PowerHint;
using ::android::hardware::power::V1_0::IPower;
using ::android::hardware::Return;
using ::android::hardware::Void;

struct Power : public IPower {
    // Methods from ::android::hardware::power::V1_0::IPower follow.

    Power();

    Return<void> setInteractive(bool interactive) override;
    Return<void> powerHint(PowerHint hint, int32_t data) override;
    Return<void> setFeature(Feature feature, bool activate) override;
    Return<void> getPlatformLowPowerStats(getPlatformLowPowerStats_cb _hidl_cb) override;

  private:
    bool mDoubleTapEnabled;

    InteractionHandler mInteractionHandler;
    Epic mEpic;
    std::vector<std::string> mInteractiveNodes;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace power
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_POWER_V1_0_POWER_H
