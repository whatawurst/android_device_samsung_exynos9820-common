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

PLATFORM_PATH := device/samsung/exynos9820-common

# Soong namespaces
PRODUCT_SOONG_NAMESPACES += \
    $(PLATFORM_PATH)

DEVICE_PACKAGE_OVERLAYS += \
    $(PLATFORM_PATH)/overlay

### RECOVERY
ifeq ($(WITH_TWRP),true)
# Add Timezone database
PRODUCT_COPY_FILES += \
    system/timezone/output_data/iana/tzdata:recovery/root/system/usr/share/zoneinfo/tzdata

# Add manifest for hwservicemanager
PRODUCT_COPY_FILES += \
    $(PLATFORM_PATH)/recovery/root/system/etc/vintf/manifest.xml:recovery/root/system/etc/vintf/manifest.xml \
    $(PLATFORM_PATH)/recovery/root/vendor/etc/vintf/manifest.xml:recovery/root/vendor/etc/vintf/manifest.xml
endif # WITH_TWRP

include $(PLATFORM_PATH)/platform/*.mk
