//
// Created by white on 2018/4/8.
//


#include "dump_utils.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "utils/log.h"
#include "utils/jni_utils.h"


static char _dump_dir[128] = {0};

// 读取配置文件的包名
static string get_pkg_name(){
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


char *dump_init_dir() {
    if(_dump_dir[0] != 0){
        return _dump_dir;
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
    return _dump_dir;

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

void dump_get_file_path(char *path, size_t size, const char *filename) {
    char *dir = dump_init_dir();
    memset(path, 0, size);
    sprintf(path, "%s/%s", dir, filename);
}

int dump_to_file(const char *filepath, u_int8_t *data, size_t size) {
    if (!data || size == 0){
        return -1;
    }

    int fp = open(filepath, O_CREAT | O_WRONLY, 0644);
    if (!fp){
        return -2;
    }
    ssize_t ret = write(fp, data, size);

    close(fp);

    return 0;
}



