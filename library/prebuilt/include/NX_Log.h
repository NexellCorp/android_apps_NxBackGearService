//------------------------------------------------------------------------------
//
//  Copyright (C) 2016 Nexell Co. All Rights Reserved
//  Nexell Co. Proprietary & Confidential
//
//  NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//  Name : NX_Log.h
//  Description : Nexell log macro
//  Author : Chris Lee.
//
//------------------------------------------------------------------------------
#ifndef __NXLOG_H__
#define __NXLOG_H__

#define NXLOG_VERSION	"0.1.0"

#ifndef __ANDROID__
// Use Linux logging system
#include <stdarg.h>
#include <string.h>
#include <syslog.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LOG_TAG
#define LOG_TAG "Unknown"
#endif

#define NXLOG_LEVEL_COUNT	6

enum {
	NXLOG_ERR = 0,
	NXLOG_WARNING,
	NXLOG_NOTICE,
	NXLOG_INFO,
	NXLOG_DEBUG,
	NXLOG_VERBOSE
};

#ifndef NXLOG_OPTION
#define NXLOG_OPTION    LOG_CONS | LOG_NDELAY | LOG_PID
#endif

#ifndef NXLOG_FACILITY
#define NXLOG_FACILITY  LOG_USER
#endif

#define NXLOGV(...) do { \
	    openlog(LOG_TAG"/V ", NXLOG_OPTION, NXLOG_FACILITY); \
	    syslog(LOG_DEBUG, ##__VA_ARGS__); \
	    closelog(); \
	} while(0)

#define NXLOGD(...) do { \
	    openlog(LOG_TAG"/D ", NXLOG_OPTION, NXLOG_FACILITY); \
	    syslog(LOG_DEBUG, ##__VA_ARGS__); \
	    closelog(); \
	} while(0)

#define NXLOGI(...) do { \
	    openlog(LOG_TAG"/I ", NXLOG_OPTION, NXLOG_FACILITY); \
	    syslog(LOG_INFO, ##__VA_ARGS__); \
	    closelog(); \
	} while(0)

#define NXLOGN(...) do { \
	    openlog(LOG_TAG"/N ", NXLOG_OPTION, NXLOG_FACILITY); \
	    syslog(LOG_NOTICE, ##__VA_ARGS__); \
	    closelog(); \
	} while(0)

#define NXLOGW(...) do { \
	    openlog(LOG_TAG"/W ", NXLOG_OPTION, NXLOG_FACILITY); \
	    syslog(LOG_WARNING, ##__VA_ARGS__); \
	    closelog(); \
	} while(0)

#define NXLOGE(...) do { \
	    openlog(LOG_TAG"/E ", NXLOG_OPTION, NXLOG_FACILITY); \
	    syslog(LOG_ERR, ##__VA_ARGS__); \
	    closelog(); \
	} while(0)

#ifdef __cplusplus
}
#endif

#else
// Use Android log daemon
//#include <cutils/log.h>
//#include <android/log.h>
#include <log/log.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Simplified macro to send a verbose log message using the current LOG_TAG.
 */
#ifndef ALOGV
#define __ALOGV(...) ((void)ALOG(LOG_VERBOSE, LOG_TAG, __VA_ARGS__))
#if LOG_NDEBUG
#define ALOGV(...) do { if (0) { __ALOGV(__VA_ARGS__); } } while (0)
#else
#define ALOGV(...) __ALOGV(__VA_ARGS__)
#endif
#endif

#ifndef ALOGV_IF
#if LOG_NDEBUG
#define ALOGV_IF(cond, ...)   ((void)0)
#else
#define ALOGV_IF(cond, ...) \
    ( (__predict_false(cond)) \
    ? ((void)ALOG(LOG_VERBOSE, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif
#endif

/*
 * Simplified macro to send a debug log message using the current LOG_TAG.
 */
#ifndef ALOGD
#define ALOGD(...) ((void)ALOG(LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGD_IF
#define ALOGD_IF(cond, ...) \
    ( (__predict_false(cond)) \
    ? ((void)ALOG(LOG_DEBUG, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send an info log message using the current LOG_TAG.
 */
#ifndef ALOGI
#define ALOGI(...) ((void)ALOG(LOG_INFO, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGI_IF
#define ALOGI_IF(cond, ...) \
    ( (__predict_false(cond)) \
    ? ((void)ALOG(LOG_INFO, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send a warning log message using the current LOG_TAG.
 */
#ifndef ALOGW
#define ALOGW(...) ((void)ALOG(LOG_WARN, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGW_IF
#define ALOGW_IF(cond, ...) \
    ( (__predict_false(cond)) \
    ? ((void)ALOG(LOG_WARN, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send an error log message using the current LOG_TAG.
 */
#ifndef ALOGE
#define ALOGE(...) ((void)ALOG(LOG_ERROR, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGE_IF
#define ALOGE_IF(cond, ...) \
    ( (__predict_false(cond)) \
    ? ((void)ALOG(LOG_ERROR, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

#define NXLOGV(...)	ALOGV(__VA_ARGS__)
#define NXLOGD(...)	ALOGD(__VA_ARGS__)
#define NXLOGI(...)	ALOGI(__VA_ARGS__)
#define NXLOGN(...)	ALOGI(__VA_ARGS__)
#define NXLOGW(...)	ALOGW(__VA_ARGS__)
#define NXLOGE(...)	ALOGE(__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif	// __ANDROID__
#endif	// __NXLOG_H__
