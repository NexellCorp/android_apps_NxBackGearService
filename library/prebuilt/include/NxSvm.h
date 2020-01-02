/** @file */
//------------------------------------------------------------------------------
//
//	Copyright (C) 2018 NEXELL Co., Ltd All Rights Reserved
//	NEXELL Co. Proprietary & Confidential
//
//	Module     :
//	File       :
//	Description:
//	Author     : hyunjh(hyunjh@nexell.co.kr)
//	Export     :
//	History    :
//     2018-03-01 hyunjh	1st release
//------------------------------------------------------------------------------

#ifndef __NXAVM_H__
#define __NXAVM_H__

#include "avm_common_def.h"

// #ifdef __cplusplus
// extern "C" {
// #endif
//------------------------------------------------------------------------------
//
//    Defines
//
//------------------------------------------------------------------------------
#define NX_DBG			printf
#define NX_ERR			printf

//---------------------------------------------------------
//    Window size define
//---------------------------------------------------------
#define AVM_WINDOW_WIDTH				AVM_DISPLAY_WIDTH
#define AVM_WINDOW_HEIGHT				AVM_DISPLAY_HEIGHT

#define MAX_USER_PRESET	20
#define MAX_DEFAULT_PRESET 5

#define MAX_WINDOW 5

typedef struct tagWIN_INFO{
	int m_iWinIdx;
	int m_iWinX;
	int m_iWinY;
	int m_iWinWidth;
	int m_iWinHeight;
	int m_iInfoIdx;
}WIN_INFO;

typedef struct tagVIEW_POS_INFO{
	float eye_x;
	float eye_y;
	float eye_z;
	float target_x;
	float target_y;
	float target_z;
	float fov;
	float hflip;
}VIEW_POS_INFO;

typedef struct tagVIEW_MODE_INFO{
	bool 			m_bUserModeEn;
	char 			m_ViewModeName[100];
	int 			m_iNumofWin;
	WIN_INFO 		m_WinInfo[MAX_WINDOW];
	VIEW_POS_INFO 	m_ViewPosInfo[MAX_WINDOW];
}VIEW_MODE_INFO;

typedef struct tagAUTOCAL_INFO{
	int pos_x[4];
	int pos_y[4];
	int pos_z[4];
	int yaw[4];
	int pitch[4];
	int roll[4];
	int cam_stride;
	int cam_width;
	int cam_height;
	int pattern_size;
}AUTOCAL_INFO;

typedef struct tagDSP_INFO{
	int lcd_w;
	int lcd_h;
	int dsp_w;
	int dsp_h;
}DSP_INFO;

typedef struct tagSVM_INFO{
	int cam_cnt;
	int backgear_enable;
	int backgear_gpio_idx;
	float pos_scale;
	DSP_INFO dsp_info;
}SVM_INFO;

//------------------------------------------------------------------------------
//
//    Functions
//
//------------------------------------------------------------------------------

int32_t NX_3DSvm_ConnectToServer(int32_t client_name, int32_t skip_socket_connect);
int32_t NX_3DSvm_DisconnectFromServer(int32_t release_resource);
int32_t NX_3DSvm_Init(int32_t client_name, int32_t preset_cnt, int32_t create_resource, DSP_INFO* pDspInfo);
int32_t NX_3DSvm_Start();
int32_t NX_3DSvm_Stop();
int32_t NX_3DSvm_GetConnectionStatus();
void NX_3DSvm_Close();

//#ifdef BACKGEAR_SVM
enum {
	NX_BACKGEAR_NOTDETECTED = 0x0,
	NX_BACKGEAR_DETECTED = 0x1,
};

int32_t NX_StartBackGearDetectService( int32_t nGpio, int32_t nChkDelay );
void NX_StopBackGearDetectService();
void NX_RegisterBackGearEventCallBack(void *, void (*callback)(void*, int32_t));

//#else

int32_t NX_3DSvm_InitForViewModeEdit(int32_t client_name, int32_t m_iModeIdx, int32_t m_iWinIdx, bool new_conf, VIEW_MODE_INFO *m_pViewInfo, DSP_INFO* pDspInfo);
void NX_3DSvm_SetViewModeInfo(VIEW_MODE_INFO *m_pInfo, int32_t mode_idx);

void NX_3DSvm_AutoCalibration(bool autocal_onecam, AUTOCAL_INFO *pAutoCalInfo );
void NX_3DSvm_AutoCalibrationWithInitValue(bool autocal_onecam, AUTOCAL_INFO *pAutoCalInfo);
 
int32_t NX_3DSvm_GetCurrentViewMode();
const char* NX_3DSvm_GetCurrentViewModeName();
void NX_3DSvm_ChangeViewMode(int32_t idx_add, int32_t mode_idx, int32_t pgl_en, int32_t sensorwall_en, float pgl_steer_angle, float sensorwall_distance);

void NX_3DSvm_MoveViewPosition(VIEW_POS_INFO *m_pPosInfo, int32_t view_mode_idx, int32_t win_idx);
void NX_3DSvm_MoveViewPositionAnimation(VIEW_POS_INFO *m_pPosInfo, int32_t view_mode_idx, int32_t win_idx, uint32_t spring_smooth_time);

void NX_3DSvm_TouchPress(int32_t x, int32_t y);
void NX_3DSvm_TouchRelease(int32_t x, int32_t y);
void NX_3DSvm_TouchMove(int32_t x, int32_t y);

//#endif

// #ifdef __cplusplus
// }
// #endif
#endif
