LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LIBGLES_MALI_LIBRARY := /vendor/lib/egl/libGLES_mali.so
LIBGLES_MALI64_LIBRARY := /vendor/lib64/egl/libGLES_mali.so

VULKAN_SYMLINK := $(TARGET_OUT_VENDOR)/lib/hw/vulkan.universal9820.so
$(VULKAN_SYMLINK): $(LOCAL_INSTALLED_MODULE)
	@echo "Creating lib/hw/vulkan.universal9820.so symlink: $@"
	@mkdir -p $(dir $@)
	$(hide) ln -sf $(LIBGLES_MALI_LIBRARY) $@

VULKAN64_SYMLINK := $(TARGET_OUT_VENDOR)/lib64/hw/vulkan.universal9820.so
$(VULKAN64_SYMLINK): $(LOCAL_INSTALLED_MODULE)
	@echo "Creating lib64/hw/vulkan.universal9820.so symlink: $@"
	@mkdir -p $(dir $@)
	$(hide) ln -sf $(LIBGLES_MALI64_LIBRARY) $@

ALL_DEFAULT_INSTALLED_MODULES += \
	$(VULKAN_SYMLINK) \
	$(VULKAN64_SYMLINK)
