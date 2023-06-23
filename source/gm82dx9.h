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
#include <map>

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

extern int* dx9_backbuffer_format;

extern void (*runner_display_reset)();

extern D3DVIEWPORT9 viewport;
extern D3DRASTER_STATUS raster_status;
extern XMMATRIX world_matrix;
extern D3DPRESENT_PARAMETERS d3d_parameters;
extern D3DCAPS9 d3d_caps;
extern XMVECTOR vertex;

HRESULT WINAPI SetVertexShader(IDirect3DDevice9 *dev, DWORD fvf);
DWORD gm_col_to_dx9(double color);
    
bool __vibe_check(const wchar_t* file, int line, HRESULT hr);
void __show_error(const wchar_t* file, int line, const char* message);
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WFILE WIDE1(__FILE__)
#define vibe_check(a) __vibe_check(WFILE,__LINE__,a)
#define show_error(a) __show_error(WFILE,__LINE__,a)

union messy_matrix_glue {
    D3DMATRIX* matrix;
    int pointer;        
};

struct GMSurface {
    unsigned int texture, width, height;
    bool exists;
    IDirect3DSurface9* zbuffer;
};

struct GMTexture {
    IDirect3DTexture9* texture;
    unsigned int width, height, width_pow2, height_pow2;
    bool exists;
};

extern GMSurface** gm_surfaces;
extern GMTexture** gm_textures;

GMSurface* get_gm_surface(double id);
GMTexture* get_gm_texture(int id);

struct VShaderWithTable {
    IDirect3DVertexShader9* shader;
    ID3DXConstantTable* constants;
};

struct PShaderWithTable {
    IDirect3DPixelShader9* shader;
    ID3DXConstantTable* constants;
};

struct DXData {
    unsigned int idcounter_vertex, idcounter_pixel;

    std::map<unsigned int, VShaderWithTable> vertex_shaders;
    std::map<unsigned int, PShaderWithTable> pixel_shaders;

    ~DXData() {
        for (auto& shader : vertex_shaders) {            
            ((shader.second).shader)->Release();
            ((shader.second).constants)->Release();
        }
        for (auto& shader : pixel_shaders) {
            ((shader.second).shader)->Release();
            ((shader.second).constants)->Release();
        }
    }
};

extern DXData dx_data;
