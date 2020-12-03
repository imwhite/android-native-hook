//
// Created by white on 2018/4/8.
//

#ifndef DEVICE_FUNC_H
#define DEVICE_FUNC_H

#include <sys/system_properties.h>
#include <stdlib.h>

#ifndef DUMPDEX_DEVICEUTIL_H
#define DUMPDEX_DEVICEUTIL_H

#endif //DUMPDEX_DEVICEUTIL_H

char *get_open_function_flag();

void **get_old_open_function_addr();

void *get_new_open_function_addr();

bool is_arm_64();

#endif // DEVICE_FUNC_H