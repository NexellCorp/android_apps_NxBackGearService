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

#ifndef __AVM_COMMON_DEF_H__
#define __AVM_COMMON_DEF_H__

//------------------------------------------------------------------------------
//
//    Includes
//    
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//
//    Defines
//    
//------------------------------------------------------------------------------

//---------------------------------------------------------
//    1. Target board defines (only for dual display example)
//       This values are included in avm_config.txt
//---------------------------------------------------------
#define NX_FEATURE_AVM_TARGET_BOARD_CONNECTOR_SUB_ID 	43
#define NX_FEATURE_AVM_TARGET_BOARD_CRTC_SUB_ID 		35
#define NX_FEATURE_AVM_TARGET_BOARD_PLANE_SUB_ID 		36

//---------------------------------------------------------
//    2. Input Camera Resolution defines (only for detecting one or four camera example at build.sh) 
//	     This values are included in avm_config.txt
//---------------------------------------------------------
#ifdef WIN32
#define AVM_CAMERA_COUNT			4
#define AVM_CAMERA_STRIDE			(1280 * AVM_CAMERA_COUNT)
#define AVM_CAMERA_WIDTH			1280
#define AVM_CAMERA_HEIGHT			720
#else
#if 0 /* chemtronics */
#define AVM_CAMERA_COUNT			4
#define AVM_CAMERA_STRIDE			(1280 * AVM_CAMERA_COUNT)
#define AVM_CAMERA_WIDTH			1280
#define AVM_CAMERA_HEIGHT			800
#elif 1 /* camsys */
#define AVM_CAMERA_COUNT			4
#define AVM_CAMERA_STRIDE			(1280 * AVM_CAMERA_COUNT)
#define AVM_CAMERA_WIDTH			1280
#define AVM_CAMERA_HEIGHT			720
#elif 0 /* one rear camera */
#define AVM_CAMERA_COUNT			1
#define AVM_CAMERA_STRIDE			(960 * AVM_CAMERA_COUNT)
#define AVM_CAMERA_WIDTH			960
#define AVM_CAMERA_HEIGHT			480
#elif 1 /* one rear camera, TW9900 */
#define AVM_CAMERA_COUNT			1
#define AVM_CAMERA_STRIDE			(704 * AVM_CAMERA_COUNT)
#define AVM_CAMERA_WIDTH			704
#define AVM_CAMERA_HEIGHT			480
#endif
#endif

//---------------------------------------------------------
//    3. Display Size defines (you should check)
//---------------------------------------------------------
#ifdef WIN32
	#define AVM_DISPLAY_LCD_WIDTH	1920//1280//1920
	#define AVM_DISPLAY_LCD_HEIGHT	720//1080
	#define AVM_DISPLAY_WIDTH		1920//1280//1920
	#define AVM_DISPLAY_HEIGHT		720//1080
#else
	#if 1
	#define AVM_DISPLAY_LCD_WIDTH	1920
	#define AVM_DISPLAY_LCD_HEIGHT	720
	#define AVM_DISPLAY_WIDTH		1920
	#define AVM_DISPLAY_HEIGHT		720 
	#else /* HDMI LCD */
	#define AVM_DISPLAY_LCD_WIDTH	1920
	#define AVM_DISPLAY_LCD_HEIGHT	1080
	#define AVM_DISPLAY_WIDTH		1440
	#define AVM_DISPLAY_HEIGHT		810
	#endif
#endif
#define AVM_DISPLAY_LCD_SUB_WIDTH	AVM_DISPLAY_LCD_WIDTH
#define AVM_DISPLAY_LCD_SUB_HEIGHT	AVM_DISPLAY_LCD_HEIGHT
#define AVM_DISPLAY_SUB_WIDTH	AVM_DISPLAY_WIDTH
#define AVM_DISPLAY_SUB_HEIGHT	AVM_DISPLAY_HEIGHT

//---------------------------------------------------------
//    4. PATH define (you should check)
//---------------------------------------------------------
#if defined( WIN32 ) 	//------------------------------------------
#define AVM_CONFIG_QUICK_ENV_FILE 		 "./avm/avm_config.txt"
#define AVM_CONFIG_QUICK_ONECAM_ENV_FILE "./avm_onecam/avm_config_onecam.txt"
#define AVM_INFO_TOPVIEW_FILE 			 "./avm/avm_info_topview.data"
#define AVM_INFO_TOPVIEW_ONECAM_FILE 	 "./avm_onecam/avm_info_topview.data"
#define AVM_INFO_3DVIEW_FILE 			 "./avm/avm_info_3dview.data"
#elif defined( linux )
#define AVM_CONFIG_QUICK_ENV_FILE 		 "/product/avm/avm_config.txt"
#define AVM_CONFIG_QUICK_ONECAM_ENV_FILE "/product/avm_onecam/avm_config_onecam.txt"
#define AVM_INFO_TOPVIEW_FILE 			 "/product/avm/avm_info_topview.data"
#define AVM_INFO_TOPVIEW_ONECAM_FILE 	 "/product/avm_onecam/avm_info_topview.data"
#define AVM_INFO_3DVIEW_FILE 			 "/product/avm/avm_info_3dview.data"
#else
#error "choose valid platform!!!"
#endif

