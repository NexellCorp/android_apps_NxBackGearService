//
// Created by jmbahn on 19. 8. 19.
//

#include <android/log.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <NX_CGpioControl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <nxcommand.h>
#include <NxSvm.h>

//
// Debug tools
//
#define NX_DBG_VBS			2	// ANDROID_LOG_VERBOSE
#define NX_DBG_DEBUG		3	// ANDROID_LOG_DEBUG
#define	NX_DBG_INFO			4	// ANDROID_LOG_INFO
#define	NX_DBG_WARN			5	// ANDROID_LOG_WARN
#define	NX_DBG_ERR			6	// ANDROID_LOG_ERROR
#define NX_DBG_DISABLE		9

#define NX_DTAG "nxgpiocontrol_jni"

#define COMMAND_FILE_NAME "/data/data/com.nexell.backgearservice/rearcam_cmd"

const char *rearcam_type = "nx_rearcam=";

enum {
	CAM_TYPE_QUICKREAR	= 0x0,
	CAM_TYPE_1CAMTOPVIEW	= 0x1,
	CAM_TYPE_4CAMSVM	= 0x2
};


int gNx3DAvmDebugLevel 	= NX_DBG_VBS;

#define DBG_PRINT			__android_log_print
#define NxTrace(...)		DBG_PRINT(ANDROID_LOG_VERBOSE, NX_DTAG, __VA_ARGS__);

#define NxDbgMsg(A, ...)	do {										\
								if( gNx3DAvmDebugLevel <= A ) {		\
									DBG_PRINT(A, NX_DTAG, __VA_ARGS__);	\
								}										\
							} while(0)

static NX_CGpioControl *g_pGpioControl = NULL;
static void* pCmdHandle = NULL;


/**********************************************************************************
 * gpio export
 **********************************************************************************/
JNIEXPORT jint JNICALL NX_JniGpioInit(JNIEnv *env, jclass obj, jint gpio)
{
    if (g_pGpioControl) {
        return g_pGpioControl->Init(gpio);
    }

    return -1;
}

/**********************************************************************************
 * gpio unexport
 **********************************************************************************/
JNIEXPORT jint JNICALL NX_JniGpioDeInit(JNIEnv *env, jclass obj)
{
    if (g_pGpioControl) {
        return g_pGpioControl->Deinit();
    }

    return -1;
}

/**********************************************************************************
 * set gpio direction
 **********************************************************************************/
JNIEXPORT jint JNICALL NX_JniGpioSetDirection(JNIEnv *env, jclass obj, jint direction)
{
    if (g_pGpioControl) {
        return g_pGpioControl->SetDirection(direction);
    }

    return -1;
}

/**********************************************************************************
 * set gpio value
 **********************************************************************************/
JNIEXPORT jint JNICALL NX_JniGpioSetValueA(JNIEnv *env, jclass obj, jint value)
{
    // if (g_pGpioControl) {
    //     return g_pGpioControl->SetValue(value);
    // }

    return -1;
}

/**********************************************************************************
 * get gpio value
 **********************************************************************************/
JNIEXPORT jint JNICALL NX_JniGpioGetValue(JNIEnv *env, jclass obj)
{
    if (g_pGpioControl) {
        return g_pGpioControl->GetValue();
    }

    return -1;
}

/**********************************************************************************
 * set gpio edge
 **********************************************************************************/
JNIEXPORT jint JNICALL NX_JniGpioSetEdge(JNIEnv *env, jclass obj, jint edge)
{
    if (g_pGpioControl) {
        g_pGpioControl->SetEdge(edge);
    }

    return -1;
}


/**********************************************************************************
 * create command file
 **********************************************************************************/
JNIEXPORT jint JNICALL NX_JniCreateCmdFile(JNIEnv *env, jclass obj, jstring cmd_file_name)
{
    const char* file_name = env->GetStringUTFChars(cmd_file_name, NULL);

    if(pCmdHandle == NULL)
    {
        pCmdHandle = NX_GetCommandHandle();
    }
    return NX_CreateCmdFile(pCmdHandle, file_name);
}

// /**********************************************************************************
//  * remove command file
//  **********************************************************************************/
// JNIEXPORT jint JNICALL NX_JniRemoveCmdFile(JNIEnv *env, jclass obj, jstring cmd_file_name)
// {
//     const char* file_name = env->GetStringUTFChars(cmd_file_name, NULL);

