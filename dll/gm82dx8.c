#define GMREAL __declspec(dllexport) double __cdecl
#define GMSTR __declspec(dllexport) char* __cdecl
#define _USE_MATH_DEFINES
#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <math.h>
#include "C:\DXSDK\include\d3d8.h"
#include "C:\DXSDK\include\d3dx8.h"

#pragma comment(lib, "d3d8.lib")
#pragma comment(lib, "d3dx8.lib")

typedef struct {
    int is_string;
    int padding;    
    double real;    
    char* string;
    int padding2;    
}GMVAL;

IDirect3DDevice8** d3d8_device = (IDirect3DDevice8**)0x6886a8;
static D3DVIEWPORT8 viewport;

GMREAL __gm82dx8_setpointsize(double size) {
    float ps = (float)size;
    IDirect3DDevice8_SetRenderState(*d3d8_device,D3DRS_POINTSIZE,*(DWORD *)&ps);
    return 0;
}

GMREAL __gm82dx8_setcolwrite(double red, double green, double blue, double alpha) {
    UINT mask = 0;
    if (alpha>=0.5) mask += D3DCOLORWRITEENABLE_ALPHA;
    if (red>=0.5) mask += D3DCOLORWRITEENABLE_RED;
    if (green>=0.5) mask += D3DCOLORWRITEENABLE_GREEN;
    if (blue>=0.5) mask += D3DCOLORWRITEENABLE_BLUE;
    IDirect3DDevice8_SetRenderState(*d3d8_device,D3DRS_COLORWRITEENABLE,*(DWORD *)&mask);
    return 0;
}

GMREAL __gm82dx8_set_fill_mode(double mode) {
    DWORD newmode=D3DFILL_POINT;
    if (mode>=0.5) mode=D3DFILL_WIREFRAME;
    if (mode>=1.5) mode=D3DFILL_SOLID;
    IDirect3DDevice8_SetRenderState(*d3d8_device,D3DRS_FILLMODE,newmode);    
    return 0;
}

GMREAL __gm82dx8_setviewport(double x, double y, double width, double height) {
    IDirect3DDevice8_GetViewport(*d3d8_device,&viewport);
    viewport.X=(DWORD)x;
    viewport.Y=(DWORD)y;
    viewport.Width=(DWORD)width;
    viewport.Height=(DWORD)height;
    
    IDirect3DDevice8_SetViewport(*d3d8_device,&viewport);
    
    return 0;
}

GMREAL __gm82dx8_setzscale(double znear, double zfar) {
    
    IDirect3DDevice8_GetViewport(*d3d8_device,&viewport);
    viewport.MinZ=(float)znear;
    viewport.MaxZ=(float)zfar;
    IDirect3DDevice8_SetViewport(*d3d8_device,&viewport);
    
    return 0;
}