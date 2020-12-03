//
// Created by boyliang on 2019/5/17.
//
#include <jni.h>
#include <stdlib.h>
#include <algorithm>
#include <unistd.h>
#include <string>
#include "utils/log.h"
#include "utils/jni_utils.h"
#include "dump_dex/dump_dex.h"
#include "hook_S3.h"

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

    JniUtils::Init(vm);

    // start_dump_dex(env);
    
    hook_S3();


    LOGI(" ----- JNI_OnLoad ----- end");
    return JNI_VERSION_1_4;
}