//     return NX_RemoveCmdFile(file_name);
// }

/**********************************************************************************
 * write command to command file
 **********************************************************************************/
JNIEXPORT jint JNICALL NX_JniWriteCmd(JNIEnv *env, jclass obj, jstring cmd_file_name, jboolean quick_cmd)
{
    const char* file_name = env->GetStringUTFChars(cmd_file_name, NULL);
    bool quick_cmd_enable = (bool)(quick_cmd != JNI_FALSE);

    if(pCmdHandle == NULL)
    {
        pCmdHandle = NX_GetCommandHandle();
    }

    if(pCmdHandle != NULL)
    {
       return NX_WriteCmd(pCmdHandle, file_name, quick_cmd_enable);
    }
    return -1;
}


/**********************************************************************************
 * start command service
 **********************************************************************************/
JNIEXPORT jint JNICALL NX_JniNxStartCmdService( JNIEnv *env, jclass obj , jstring cmd_file_name, jboolean quick_cmd)
{
    const char* file_name = env->GetStringUTFChars(cmd_file_name, NULL);
    bool quick_cmd_enable = (bool)(quick_cmd != JNI_FALSE);
    if(pCmdHandle == NULL)
    {
        pCmdHandle = NX_GetCommandHandle();
    }

    NX_StartCommandService(pCmdHandle, file_name, quick_cmd_enable);

    return 0;
}

/**********************************************************************************
 * check NxQuickRearCam Status
 **********************************************************************************/
JNIEXPORT jint JNICALL NX_JniCheckQuickRearCamStatus(JNIEnv *env, jclass obj)
{
    if(pCmdHandle != NULL)
    {
        return NX_QuickRearCamGetStatus(pCmdHandle);
    }
    return -1;
}


/**********************************************************************************
 * stop command service
 **********************************************************************************/
JNIEXPORT jint JNICALL NX_JniNXStopCommandService( JNIEnv *env, jclass obj )
{
    if(pCmdHandle != NULL)
    {
        NX_StopCommandService(pCmdHandle);
    }
    return 0;
}

/**********************************************************************************
 * connect to nx_3d_avm_daemon
 **********************************************************************************/
JNIEXPORT jint JNICALL NX_JniNxSvmConnectToServer( JNIEnv *env, jclass obj, jint client_name, jint skip_socket_connect)
{
	int cam_cnt = -1;

	cam_cnt = NX_3DSvm_ConnectToServer(client_name, skip_socket_connect);

	return cam_cnt;
}

/**********************************************************************************
 * disconnect from nx_3d_avm_daemon
 **********************************************************************************/
JNIEXPORT jint JNICALL NX_JniNxSvmDisconnectFromServer( JNIEnv *env, jclass obj, jint release_resource)
{
	int ret = -1;

	ret = NX_3DSvm_DisconnectFromServer(release_resource);

	return ret;
}
//------------------------------------------------------------------------------

/*******************************************************************************
 * Get QuickRearCam type (NxQuickRearCam / SVM / 1CamTopView
 **********************************************************************************/
JNIEXPORT jint JNICALL NX_JniGetQuickRearCamType( JNIEnv *env, jclass obj)
{
    FILE *fp;
    int r_size = 0;
    char cmdline[2048];
    char *ptr;
    long nxrear_cam = CAM_TYPE_QUICKREAR;
	

//#ifdef ANDROID_PIE
    fp = fopen("/proc/cmdline", "r");
    if(fp == NULL)
    {
        NxDbgMsg(NX_DBG_INFO, "/proc/cmdline read failed \n");
        //return -1;
		return (int)nxrear_cam;
    }

    r_size = fread(cmdline, 1, 2048, fp);
    if(r_size == 2048){
        NxDbgMsg(NX_DBG_INFO, "check size of cmdline!!\n");
		//return -1;
		return (int)nxrear_cam;
    }

    fclose(fp);

    ptr = strstr(cmdline, rearcam_type);
    if(ptr){
        int len = strlen(rearcam_type);
        nxrear_cam = strtol(ptr+len, NULL, 10);
    }else
    {
        nxrear_cam = CAM_TYPE_QUICKREAR;
    }
// #else
//     nxrear_cam = CAM_TYPE_QUICKREAR;
// #endif

    return (int)nxrear_cam;
}


