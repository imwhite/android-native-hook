/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <android/log.h>
#include <sys/system_properties.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <string>
#include <dlfcn.h>
#include <string.h>
#include "dlopen.h"
#include "dump_utils.h"
#include "device_func.h"

#if defined(__aarch64__)

#include "And64InlineHook.hpp"

#elif defined(__arm__)
#include "dx_hook.h"
#endif

#ifndef _Included_com_wrbug_xposeddemo_Native
#define _Included_com_wrbug_xposeddemo_Native



void start_hook_to_dump_dex(JNIEnv *env);


#endif
