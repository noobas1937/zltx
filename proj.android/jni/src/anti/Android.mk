LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := anti
LOCAL_MODULE_FILENAME := libanti
LOCAL_SRC_FILES := anti.c \

LOCAL_C_INCLUDES        := $(LOCAL_PATH)/..

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

include $(BUILD_STATIC_LIBRARY)
