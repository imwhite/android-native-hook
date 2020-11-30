//
// Created by boyliang on 2019/5/17.
//
#include <jni.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include "utils/log.h"
#include "utils/jni_utils.h"
#include "dump_dex.h"

#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#define JSTRING "Ljava/lang/String;"
#define JCONTEXT "Landroid/content/Context;"


JNIEXPORT jint JNICALL JNI_OnLoad
        (JavaVM *vm, void *reserved) {
    LOGI(" ----- JNI_OnLoad ----- uid:%d", (int)getuid());
    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    // JNIUtils_AndroidRuntime::init(vm);

    start_hook_to_dump_dex(env);
    LOGI(" ----- JNI_OnLoad ----- end");

    return JNI_VERSION_1_4;
}