/**********************************************************************************
 *
 **********************************************************************************/
//
// JNI Intialize
//
static JNINativeMethod sMethods[] = {
    // Native Function Name,            Signature,  C++ Function Name
    {"NX_JniGpioInit",                  "(I)I",     (void *)NX_JniGpioInit},
    {"NX_JniGpioDeInit",                "()I",      (void *)NX_JniGpioDeInit},
    {"NX_JniGpioSetDirection",          "(I)I",     (void *)NX_JniGpioSetDirection},
    {"NX_JniGpioGetValue",              "()I",      (void *)NX_JniGpioGetValue},
    {"NX_JniGpioSetEdge",               "(I)I",     (void *)NX_JniGpioSetEdge},
    {"NX_JniCreateCmdFile",             "(Ljava/lang/String;)I",      (void *)NX_JniCreateCmdFile},
    {"NX_JniWriteCmd",                  "(Ljava/lang/String;Z)I",      (void *)NX_JniWriteCmd},
    {"NX_JniNxStartCmdService",		    "(Ljava/lang/String;Z)I", 	  (void*)NX_JniNxStartCmdService 	},
    {"NX_JniCheckQuickRearCamStatus",   "()I",                        (void *)NX_JniCheckQuickRearCamStatus},
    {"NX_JniNXStopCommandService",		"()I", 	  (void*)NX_JniNXStopCommandService 	},
    {"NX_JniNxSvmConnectToServer",         "(II)I",                   (void*)NX_JniNxSvmConnectToServer      },
    {"NX_JniNxSvmDisconnectFromServer",    "(I)I",                     (void*)NX_JniNxSvmDisconnectFromServer },
    {"NX_JniGetQuickRearCamType",    "()I",                     (void*)NX_JniGetQuickRearCamType },

};

static int RegisterNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *gMethods, int numMethods)
{
    NxDbgMsg(NX_DBG_VBS, "%s()++", __FUNCTION__);

    jclass clazz;
    int result = JNI_FALSE;

    clazz = env->FindClass(className);
    if (clazz == NULL) {
        NxTrace("%s(): Native registration unable to find class '%s'", __FUNCTION__, className);
        goto FAIL;
    }

    if (env->RegisterNatives( clazz, gMethods, numMethods) < 0) {
        NxTrace("%s(): RegisterNatives failed for '%s'", __FUNCTION__, className);
        goto FAIL;
    }

    result = JNI_TRUE;

FAIL:
    NxDbgMsg(NX_DBG_VBS, "%s()--", __FUNCTION__);
    return result;
}

//------------------------------------------------------------------------------
jint JNI_OnLoad( JavaVM *vm, void *reserved )
{
    NxDbgMsg(NX_DBG_VBS, "%s()++", __FUNCTION__);

    jint result = -1;
    JNIEnv *env = NULL;

    if (g_pGpioControl == NULL) {
        g_pGpioControl = new NX_CGpioControl();
    }

    if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
        NxTrace("%s(): GetEnv failed!\n", __FUNCTION__);
        goto FAIL;
    }

    if (RegisterNativeMethods(env, "com/nexell/backgearservice/CtrlBackGearService", sMethods, sizeof(sMethods) / sizeof(sMethods[0]) ) != JNI_TRUE) {
        NxTrace("%s(): RegisterNativeMethods failed!", __FUNCTION__);
        goto FAIL;
    }

    result = JNI_VERSION_1_4;

FAIL:
    NxDbgMsg(NX_DBG_VBS, "%s()--", __FUNCTION__);
    return result;
}

//------------------------------------------------------------------------------
void JNI_OnUnload( JavaVM *vm, void *reserved )
{
    NxDbgMsg( NX_DBG_VBS, "%s()++", __FUNCTION__ );

    JNIEnv *env = NULL;

    if (g_pGpioControl) {
        delete g_pGpioControl;
        g_pGpioControl = NULL;
    }

    if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
        NxTrace( "%s(): GetEnv failed!", __FUNCTION__ );
        goto FAIL;
    }

FAIL:
    NxDbgMsg(NX_DBG_VBS, "%s()--", __FUNCTION__);
}

//-----------------------------------------------------------------------------
