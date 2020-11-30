//
// Created by WrBug on 2018/4/8.
//


#include "dump_utils.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "utils/log.h"
#include "utils/jni_utils.h"


static char _dump_dir[128] = {0};

//生成dex文件
static void get_file_name(char *name, int len, int dexlen) {
    init_dump_dir();
    memset(name, 0, len);
    sprintf(name, "%s/dump-%u.dex", _dump_dir, dexlen);
}

//保存dex文件
void save_dex_file(u_int8_t *data, size_t length) {
    char filename[1024];
    get_file_name(filename, sizeof(filename), length);
    if(access(filename, R_OK) == 0 ){
        LOGI("%s file exist", filename);
        return;
    }

    LOGI("dump dex file name is : %s", filename);
    LOGI("start dump");
    int fp = open(filename, O_CREAT | O_WRONLY, 0644);
    if (fp <= 0) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "open or create file error");
        return;
    }
    int ret = (int) write(fp, data, length);
    if (ret < 0) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "write file error");
    } else {
        LOGI("dump dex file success `%s`", filename);
    }
    close(fp);
     LOGI("save_dex_file end");
}


// 读取配置文件的包名
string get_pkg_name(){
    string pkgname;

    // 等待500ms，防止cmdline的值会是 <pre-initialized>
    usleep(1000 * 500);
    const char *path = "/proc/self/cmdline";
    FILE *fp = fopen(path, "rb");
    if(fp == NULL){
        return pkgname;
    }
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    size_t ret = fread(buf, 1, sizeof(buf), fp);
    if(ret > 0){
        pkgname = buf;
    }
    fclose(fp);
    return pkgname;
}

void init_dump_dir() {
    if(_dump_dir[0] != 0){
        return;
    }

    char dir[128] = {0};
    const char *pkg_name = get_pkg_name().c_str();
    LOGI("get_pkg_name %s", pkg_name);
    sprintf(dir, "/data/data/%s/dump", pkg_name);
    // 创建目录
    if(access(dir, R_OK) != 0){
        mode_t mode = umask(0);
        mkdir(dir, 755);
        umask(mode);
    }
    strcpy(_dump_dir, dir);


    // JNIUtils_JNIEnvGuard guard;
    // JNIEnv *env = guard.JNI();
    // jobject jContext = jni_getGlobalContext(env);
    // if(jContext == NULL){
        
    //     return;
    // }
    // jstring jParams1 = jni_string2jstring(env, "dump");
    // jobject jFile = jni_callMethodByName(env, jContext, "getDir", "(Ljava/lang/String;I)Ljava/io/File;", jParams1, 0);
    // jstring jPath = (jstring)jni_callMethodByName(env, jFile, "getAbsolutePath", "()Ljava/lang/String;");
    // string path = jni_jstring2string(env, jPath);

    // LOGI("jContext:%p", jContext);
    // LOGI("jParams1:%p", jParams1);
    // LOGI("jFile:%p", jFile);
    // LOGI("jPath:%p", jPath);
    // LOGI("path:%s", path.c_str());

    // jni_delete_obj(jParams1);
    // jni_delete_obj(jFile);
    // jni_delete_obj(jPath);

    // strcpy(_dump_dir, path.c_str());
}