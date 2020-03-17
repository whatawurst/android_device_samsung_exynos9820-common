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

#define LOG_TAG "ExynosVibrator-HAL"

#include <android-base/logging.h>

#include <android-base/stringprintf.h>

#include <hardware/hardware.h>
#include <hardware/vibrator.h>

#include "Vibrator.h"

#include <cinttypes>
#include <cmath>
#include <fstream>
#include <iostream>

#define INTENSITY_MIN 1000
#define INTENSITY_MAX 10000
#define INENSITY_DEFAULT INTENSITY_MAX

#define CLICK_TIMING_MS 20

#define VIBRATOR_TIMEOUT_PATH "/sys/class/timed_output/vibrator/enable"
#define VIBRATOR_HAPTIC_PATH "/sys/class/timed_output/vibrator/haptic_engine"
#define VIBRATOR_INTENSITY_PATH "/sys/class/timed_output/vibrator/intensity"

namespace android {
namespace hardware {
namespace vibrator {
namespace V1_0 {
namespace implementation {

/*
 * Write value to path and close file.
 */
template <typename T>
static Return<Status> writeNode(const std::string& path, const T& value)
{
    std::ofstream node(path);
    if (!node) {
        LOG(ERROR) << "Failed to open: " << path;
        return Status::UNKNOWN_ERROR;
    }

    LOG(DEBUG) << "writeNode node: " << path << " value: " << value;

    node << value << std::endl;
    if (!node) {
        LOG(ERROR) << "Failed to write: " << value;
        return Status::UNKNOWN_ERROR;
    }

    return Status::OK;
}

static bool doesNodeExist(const std::string& path)
{
    std::ifstream f(path.c_str());
    return f.good();
}

Vibrator::Vibrator()
{
    bool ok;

    ok = doesNodeExist(VIBRATOR_TIMEOUT_PATH);
    if (ok) {
        mIsTimedOutVibriator = true;
    }
}

Return<Status> Vibrator::on(uint32_t timeout_ms)
{
    if (!mIsTimedOutVibriator) {
        return Status::UNSUPPORTED_OPERATION;
    }

    return writeNode(VIBRATOR_TIMEOUT_PATH, timeout_ms);
}

Return<Status> Vibrator::off()
{
    if (!mIsTimedOutVibriator) {
        return Status::UNSUPPORTED_OPERATION;
    }

    return writeNode(VIBRATOR_TIMEOUT_PATH, 0);
}

Return<bool> Vibrator::supportsAmplitudeControl()
{
    return true;
}

Return<Status> Vibrator::setAmplitude(uint8_t amplitude)
{
    uint32_t intensity;

    if (amplitude == 0) {
        return Status::BAD_VALUE;
    }

    LOG(VERBOSE) << "setting amplitude: " << amplitude;

    intensity = std::lround((amplitude - 1) * 10000.0 / 254.0);
    if (intensity > INTENSITY_MAX) {
        intensity = INTENSITY_MAX;
    }
    LOG(VERBOSE) << "setting intensity: " << intensity;

    if (doesNodeExist(VIBRATOR_HAPTIC_PATH)) {
        std::string haptic = android::base::StringPrintf("4 %u %u %u %u",
                                                         CLICK_TIMING_MS,
                                                         intensity,
                                                         2000,
                                                         1);

        return writeNode(VIBRATOR_HAPTIC_PATH, haptic);
    }

    if (doesNodeExist(VIBRATOR_INTENSITY_PATH)) {
        return writeNode(VIBRATOR_INTENSITY_PATH, intensity);
    }

    return Status::OK;
}

Return<void> Vibrator::perform(Effect effect,
                               EffectStrength strength,
                               perform_cb _hidl_cb)
{
    Status status = Status::OK;
    uint32_t timeMS = CLICK_TIMING_MS;
    uint32_t amplitude = 128;

    switch (effect) {
    case Effect::CLICK:
        switch (strength) {
        case EffectStrength::LIGHT:
            amplitude = 26;
            break;
        case EffectStrength::MEDIUM:
            amplitude = 78;
            break;
        case EffectStrength::STRONG:
            amplitude = 128;
            break;
        }

        on(timeMS);
        setAmplitude(amplitude);
        break;
    default:
        status = Status::UNSUPPORTED_OPERATION;
        break;
    }

    _hidl_cb(status, timeMS);

    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace vibrator
}  // namespace hardware
}  // namespace android
