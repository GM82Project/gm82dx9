#define GMREAL __declspec(dllexport) double __cdecl
#define GMSTR __declspec(dllexport) char* __cdecl
#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
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
static D3DRASTER_STATUS raster_status;
D3DPRESENT_PARAMETERS* d3d8_present = (D3DPRESENT_PARAMETERS*)0x85b38c;

GMREAL __gm82dx8_cleardepth() {
    ((void (*)())0x563a8c)(); //clear depth buffer
    return 1;
}

GMREAL __gm82dx8_setfullscreen(double hz) {
    int z = (int)hz;
    
    *(int*)0x85af74 = 0;  //multisample off
    *(int*)0x85af7c = 3;  //swap effect copy
    *(int*)0x85b3a8 = !z; //windowed mode
    *(int*)0x85b3b8 = z;  //refresh rate
    
    ((void (*)())0x61f9f4)(); //display_reset()

    return 1;
}

GMREAL __gm82dx8_resize_backbuffer(double width, double height) {
    int iwidth = width;
    int iheight = height;
    const void *fun = (void*)0x61fbc0; //YoYo_resize_backbuffer
    __asm {
        mov eax, iwidth
        mov edx, iheight
        call fun
    }
    return 0;
}

GMREAL __gm82dx8_setpointscale(double size,double scaling,double minscale,double maxscale,double sprite) {
    float ps = (float)size;
    IDirect3DDevice8_SetRenderState(*d3d8_device,D3DRS_POINTSIZE,*(DWORD *)&ps);
    IDirect3DDevice8_SetRenderState(*d3d8_device,D3DRS_POINTSPRITEENABLE,(DWORD)sprite);
    IDirect3DDevice8_SetRenderState(*d3d8_device,D3DRS_POINTSCALEENABLE,(DWORD)scaling);
    if (scaling) {
        float ps = (float)minscale;
        IDirect3DDevice8_SetRenderState(*d3d8_device,D3DRS_POINTSIZE_MIN,*(DWORD *)&ps);
        ps = (float)maxscale;
        IDirect3DDevice8_SetRenderState(*d3d8_device,D3DRS_POINTSIZE_MAX,*(DWORD *)&ps);
        ps = 1.0f;
        IDirect3DDevice8_SetRenderState(*d3d8_device,D3DRS_POINTSCALE_B,*(DWORD *)&ps);        
        IDirect3DDevice8_SetRenderState(*d3d8_device,D3DRS_POINTSCALE_C,*(DWORD *)&ps); 
    } else {
        IDirect3DDevice8_SetRenderState(*d3d8_device,D3DRS_POINTSIZE_MIN,*(DWORD *)&ps);
        IDirect3DDevice8_SetRenderState(*d3d8_device,D3DRS_POINTSIZE_MAX,*(DWORD *)&ps);
    }
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
    if (mode>=0.5) newmode=D3DFILL_WIREFRAME;
    if (mode>=1.5) newmode=D3DFILL_SOLID;
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

///begin vsync shit

ULONGLONG resolution = 1000000, frequency = 1;

GMREAL __gm82dx8_dll_init() {
    QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
    return 0;    
}

GMREAL __gm82dx8_time_now() {
    ULONGLONG now;
    if (QueryPerformanceCounter((LARGE_INTEGER*)&now)) {
        return (double)(now*resolution/frequency);
    } else {
        return -1.0;
    }
}

GMREAL __gm82dx8_waitvblank() {    
    IDirect3DDevice8_GetRasterStatus(*d3d8_device,&raster_status);
    if (raster_status.InVBlank) return 1;
    return 0;    
}

GMREAL __gm82dx8_sleep(double ms) {
    SleepEx((DWORD)ms,TRUE);
    return 0;
}