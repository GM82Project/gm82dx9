#define GMREAL __declspec(dllexport) double __cdecl
#define GMSTR __declspec(dllexport) char* __cdecl
#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
#include <versionhelpers.h>
#include "C:\DXSDK\include\d3d8.h"
#include "C:\DXSDK\include\d3dx8.h"
#include "C:\DXSDK\include\D3dx8math.h"
#include "C:\DXSDK\include\D3d8types.h"

#pragma comment(lib, "C:\\DXSDK\\lib\\d3d8.lib")
#pragma comment(lib, "C:\\DXSDK\\lib\\d3dx8.lib")
#pragma comment(lib, "Advapi32.lib")

#ifndef gm82dx8
#define gm82dx8

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
extern D3DMATRIX world_matrix,matrix;
extern D3DPRESENT_PARAMETERS* d3d8_present;
extern D3DCAPS8* d3d8_caps;

#endif