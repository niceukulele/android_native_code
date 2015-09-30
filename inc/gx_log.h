/*
 * _log.h
 *
 *  Created on: 2015-1-29
 *      Author: hupeng
 */

#ifndef _GX_LOG_H_
#define _GX_LOG_H_
#include <android/log.h>
//extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
#if 0
typedef enum android_LogPriority {  
    ANDROID_LOG_UNKNOWN = 0,  
    ANDROID_LOG_DEFAULT,    /* only for SetMinPriority() */  
    ANDROID_LOG_VERBOSE,  
    ANDROID_LOG_DEBUG,  
    ANDROID_LOG_INFO,  
    ANDROID_LOG_WARN,  
    ANDROID_LOG_ERROR,  
    ANDROID_LOG_FATAL,  
    ANDROID_LOG_SILENT, /* only for SetMinPriority(); must be last */  
} android_LogPriority; 
#endif
#define LOG_TAG "gx_algorithm"

#define LOGV(fmt,args...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,"[%s]" fmt "",__FUNCTION__,##args)
#define LOGD(fmt,args...)  __android_log_print(ANDROID_LOG_ERROR  ,LOG_TAG,"[%s]" fmt "",__FUNCTION__,##args)
#define LOGI(fmt,args...)  __android_log_print(ANDROID_LOG_INFO   ,LOG_TAG,"[%s]" fmt "",__FUNCTION__,##args)
#define LOGW(fmt,args...)  __android_log_print(ANDROID_LOG_WARN   ,LOG_TAG,"[%s]" fmt "",__FUNCTION__,##args)
#define LOGE(fmt,args...)  __android_log_print(ANDROID_LOG_ERROR  ,LOG_TAG,"[%s]" fmt "",__FUNCTION__,##args)
#define LOGF(fmt,args...)  __android_log_print(ANDROID_LOG_FATAL  ,LOG_TAG,"[%s]" fmt "",__FUNCTION__,##args)

#endif /* LOG_H_ */