#if defined( WIN32 ) 	//------------------------------------------
#define AVM_SENSOR_WALL_IMG_PATH		"./ui/wall.bmp"
#define AVM_PGL_IMG_PATH				"./ui/pgl_128_128_4.rgba"
#elif defined( linux )
#define AVM_SENSOR_WALL_IMG_PATH		"/oem/ui/wall.bmp"
#define AVM_PGL_IMG_PATH				"/oem/ui/pgl_128_128_4.rgba"
#else
#error "choose valid platform!!!"
#endif

#ifdef NX_FEATURE_AVM_TARGET_YUV422_EN  /* YUV */
#if defined( WIN32 ) 	//------------------------------------------
#	define AVM_CAR_MODEL_DATA_PATH 			"./3DS/sonata2018/sonata2018_01_yuv_high.nx3"
#elif defined( linux )
#	define AVM_CAR_MODEL_DATA_PATH 			"/oem/3DS/sonata2018/sonata2018_01_yuv_high.nx3"
#else
#error "choose valid platform!!!"
#endif

#else /* RGBA */

#if defined( WIN32 ) 	//------------------------------------------
#	define AVM_CAR_MODEL_DATA_PATH 			"./3DS/sonata2018/sonata2018_01_rgb_high.nx3"
#elif defined( linux )
#	define AVM_CAR_MODEL_DATA_PATH 			"/oem/3DS/sonata2018/sonata2018_01_rgb_high.nx3"
#else
#error "choose valid platform!!!"
#endif
#endif

//---------------------------------------------------------
//    5. Sub detailed defines (you should check)
//---------------------------------------------------------
//---------------------------------------------------------
//    Mini car frame defines for sub detailed defines (you should check)
//---------------------------------------------------------
#define AVM_TEMP_SCALE  					1.0
#define AVM_CAR_MODEL_WIDTH					1800
#define AVM_CAR_MODEL_HEIGHT				1550
#define AVM_CAR_MODEL_LENGTH 				4650//4600//4380//4165
#define AVM_PGL_LENGTH						(((float)AVM_CAR_MODEL_LENGTH * 0.6f)) 
#define AVM_PGL_WIDTH						1700
#define AVM_PGL_REAR_OFFSET 				(AVM_PGL_LENGTH/2)
#define AVM_PGL_TYRE_WIDTH					(130 * AVM_TEMP_SCALE)
#define AVM_PGL_UI_LINE_LENGTH				(8000.f * AVM_TEMP_SCALE) 
#define AVM_SENSORWALL_HEIGHT				5200
//---------------------------------------------------------
//	  Calibration defines  
//---------------------------------------------------------
#define AVM_PATTERN_CIRCLE_SIZE 			1400.f

//---------------------------------------------------------
//    Build option defines
//---------------------------------------------------------
#define AVM_BUILD_DRAW_CAR_EN
#define AVM_BUILD_GUIDE_LINE_EN
#define AVM_BUILD_SENSOR_WALL_EN

//---------------------------------------------------------
//    MACRO defines for YUV display
//---------------------------------------------------------
#define CONVERT_REAL_FB_SIZE_X(x)	(((float)(x)* (float)AVM_DISPLAY_WIDTH)/(float)AVM_DISPLAY_LCD_WIDTH)
#define CONVERT_REAL_FB_SIZE_Y(y)	(((float)(y)* (float)AVM_DISPLAY_HEIGHT)/(float)AVM_DISPLAY_LCD_HEIGHT)

//---------------------------------------------------------
//    Cmd parameter defines
//---------------------------------------------------------
typedef enum
{
	NX_AVM_CMD_DISPLAY_PROPERTY_VIDEO_PRIORITY,
	NX_AVM_CMD_DISPLAY_PROPERTY_IS_SECOND_DISPLAY_CREATED,
	NX_AVM_CMD_DISPLAY_PROPERTY_MAX
}NX_AVM_CMD_DISPLAY_PROPERTY;

typedef enum
{
	NX_AVM_CMD_SCENE_ANI_TOUCH_NONE,
	NX_AVM_CMD_SCENE_ANI_TOUCH_PRESS,
	NX_AVM_CMD_SCENE_ANI_TOUCH_DRAG,
	NX_AVM_CMD_SCENE_ANI_TOUCH_RELEASE,
	NX_AVM_CMD_SCENE_ANI_TOUCH_MODE_MAX
}NX_AVM_CMD_SCENE_ANI_TOUCH_MODE;

#endif
