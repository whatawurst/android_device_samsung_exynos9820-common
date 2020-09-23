LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := init.recovery.exynos9820
ifeq ($(WITH_TWRP), true)
LOCAL_SRC_FILES := init.recovery.twrp.exynos9820.rc
else
LOCAL_SRC_FILES := init.recovery.exynos9820.rc
endif # WITH_TWRP
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_STEM := init.recovery.exynos9820.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_RECOVERY_ROOT_OUT)
include $(BUILD_PREBUILT)

ifeq ($(TW_EXCLUDE_DEFAULT_USB_INIT), true)
include $(CLEAR_VARS)
LOCAL_MODULE := init.recovery.usb.rc
LOCAL_SRC_FILES := init.recovery.usb.rc
LOCAL_MODULE_TAGS := optional eng
LOCAL_MODULE_CLASS := RECOVERY_EXECUTABLES
LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT)
include $(BUILD_PREBUILT)
endif
