cmake_minimum_required(VERSION 3.19)
project(gm82dx8 CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/O2 /GS- /W2 /WX")
endif()

include_directories("C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include")
link_directories("C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86")

add_library(gm82dx8 SHARED source/gm82dx8.cpp source/transform.cpp source/vsync.cpp source/shaders.cpp source/inject.cpp)
target_link_libraries(gm82dx8 d3d9 d3dx9 dxerr legacy_stdio_definitions)

SET(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} /D \"SAFESEH:NO\"")
SET(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")

option(INSTALL_GEX "Install the generated .gex" OFF)

if (INSTALL_GEX)
	add_custom_command(
		TARGET gm82dx8
		POST_BUILD
		COMMAND py build_gex.py gm82dx8.gej
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Building .gex"
	)
else()
	add_custom_command(
		TARGET gm82dx8
		POST_BUILD
		COMMAND py build_gex.py gm82dx8.gej --noinstall
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Building .gex"
	)
endif()