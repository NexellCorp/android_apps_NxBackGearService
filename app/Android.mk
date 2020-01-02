LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

partition ?= system

LOCAL_PACKAGE_NAME := NxBackGearService
LOCAL_MODULE_TAGS := optional
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_MODULE_TAGS := optional
LOCAL_USE_AAPT2 := true

# LOCAL_STATIC_ANDROID_LIBRARIES := \
#     androidx.fragment_fragment \
#     androidx.legacy_legacy-support-core-ui \
#     androidx.core_core \
#     androidx.legacy_legacy-support-v13

LOCAL_STATIC_ANDROID_LIBRARIES := \
	android-support-v13


LOCAL_SRC_FILES += 	src/com/nexell/backgearservice/AutoRun.java \
					src/com/nexell/backgearservice/BackGearDetect.java \
					src/com/nexell/backgearservice/BackGearService.java \
					src/com/nexell/backgearservice/CtrlBackGearService.java \
					src/com/nexell/backgearservice/MainActivity.java \
					src/com/nexell/backgearservice/NxSvmConnectToServer.java \
					src/com/nexell/backgearservice/QuickRearCamCmd.java \

LOCAL_RESOURCE_DIR += \
	$(LOCAL_PATH)/res \

LOCAL_USE_AAPT2 := true



LOCAL_SDK_VERSION := current


#LOCAL_STATIC_LIBRARIES += libnxgpiocontrol
#LOCAL_SHARED_LIBRARIES += libnxsvm libnxavm_client 
LOCAL_JNI_SHARED_LIBRARIES += libnxbackgearservice_jni

include $(BUILD_PACKAGE)

