LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/src)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
#$(call import-add-path,$(LOCAL_PATH)/../../IF/libs)
LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame
#LOCAL_CFLAGS += -Wno-psabi

#LOCAL_CPPFLAGS += -std=gnu++11 -D__STDC_INT64__ -Wno-error=format-security

#LOCAL_CPPFLAGS += -fexceptions
LOCAL_LDLIBS += -llog

include $(LOCAL_PATH)/Sources.mk
include $(LOCAL_PATH)/Includes.mk
include $(LOCAL_PATH)/breakpad.mk

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

#LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES := cocos2d_lua_static
LOCAL_STATIC_LIBRARIES += anti
LOCAL_STATIC_LIBRARIES += HoolaiGameProxy
LOCAL_STATIC_LIBRARIES += cpufeatures
# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END


include $(BUILD_SHARED_LIBRARY)

$(call import-module,anti)
$(call import-module,HoolaiFastAccess)
$(call import-module,.)
$(call import-module,scripting/lua-bindings/proj.android)
$(call import-module,android/cpufeatures)


# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
