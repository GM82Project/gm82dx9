#ifndef GM82DX8_H
#define GM82DX8_H

#define GMREAL extern "C" __declspec(dllexport) double __cdecl
#define GMSTR extern "C" __declspec(dllexport) char* __cdecl

#define create_c_function(rettype, name, addr, ...)\
    rettype (*name)(__VA_ARGS__) = (rettype(*)(__VA_ARGS__))addr;

#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <versionhelpers.h>
#include "d3d8/d3d8.h"
#include "C:/DXSDK/include/dxerr8.h"
#include "DirectXMath/DirectXMath.h"

#pragma comment(lib, "C:/DXSDK/lib/DxErr8.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

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
extern int isdwm;
extern HINSTANCE dwm_dll;
extern DLL_FUNC DwmIsCompositionEnabled;
extern DLL_FUNC DwmFlush;

extern IDirect3DDevice8** d3d8_device;
extern IDirect3DDevice8** d3d8_device_8;
extern D3DVIEWPORT8 viewport;
extern D3DRASTER_STATUS raster_status;
extern XMMATRIX world_matrix,matrix;
extern D3DPRESENT_PARAMETERS* d3d8_present;
extern D3DCAPS8* d3d8_caps;
extern XMVECTOR vertex;

#endif