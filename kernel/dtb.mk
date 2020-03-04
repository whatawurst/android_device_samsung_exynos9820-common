# BOARD_CUSTOM_DTBIMG_MK := $(PLATFORM_PATH)/kernel/dtb.mk

MKDTBIMG   := $(HOST_OUT_EXECUTABLES)/mkdtimg$(HOST_EXECUTABLE_SUFFIX)
KERNEL_OUT := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ
DTB_DIR    := $(KERNEL_OUT)/arch/$(KERNEL_ARCH)/boot/dts/exynos
DTB_CFG    := $(PLATFORM_PATH)/kernel/$(TARGET_SOC).cfg

define build-dtbimage-target
	$(call pretty,"Target dtb image: $(BOARD_PREBUILT_DTBIMAGE)")
	$(MKDTBIMG) cfg_create $@ $(DTB_CFG) -d $(DTB_DIR)
	$(hide) chmod a+r $@
endef

$(BOARD_PREBUILT_DTBIMAGE): $(MKDTBIMG) $(INSTALLED_KERNEL_TARGET)
	$(build-dtbimage-target)
