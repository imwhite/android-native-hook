//
// Created by WrBug on 2018/3/23.
//
#include "dump_dex.h"
#include "utils/log.h"
#include "dump_utils.h"
#include "dlopen.h"
#include "device_func.h"
#include "dx_hook.h"
#include <string>


void start_dump_dex(JNIEnv *env) {

    static bool is_hook = false;
    if (is_hook) {
        LOGE("hooked already");
        return;
    }

    ndk_init(env);
    void *handle = ndk_dlopen("libart.so", RTLD_NOW);
    if (handle == NULL) {
        LOGE("Error: unable to find the SO : libart.so");
        return;
    }
    void *open_common_addr = ndk_dlsym(handle, get_open_function_flag());
    if (open_common_addr == NULL) {
        LOGE("Error: unable to find the Symbol : ");
        return;
    }

    DXRetStatus status = dx_hook_hookfun((void *) open_common_addr, (void *) get_new_open_function_addr(),
                           (void **) get_old_open_function_addr());
    LOGI("hook start_dump_dex status:%d", status);               

    LOGI("start_dump_dex success:%d", get_old_open_function_addr != 0);
    is_hook = true;
}





