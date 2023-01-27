#ifndef GM82DX8_H
#define GM82DX8_H

#define GMREAL extern "C" __declspec(dllexport) double __cdecl
#define GMSTR extern "C" __declspec(dllexport) char* __cdecl

#define create_c_function(rettype, name, addr, ...)\
    rettype (*name)(__VA_ARGS__) = (rettype(*)(__VA_ARGS__))addr;

#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
#include <versionhelpers.h>
#include "d3d9.h"
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
extern int isdwm;
extern HINSTANCE dwm_dll;
extern DLL_FUNC DwmIsCompositionEnabled;
extern DLL_FUNC DwmFlush;

extern IDirect3DDevice9** d3d8_device;
extern IDirect3DDevice9** d3d8_device_8;
extern D3DVIEWPORT9 viewport;
extern D3DRASTER_STATUS raster_status;
extern XMMATRIX world_matrix,matrix;
extern D3DPRESENT_PARAMETERS d3d_parameters;
extern D3DCAPS9 d3d_caps;
extern XMVECTOR vertex;

HRESULT WINAPI SetVertexShader(IDirect3DDevice9 *dev, DWORD fvf);

#endif