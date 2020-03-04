# BOARD_CUSTOM_DTBOIMG_MK := $(PLATFORM_PATH)/kernel/dtbo.mk

MKDTIMG := $(HOST_OUT_EXECUTABLES)/mkdtimg$(HOST_EXECUTABLE_SUFFIX)

KERNEL_OUT := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ
DTBO_DIR   := $(KERNEL_OUT)/arch/$(KERNEL_ARCH)/boot/dts/samsung

DTBO_CFG := $(PLATFORM_PATH)/kernel/$(TARGET_DEVICE).cfg

define build-dtboimage-target
    $(call pretty,"Target dtbo image: $(BOARD_PREBUILT_DTBOIMAGE)")
    $(MKDTIMG) cfg_create $@ $(DTBO_CFG) -d $(DTBO_DIR)
    $(hide) chmod a+r $@
endef

$(BOARD_PREBUILT_DTBOIMAGE): $(MKDTIMG) $(INSTALLED_KERNEL_TARGET)
	$(build-dtboimage-target)
