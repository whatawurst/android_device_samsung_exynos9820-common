/*
 * Copyright (C) 2019-2020 The LineageOS Project
 * Copyright (C) 2020      Andreas Schneider <asn@cryptomilk.org>
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

#ifndef ANDROID_HARDWARE_VIBRATOR_V1_0_VIBRATOR_H
#define ANDROID_HARDWARE_VIBRATOR_V1_0_VIBRATOR_H

#include <android/hardware/vibrator/1.0/IVibrator.h>
#include <hidl/Status.h>

#include <fstream>

namespace android {
namespace hardware {
namespace vibrator {
namespace V1_0 {
namespace implementation {

class Vibrator : public IVibrator {
  public:
    Vibrator();

    // Methods from ::android::hardware::vibrator::V1_0::IVibrator follow.
    Return<Status> on(uint32_t timeoutMs) override;
    Return<Status> off(void) override;
    Return<bool> supportsAmplitudeControl(void) override;
    Return<Status> setAmplitude(uint8_t amplitude) override;
    Return<void> perform(Effect effect,
                         EffectStrength strength,
                         perform_cb _hidl_cb) override;

  private:
    Status activate(uint32_t ms);

    static const std::string effectToName(Effect effect);
    static const std::string effectStrengthToName(EffectStrength effect);

    static uint8_t strengthToAmplitude(EffectStrength strength, Status* status);
    static uint32_t effectToMs(Effect effect, Status* status);

    bool mIsTimedOutVibriator;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace vibrator
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_VIBRATOR_V1_0_VIBRATOR_H
