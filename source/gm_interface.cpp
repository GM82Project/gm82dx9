#include "gm82dx9.h"

int* dx9_backbuffer_format = (int*)0x85b394;
IDirect3DDevice9** d3d9_device = (IDirect3DDevice9**)0x6886a8;

extern bool __vibe_check(const wchar_t* file, int line, HRESULT hr) {
    if (SUCCEEDED(hr)) return false;    
    wchar_t buf[1024];
    wchar_t errbuf[1024];
    DXGetErrorDescriptionW(hr, errbuf, 1024);
    _snwprintf_s(buf, 1024, L"DirectX9 error in file %s at line %i:\n%s\n%s",file,line,DXGetErrorStringW(hr),errbuf);
    MessageBoxW(0, buf, L"Warning", 0);
    return true;
}

wchar_t* make_wstr(const char* input) {
    int len = MultiByteToWideChar(CP_UTF8, 0, input, -1, NULL, 0);
    wchar_t* output = (wchar_t*)malloc(len*2);
    MultiByteToWideChar(CP_UTF8, 0, input, -1, output, len);
    return output;
}

extern void __show_error(const wchar_t* file, int line, const char* message) {
    wchar_t buf[1024];
    wchar_t* msg = make_wstr(message);    
    _snwprintf_s(buf, 1024, L"DirectX9 error in file %s at line %i:\n%s",file,line,msg);
    MessageBoxW(0, buf, L"Error", 0);
    free(msg);
    exit(1);
}

extern DWORD gm_col_to_dx9(double color) {
    int col=(int)round(color);
    return 0xff000000|((col & 0xff)<<16) + (col & 0xff00) + ((col & 0xff0000)>>16);
}

GMSurface** gm_surfaces = (GMSurface**)(0x84527c);
GMTexture** gm_textures = (GMTexture**)(0x85b3c4);

extern inline GMSurface* get_gm_surface(double id) {
    return &(*gm_surfaces)[int(id)];
}

extern inline GMTexture* get_gm_texture(int id) {
    return &(*gm_textures)[id];
}