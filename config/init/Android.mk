#
# Copyright (C) 2020 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := fstab.exynos9820
LOCAL_SRC_FILES := fstab.exynos9820
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_STEM := fstab.exynos9820
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_ETC)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := skip_mount.exynos9820
LOCAL_SRC_FILES := skip_mount.cfg
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_STEM := skip_mount.cfg
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/init/config
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := init.exynos9820
LOCAL_SRC_FILES := init.exynos9820.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_STEM := init.exynos9820.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_ETC)/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := init.exynos9820.root
LOCAL_SRC_FILES := init.exynos9820.root.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_STEM := init.exynos9820.root.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_ETC)/init/hw
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := init.exynos9820.usb
LOCAL_SRC_FILES := init.exynos9820.usb.rc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_STEM := init.exynos9820.usb.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_ETC)/init
include $(BUILD_PREBUILT)
