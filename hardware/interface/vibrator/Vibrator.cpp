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

#define LOG_TAG "vibrator@1.0-exynos9820"

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

Status Vibrator::activate(uint32_t timeoutMs)
{
    if (!mIsTimedOutVibriator) {
        return Status::UNSUPPORTED_OPERATION;
    }

    return writeNode(VIBRATOR_TIMEOUT_PATH, timeoutMs);
}

Return<Status> Vibrator::on(uint32_t timeoutMs)
{
    return activate(timeoutMs);
}

Return<Status> Vibrator::off()
{
    return activate(0);
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

    LOG(DEBUG) << "setting amplitude: " << (uint32_t)amplitude;

    intensity = std::lround((amplitude - 1) * 10000.0 / 254.0);
    if (intensity > INTENSITY_MAX) {
        intensity = INTENSITY_MAX;
    }
    LOG(DEBUG) << "setting intensity: " << intensity;

    if (doesNodeExist(VIBRATOR_INTENSITY_PATH)) {
        return writeNode(VIBRATOR_INTENSITY_PATH, intensity);
    }

    if (doesNodeExist(VIBRATOR_HAPTIC_PATH)) {
        std::string haptic = android::base::StringPrintf("4 %u %u %u %u",
                                                         CLICK_TIMING_MS,
                                                         intensity,
                                                         2000,
                                                         1);

        return writeNode(VIBRATOR_HAPTIC_PATH, haptic);
    }

    return Status::OK;
}

uint8_t Vibrator::strengthToAmplitude(EffectStrength strength, Status* status)
{
    *status = Status::OK;

    switch (strength) {
    case EffectStrength::LIGHT:
        return 78;
    case EffectStrength::MEDIUM:
        return 128;
    case EffectStrength::STRONG:
        return 204;
    }

    *status = Status::UNSUPPORTED_OPERATION;
    return 0;
}

uint32_t Vibrator::effectToMs(Effect effect, Status* status) {
    *status = Status::OK;

    switch (effect) {
    case Effect::CLICK:
        return 20;
    case Effect::DOUBLE_CLICK:
        return 25;
    }

    *status = Status::UNSUPPORTED_OPERATION;
    return 0;
}

const std::string Vibrator::effectToName(Effect effect)
{
    return toString(effect);
}

const std::string Vibrator::effectStrengthToName(EffectStrength strength)
{
    return toString(strength);
}

Return<void> Vibrator::perform(Effect effect,
                               EffectStrength strength,
                               perform_cb _hidl_cb)
{
    Status status = Status::OK;
    uint8_t amplitude;
    uint32_t ms;

    LOG(DEBUG) << "perform effect: " << effectToName(effect) <<
                 ", strength: " << effectStrengthToName(strength);

    amplitude = strengthToAmplitude(strength, &status);
    if (status != Status::OK) {
        _hidl_cb(status, 0);
        return Void();
    }
    setAmplitude(amplitude);

    ms = effectToMs(effect, &status);
    if (status != Status::OK) {
        _hidl_cb(status, 0);
        return Void();
    }
    status = activate(ms);

    _hidl_cb(status, ms);

    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace vibrator
}  // namespace hardware
}  // namespace android
