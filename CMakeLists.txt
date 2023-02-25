cmake_minimum_required(VERSION 3.19)
project(gm82dx9 CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/O2 /GS- /W2 /WX")
endif()

include_directories("include")
link_directories("lib")

add_library(gm82dx9 SHARED source/gm82dx9.cpp source/transform.cpp source/shaders.cpp source/inject.cpp source/dxerr/dxerr.cpp)
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

option(INSTALL_GEX "Install the generated .gex" OFF)

if (INSTALL_GEX)
	add_custom_command(
		TARGET gm82dx9
		POST_BUILD
		COMMAND py build_gex.py gm82dx9.gej
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Building .gex"
	)
else()
	add_custom_command(
		TARGET gm82dx9
		POST_BUILD
		COMMAND py build_gex.py gm82dx9.gej --noinstall
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Building .gex"
	)
endif()