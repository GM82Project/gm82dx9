cmake_minimum_required(VERSION 3.19)
project(gm82dx9 CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/O2 /GS- /W2 /WX")
endif()

include_directories("include")
link_directories("lib")

add_library(gm82dx9 SHARED source/gm_interface.cpp source/gm82dx9.cpp source/transform.cpp source/shaders.cpp source/inject.cpp source/dxerr/dxerr.cpp source/vertex_buffers.cpp)
target_link_libraries(gm82dx9 d3d9 d3dx9)

SET(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} /D \"SAFESEH:NO\"")
SET(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")


macro(compile_shader in_fn out_fn model)
	add_custom_command(
			TARGET gm82dx9
			PRE_BUILD
			COMMAND fxc /nologo /T${model} /Fo source/${out_fn} source/${in_fn}
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	)
endmacro()

compile_shader(vs_pass.hlsl vs_pass.vs3 vs_3_0)
compile_shader(ps_pass.hlsl ps_pass.ps3 ps_3_0)

set(UPX_PATH "upx" CACHE STRING "Path to upx.exe if applicable")
if(NOT UPX_PATH STREQUAL "")
	add_custom_command(
		TARGET gm82dx9
		POST_BUILD	
		COMMAND ${UPX_PATH} --lzma ${CMAKE_BINARY_DIR}/$<CONFIG>/gm82dx9.dll
	)
endif()

option(INSTALL_GEX "Install the generated .gex" OFF)

set(GEX_URL https://raw.githubusercontent.com/GM82Project/gm82gex/main/gm82gex.py)
set(GEX_PY ${CMAKE_SOURCE_DIR}/gm82gex.py)
file(DOWNLOAD ${GEX_URL} STATUS DOWNLOAD_STATUS)
list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
if(${STATUS_CODE} EQUAL 0)
	file(DOWNLOAD ${GEX_URL} ${GEX_PY})
else()
	message(WARNING "Couldn't redownload gm82gex.py")
endif()

if (INSTALL_GEX)
	add_custom_command(
		TARGET gm82dx9
		POST_BUILD
		COMMAND py ${GEX_PY} gm82dx9.gej
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Building .gex"
	)
else()
	add_custom_command(
		TARGET gm82dx9
		POST_BUILD
		COMMAND py ${GEX_PY} gm82dx9.gej --noinstall
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Building .gex"
	)
endif()