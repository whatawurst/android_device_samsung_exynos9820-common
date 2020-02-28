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

BUILD_TOP := $(shell pwd)

PLATFORM_PATH := device/samsung/exynos9820-common

### BOARD
TARGET_BOARD_PLATFORM := exynos5
TARGET_SLSI_VARIANT := bsp
TARGET_SOC := exynos9820
TARGET_BOOTLOADER_BOARD_NAME := universal9820
TARGET_BOARD_PLATFORM_GPU := mali-g76

# build/make/core/Makefile
TARGET_NO_BOOTLOADER := true

# Enable hardware/samsung
BOARD_VENDOR := samsung

### PROCESSOR
TARGET_ARCH := arm64
TARGET_ARCH_VARIANT := armv8-a
TARGET_CPU_ABI := arm64-v8a
TARGET_CPU_ABI2 :=
TARGET_CPU_VARIANT := cortex-a53
TARGET_CPU_SMP := true

TARGET_2ND_ARCH := arm
TARGET_2ND_ARCH_VARIANT := armv8-a
TARGET_2ND_CPU_ABI := armeabi-v7a
TARGET_2ND_CPU_ABI2 := armeabi
TARGET_2ND_CPU_VARIANT := cortex-a53

### KERNEL
TARGET_KERNEL_SOURCE = kernel/samsung/exynos9820/
TARGET_KERNEL_CLANG_COMPILE := true
# Uses r353983c clang prebuilts copy from Android 10
TARGET_KERNEL_CLANG_VERSION := 9.0.3

BOARD_KERNEL_BASE            := 0x10000000
# See `bbootimg -i boot.img`
BOARD_KERNEL_PAGESIZE        := 2048
BOARD_KERNEL_IMAGE_NAME      := Image.gz
BOARD_KERNEL_SEPARATED_DTBO  := true

# See `bbootimg -i boot.img`
BOARD_KERNEL_OFFSET          := 0x00008000
BOARD_KERNEL_RAMDISK_OFFSET  := 0x01000000
BOARD_KERNEL_TAGS_OFFSET     := 0x00000100

# See `bbootimg -i boot.img`
BOARD_BOOT_HEADER_VERSION    := 1
BOARD_BOOT_HEADER_NAME       := SRPRI28A004RU

BOARD_MKBOOTIMG_ARGS := --kernel_offset $(BOARD_KERNEL_OFFSET)
BOARD_MKBOOTIMG_ARGS += --ramdisk_offset $(BOARD_KERNEL_RAMDISK_OFFSET)
BOARD_MKBOOTIMG_ARGS += --tags_offset $(BOARD_KERNEL_TAGS_OFFSET)
BOARD_MKBOOTIMG_ARGS += --header_version $(BOARD_BOOT_HEADER_VERSION)
BOARD_MKBOOTIMG_ARGS += --board $(BOARD_BOOT_HEADER_NAME)

### ANDROID VERIFIED BOOT
BOARD_AVB_ENABLE := true
ifeq ($(BOARD_AVB_ENABLE), true)
   BOARD_AVB_RECOVERY_KEY_PATH := external/avb/test/data/testkey_rsa2048.pem
   BOARD_AVB_RECOVERY_ALGORITHM := SHA256_RSA2048
   BOARD_AVB_RECOVERY_ROLLBACK_INDEX := 1
   BOARD_AVB_RECOVERY_ROLLBACK_INDEX_LOCATION := 1
endif # BOARD_AVB_ENABLE

### BINDER
TARGET_USES_64_BIT_BINDER := true

### SYSTEM
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4
# build/make
BOARD_BUILD_SYSTEM_ROOT_IMAGE := true
# system/core and build/make
AB_OTA_UPDATER := false

### VENDOR
BOARD_VENDORIMAGE_FILE_SYSTEM_TYPE := ext4
TARGET_COPY_OUT_VENDOR := vendor

### CACHE
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4

### USERDATA
TARGET_USERIMAGES_USE_EXT4 := true

### KEYMASTER
TARGET_KEYMASTER_VARIANT := samsung

### RECOVERY
BOARD_HAS_DOWNLOAD_MODE := true
TARGET_RECOVERY_PIXEL_FORMAT := "ABGR_8888"
BOARD_INCLUDE_RECOVERY_DTBO := true
TARGET_RECOVERY_FSTAB := $(PLATFORM_PATH)/rootdir/recovery.fstab

include $(PLATFORM_PATH)/twrp.mk
