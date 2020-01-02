LOCAL_PATH		:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := libnxavm_client
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_SUFFIX := .so
LOCAL_C_INCLUDE := ./include
LOCAL_SRC_FILES := libnxavm_client.so
LOCAL_MODULE_TAGS := optional
include $(BUILD_PREBUILT)