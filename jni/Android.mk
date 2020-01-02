LOCAL_PATH := $(call my-dir)

#
# Prebuilt Static Library
#


# include $(CLEAR_VARS)
# LOCAL_MODULE    := libnxgpiocontrol
# LOCAL_SRC_FILES := ../../../../../../library/lib/libnxgpiocontrol.a
# include $(PREBUILT_STATIC_LIBRARY)

# include $(CLEAR_VARS)
# LOCAL_MODULE   := libnxsvm
# LOCAL_SRC_FILES := ../../../../../../library/prebuilt/libnxsvm.so
# include $(PREBUILT_SHARED_LIBRARY)

# include $(CLEAR_VARS)
# LOCAL_MODULE    := libnxavm_client
# LOCAL_SRC_FILES := ../../../../../../library/prebuilt/libnxavm_client.so
# include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_CFLAGS := -O3 -DNDEBUG
LOCAL_CFLAGS += -Wall -Werror

LOCAL_CFLAGS += \
        -Wno-gnu-static-float-init \
        -Wno-non-literal-null-conversion \
        -Wno-self-assign \
        -Wno-unused-parameter \
        -Wno-unused-variable \
        -Wno-unused-function \

LOCAL_SRC_FILES         :=  \
    nxbackgearservice_jni.cpp \
    nxcommand.cpp \

LOCAL_C_INCLUDES        :=  \
    $(JNI_H_INCLUDE)         \
    $(LOCAL_PATH)/include   \
    $(LOCAL_PATH)/../library/include \
    $(LOCAL_PATH)/../library/prebuilt/include \

# some warnings should be fixed
LOCAL_CFLAGS += \
        -Wno-error=sometimes-uninitialized \


LOCAL_LDLIBS            :=  \
    -llog                   \
    -landroid               \


LOCAL_LDFLAGS		+=	\
	-L$(LOCAL_PATH)/lib		\


LOCAL_STATIC_LIBRARIES += libnxgpiocontrol
LOCAL_SHARED_LIBRARIES += libnxsvm libnxavm_client



LOCAL_MODULE := libnxbackgearservice_jni

include $(BUILD_SHARED_LIBRARY)
