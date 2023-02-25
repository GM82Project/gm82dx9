#pragma once

#define GMREAL extern "C" __declspec(dllexport) double __cdecl
#define GMSTR extern "C" __declspec(dllexport) char* __cdecl

#define create_c_function(rettype, name, addr, ...)\
    rettype (*name)(__VA_ARGS__) = (rettype(*)(__VA_ARGS__))addr;

#define _USE_MATH_DEFINES

#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <versionhelpers.h>

#include "d3dx9.h"
#include "dxerr/dxerr.h"
#include "DirectXMath/DirectXMath.h"

using namespace DirectX;

typedef int (__cdecl *DLL_FUNC)(); 
typedef struct {
    int is_string;
    int padding;    
    double real;    
    char* string;
    int padding2;    
}GMVAL;

extern int has_started;

extern IDirect3DDevice9** d3d9_device;
#define Device (*d3d9_device)

extern void (*runner_display_reset)();

extern D3DVIEWPORT9 viewport;
extern D3DRASTER_STATUS raster_status;
extern XMMATRIX world_matrix;
extern D3DPRESENT_PARAMETERS d3d_parameters;
extern D3DCAPS9 d3d_caps;
extern XMVECTOR vertex;

HRESULT WINAPI SetVertexShader(IDirect3DDevice9 *dev, DWORD fvf);

bool __dx_vibe_check(const wchar_t* file, int line, HRESULT hr);
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WFILE WIDE1(__FILE__)
#define vibe_check(a) __dx_vibe_check(WFILE,__LINE__,a)
