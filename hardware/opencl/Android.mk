LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LIBGLES_MALI_LIBRARY := /vendor/lib64/egl/libGLES_mali.so

LIBOPENCL_SYMLINK := $(TARGET_OUT_VENDOR_SHARED_LIBRARIES)/libOpenCL.so
$(LIBOPENCL_SYMLINK): $(LOCAL_INSTALLED_MODULE)
	@echo "Creating egl/libGLES_mali.so symlink: $@"
	@mkdir -p $(dir $@)
	$(hide) ln -sf $(LIBGLES_MALI_LIBRARY) $@

ALL_DEFAULT_INSTALLED_MODULES += \
	$(LIBOPENCL_SYMLINK)
