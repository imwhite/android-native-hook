//
// Created by WrBug on 2018/3/23.
//
#include "dump_dex.h"
#include "utils/log.h"
#include "dump_utils.h"
#include <string>


void start_hook_to_dump_dex(JNIEnv *env) {

    static bool is_hook = false;
    if (is_hook) {
        LOGE("hooked already");
        return;
    }

    ndk_init(env);
    // init_dump_dir();
    void *handle = ndk_dlopen("libart.so", RTLD_NOW);
    if (handle == NULL) {
        LOGE("Error: unable to find the SO : libart.so");
        return;
    }
    void *open_common_addr = ndk_dlsym(handle, get_open_function_flag());
    if (open_common_addr == NULL) {
        LOGE(TAG,"Error: unable to find the Symbol : ");
        return;
    }

#if defined(__aarch64__)
    A64HookFunction(open_common_addr, get_new_open_function_addr(), get_old_open_function_addr());
    LOGI("loaded so: libart.so");
#elif defined(__arm__)
    DXRetStatus status = dx_hook_hookfun((void *) open_common_addr, (void *) get_new_open_function_addr(),
                           (void **) get_old_open_function_addr());
    LOGI("hook status%d", status);               
    if (status != DX_DONE) {
    // if (registerInlineHook((uint32_t) open_common_addr, (uint32_t) get_new_open_function_addr(),
    //                        (uint32_t **) get_old_open_function_addr()) != ELE7EN_OK) {
        LOGE("register1 hook failed!");
        return;
    } else {
        LOGE("register1 hook success!");
    }
    // if (inlineHook((uint32_t) open_common_addr) != ELE7EN_OK) {
    //     LOGE("register2 hook failed!");
    //     return;
    // } else {
    //     LOGE("register2 hook success!");
    // }
    LOGI("loaded so: libart.so");
#endif
    LOGI("hook init complete");
    is_hook = true;
}



