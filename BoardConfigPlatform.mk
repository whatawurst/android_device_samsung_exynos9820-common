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

PLATFORM_PATH := device/samsung/android_device_samsung_exynos9820-common

### BOARD
TARGET_BOARD_PLATFORM := exynos5
TARGET_SLSI_VARIANT := bsp
TARGET_SOC := exynos9820
TARGET_BOOTLOADER_BOARD_NAME := universal9820
TARGET_BOARD_PLATFORM_GPU := mali-g76

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
#TARGET_KERNEL_CLANG_PATH := $(BUILD_TOP)/kernel/samsung/exynos9820/toolchain/clang/host/linux-x86/clang-4639204-cfp-jopp/
TARGET_KERNEL_CLANG_VERSION := r353983c
BOARD_KERNEL_IMAGE_NAME := Image.gz

BOARD_KERNEL_BASE        := 0x10000000
BOARD_KERNEL_PAGESIZE    := 2048

### SYSTEM
# build/make
BOARD_BUILD_SYSTEM_ROOT_IMAGE := true
# system/core and build/make
AB_OTA_UPDATER := false

### RECOVERY
TARGET_RECOVERY_PIXEL_FORMAT := "ABGR_8888"
