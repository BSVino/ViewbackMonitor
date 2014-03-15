include_directories (
	${PROJECT_SOURCE_DIR}/../ext-deps/EASTL/include
	.
	../
	../common
	../common/math
	../tinker
)

if (WIN32)
	include_directories(${CMAKE_SOURCE_DIR}/../ext-deps/gl3w/include)
	include_directories(${CMAKE_SOURCE_DIR}/../ext-deps/pthreads-w32-2-8-0-release)
	include_directories(${CMAKE_SOURCE_DIR}/../ext-deps/ftgl-gl3/src)
	include_directories(${CMAKE_SOURCE_DIR}/../ext-deps/freetype-2.3.11/include)
endif (WIN32)

include_directories (
	${PROJECT_SOURCE_DIR}/../viewback
	${PROJECT_SOURCE_DIR}/../ext-deps/protobuf-2.5.0/vsprojects/include
)

include_directories(${CMAKE_SOURCE_DIR}/../ext-deps/SDL2-2.0.2/include)

add_definitions(-DPTW32_STATIC_LIB)

set (MONITOR_SOURCES
	main.cpp
	monitor_window.cpp

	panel_container.cpp
	panel_base.cpp
	panel_2d.cpp
	panel_console.cpp
	panel_time.cpp
)

set (VIEWBACK_CLIENT_SOURCES
	../../viewback/client/viewback_client.cpp
	../../viewback/client/viewback_data.cpp
	../../viewback/client/viewback_servers.cpp
	../../viewback/protobuf/data.pb.cc
)

if (WIN32)
	set (MONITOR_SOURCES ${MONITOR_SOURCES}
		monitor.rc
	)
endif (WIN32)

source_group(ViewbackClient FILES ${VIEWBACK_CLIENT_SOURCES})
source_group(Monitor FILES ${MONITOR_SOURCES})

add_executable(ViewbackMonitor WIN32 ${MONITOR_SOURCES} ${VIEWBACK_CLIENT_SOURCES})
target_link_libraries (ViewbackMonitor tinker)

if (WIN32)
	target_link_libraries(ViewbackMonitor ${CMAKE_SOURCE_DIR}/../ext-deps/SDL2-2.0.2/lib/x86/SDL2.lib)
	target_link_libraries(ViewbackMonitor ${CMAKE_SOURCE_DIR}/../ext-deps/SDL2-2.0.2/lib/x86/SDL2main.lib)

	target_link_libraries(ViewbackMonitor debug ${CMAKE_SOURCE_DIR}/../ext-deps/ftgl-gl3/src/Debug/ftgl-gl3.lib)
	target_link_libraries(ViewbackMonitor debug ${CMAKE_SOURCE_DIR}/../ext-deps/freetype-2.3.11/objs/win32/vc2013/freetype2311_D.lib)
	target_link_libraries(ViewbackMonitor debug ${PROJECT_SOURCE_DIR}/../ext-deps/pthreads-w32-2-8-0-release-vs2013/Debug/pthread.lib)
	target_link_libraries(ViewbackMonitor debug ${PROJECT_SOURCE_DIR}/../ext-deps/protobuf-2.5.0-vs2013/vsprojects/Debug/libprotobuf.lib)

	target_link_libraries(ViewbackMonitor optimized ${CMAKE_SOURCE_DIR}/../ext-deps/ftgl-gl3/src/RelWithDebInfo/ftgl-gl3.lib)
	target_link_libraries(ViewbackMonitor optimized ${CMAKE_SOURCE_DIR}/../ext-deps/freetype-2.3.11/objs/win32/vc2013/freetype2311.lib)
	target_link_libraries(ViewbackMonitor optimized ${PROJECT_SOURCE_DIR}/../ext-deps/pthreads-w32-2-8-0-release-vs2013/Release/pthread.lib)
	target_link_libraries(ViewbackMonitor optimized ${PROJECT_SOURCE_DIR}/../ext-deps/protobuf-2.5.0-vs2013/vsprojects/Release/libprotobuf.lib)
	target_link_libraries(ViewbackMonitor optimized dbghelp.lib)

	target_link_libraries(ViewbackMonitor opengl32 glu32 iphlpapi ws2_32)
endif (WIN32)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_libraries(ViewbackMonitor ${FREETYPE_LIBRARIES} ${LIBXML2_LIBRARIES})
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
