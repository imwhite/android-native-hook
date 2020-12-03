//
// Created by white on 2020/12/1.
//


#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/mman.h>
#include <sys/system_properties.h>
#include <sys/user.h>
#include <jni.h>
#include <log.h>

#include "hook_S3.h"
#include "dump_utils.h"
#include "dx_hook.h"



// JNI_Onload
int (*JNI_OnLoad_old)(JavaVM *vm, void *reserved) = NULL;
static int JNI_OnLoad_new(JavaVM *vm, void *reserved) {
    LOGI("JNI_OnLoad_new");
    return JNI_OnLoad_old(vm, reserved);
}

// luaL_loadbufferx
int (*luaL_loadbufferx_old)(void *L, const char *buff, size_t size, const char *name, const char *mode) = NULL;
static int luaL_loadbufferx_new(void *L, const char *buff, size_t size, const char *name, const char *mode) {
    LOGI("luaL_loadbufferx_new L:%x, buff:%x, size:%d, name:%s, mode:%s", L, buff, size, name, mode);

    if (strstr(name, "script/game_config.lua") > (void *)0) {
        char path[128] = {0};
        dump_get_file_path(path, sizeof(path), "game_config.lua");

        int ret = dump_to_file(path, (uint8_t *)buff, size);
        LOGD("dump lua ret:%d", ret);
    }

    return luaL_loadbufferx_old(L, buff, size, name, mode);
}

static void do_hook_so_sym(const char *filename, void *handle) {
    // hook libdc JNI_OnLoad
    if (strstr(filename, "libdc.so") > (void *)0) {
        void *addr = dlsym(handle, "JNI_OnLoad");
        LOGI("hook_JNI_Onload addr:%p", addr);
        DXRetStatus status = dx_hook_hookfun((void *) addr, (void *) JNI_OnLoad_new, (void **) &JNI_OnLoad_old);
        LOGI("hook_JNI_Onload JNI_OnLoad_old:%p", JNI_OnLoad_old);     
    }

    // hook libnative-lib luaL_loadbufferx	
    if (strstr(filename, "libnative-lib") > (void *)0) {
        void *addr = dlsym(handle, "luaL_loadbufferx");
        LOGI("hook addr:%x", addr);
        DXRetStatus status = dx_hook_hookfun((void *) addr, (void *) luaL_loadbufferx_new, (void **) &luaL_loadbufferx_old);
        LOGI("hook success:%x", luaL_loadbufferx_old);     
    }

}










/**
 * Android 9上会，System.loadLibrary 不会调用dlopen，直接调用android_dlopen_ext
 */
void* (*android_dlopen_ext_old)(const char*, int, const void*) = NULL;
void *android_dlopen_ext_new(const char *filename, int flag, const void *android_dlextinfo) {
    LOGI("android_dlopen_ext_new so:%s", filename);
    void *handle = android_dlopen_ext_old(filename, flag, android_dlextinfo);

    do_hook_so_sym(filename, handle);
    
    return handle;
}
void hook_dlopen() {
    void *handle = dlopen("libc.so", RTLD_LAZY);
    LOGI("hook_dlopen handle:%x", handle);
    if (!handle){
        return;
    }
    void *dlopen_addr = dlsym(handle, "android_dlopen_ext");
    LOGI("hook_dlopen addr:%x", dlopen_addr);
    DXRetStatus status = dx_hook_hookfun((void *) dlopen_addr, (void *) android_dlopen_ext_new, (void **) &android_dlopen_ext_old);
    LOGI("hook_dlopen suucess:%x", android_dlopen_ext_old);     

    dlclose(handle);
}


void hook_sym(const void *sym, const void *hook_sym) {
    
}

void hook_sym_by_name(const char *libname, const char *symname, const void *hook_sym) {
    LOGI("hook_sym_by_name libname:%s, symname:%s", libname, symname);
}




void hook_S3() {
    LOGI("hook_S3");
    // hook_log();
    hook_dlopen();
    // hook_dlsym();
    // hook_JNI_Onload();
}
