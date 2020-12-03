//
// Created by WrBug on 2018/4/8.
//

#ifndef DUMPDEX_FILEUTILS_H
#define DUMPDEX_FILEUTILS_H


#include <sys/types.h>
#include <android/log.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

char *dump_init_dir();

void dump_get_file_path(char *path, size_t size, const char *filename);

int dump_to_file(const char *filepath, u_int8_t *data, size_t size);


#endif //DUMPDEX_FILEUTILS_H