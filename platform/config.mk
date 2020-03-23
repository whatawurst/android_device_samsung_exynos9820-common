### AUDIO
PRODUCT_COPY_FILES += \
    $(PLATFORM_PATH)/config/audio/audio_policy_configuration_sec.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio_policy_configuration.xml \
    $(PLATFORM_PATH)/config/audio/audio_policy_volumes_sec.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio_policy_volumes.xml

### GPS
PRODUCT_COPY_FILES += \
    $(PLATFORM_PATH)/config/gps/gps_psds.conf:$(TARGET_COPY_OUT)/etc/gps/gps_psds.conf

### KEYLAYOUT
PRODUCT_COPY_FILES += \
    $(PLATFORM_PATH)/config/keylayout/gpio_keys.kl:$(TARGET_COPY_OUT)/usr/keylayout/gpio_keys.kl
