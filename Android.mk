LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include $(LOCAL_PATH)/ $(LOCAL_PATH)/common $(LOCAL_PATH)/common/math $(LOCAL_PATH)/tinker
LOCAL_C_INCLUDES += ../viewback
LOCAL_C_INCLUDES += ../ext-deps/protobuf-2.5.0/src ../ext-deps/ftgl-gl3/src ../ext-deps/freetype-2.3.11/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../SDL2_image

LOCAL_CFLAGS += -DTINKER_OPENGLES_3 -DTINKER_NO_TOOLS

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	common/configfile.cpp \
	common/mempool.cpp \
	common/mtrand.cpp \
	common/stb_image.cpp \
	common/stb_image_write.cpp \
	common/parallelize.cpp \
	common/platform_android.cpp \
	common/sockets/sockets.cpp \
	common/math/color.cpp \
	common/math/matrix.cpp \
	common/math/quaternion.cpp \
	common/math/frustum.cpp \
	common/math/vector.cpp \
	common/math/mesh.cpp \
	datamanager/data.cpp \
	datamanager/dataserializer.cpp \
	common/protobuf/math.pb.cc \
	tinker/glgui/glgui.cpp \
	tinker/glgui/basecontrol.cpp \
	tinker/glgui/button.cpp \
	tinker/glgui/checkbox.cpp \
	tinker/glgui/colorpicker.cpp \
	tinker/glgui/droppablepanel.cpp \
	tinker/glgui/filedialog.cpp \
	tinker/glgui/label.cpp \
	tinker/glgui/menu.cpp \
	tinker/glgui/movablepanel.cpp \
	tinker/glgui/panel.cpp \
	tinker/glgui/picturebutton.cpp \
	tinker/glgui/rootpanel.cpp \
	tinker/glgui/scrollbar.cpp \
	tinker/glgui/slidingpanel.cpp \
	tinker/glgui/tree.cpp \
	tinker/glgui/textfield.cpp \
	tinker/renderer/renderer.cpp \
	tinker/renderer/renderingcontext.cpp \
	tinker/renderer/shaders.cpp \
	tinker/textures/materiallibrary.cpp \
	tinker/textures/texturelibrary.cpp \
	tinker/textures/texturesheet.cpp \
	tinker/shell.cpp \
	tinker/application.cpp \
	tinker/console.cpp \
	tinker/cvar.cpp \
	tinker/profiler.cpp \
	monitor/main.cpp \
	monitor/monitor_window.cpp \
	monitor/panel_container.cpp \
	monitor/panel_base.cpp \
	monitor/panel_2d.cpp \
	monitor/panel_console.cpp \
	monitor/panel_time.cpp \
	../../../viewback/client/viewback_client.cpp \
	../../../viewback/client/viewback_data.cpp \
	../../../viewback/client/viewback_servers.cpp \
	../../../viewback/protobuf/data.pb.cc \

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image protobuf250 ftgl3

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lGLESv3 -llog

include $(BUILD_SHARED_LIBRARY)
