LOCAL_PATH := $(call my-dir)

# strip debug
# cmd-strip = $(TOOLCHAIN_PREFIX)strip --strip-debug -x $1

define walk
    $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef


# Project Config
include $(CLEAR_VARS)
LOCAL_MODULE    := inject
LOCAL_CFLAGS    += -Wall -fpermissive -DHAVE_PTHREAD -DGOOGLE_PROTOBUF_NO_RTTI
LOCAL_CPPFLAGS  += -frtti

# delete unuse function
LOCAL_CPPFLAGS += -ffunction-sections -fdata-sections
LOCAL_CFLAGS += -ffunction-sections -fdata-sections

# treat all warning as error, never delete this !
# LOCAL_CFLAGS += -Werror


# export symbol flag, default hidden all
LOCAL_CFLAGS += -fvisibility=hidden

LOCAL_LDLIBS += -llog -lz -latomic

SRC_DIR := $(LOCAL_PATH)/../../src

# ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)  
#   LOCAL_C_INCLUDES += ${NDK_ROOT}/sysroot/usr/include/arm-linux-androidabi/asm 
# endif  
# ifeq ($(TARGET_ARCH_ABI),armeabi)  
#   LOCAL_C_INCLUDES += ${NDK_ROOT}/sysroot/usr/include/arm-linux-androidabi/asm 
# endif  
# ifeq ($(TARGET_ARCH_ABI),arm64-v8a)  
#   LOCAL_C_INCLUDES += ${NDK_ROOT}/sysroot/usr/include/aarch64-linux-android/asm  
# endif  
# ifeq ($(TARGET_ARCH_ABI),x86)  
#   LOCAL_C_INCLUDES += ${NDK_ROOT}/sysroot/usr/include/i686-linux-android/asm   
# endif  
# ifeq ($(TARGET_ARCH_ABI),x86_64)  
#   LOCAL_C_INCLUDES += ${NDK_ROOT}/sysroot/usr/include/x86_64-linux-android/asm 
# endif


$(warning LOCAL_C_INCLUDES: $(LOCAL_C_INCLUDES))

LOCAL_C_INCLUDES +=                                             \
    $(SRC_DIR)/                                                 \
    $(SRC_DIR)/utils                                                 \
    $(SRC_DIR)/hook/include                                               \



SRC := $(wildcard                                               \
    $(SRC_DIR)/*.cc                                             \
    $(SRC_DIR)/*.cpp                                            \
    $(SRC_DIR)/*.c                                              \
    $(SRC_DIR)/utils/*.cc                                             \
    $(SRC_DIR)/utils/*.cpp                                            \
    $(SRC_DIR)/hook/substrate/*.cpp                              \
    $(SRC_DIR)/hook/*.cpp                              \
    $(SRC_DIR)/dump_dex/*.cpp                              \
    $(SRC_DIR)/dump_dex/*.c                              \
)                                                               \

# ifneq ($(TARGET_ARCH_ABI),arm64-v8a)  
#     SRC += $(wildcard                                             \
#         $(SRC_DIR)/substrate/*.cpp                              \
#     )                                                               \
#     $(warning SRC: $(SRC))
# endif 

LOCAL_SRC_FILES :=  $(SRC:$(LOCAL_PATH)/%=%)               \


# build so
include $(BUILD_SHARED_LIBRARY)

# build a
# include $(BUILD_STATIC_LIBRARY)


