#include "shaders.h"

ShaderData shader_data;

D3DVERTEXELEMENT9 elems_shape[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
        D3DDECL_END()
};

D3DVERTEXELEMENT9 elems_2d[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
        {0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        D3DDECL_END()
};

D3DVERTEXELEMENT9 elems_3d[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
        {0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
        {0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        D3DDECL_END()
};

IDirect3DVertexDeclaration9 *decl_shape = nullptr;
IDirect3DVertexDeclaration9 *decl_2d = nullptr;
IDirect3DVertexDeclaration9 *decl_3d = nullptr;

bool using_shader = false;

static DWORD* load_shader_data(const char* filename) {
    int len = MultiByteToWideChar(CP_UTF8, 0, filename, -1, nullptr, 0);
    auto wname = (wchar_t *)malloc(len*2);
    MultiByteToWideChar(CP_UTF8, 0, filename, -1, wname, len);
    FILE *f = nullptr;
    errno_t err = _wfopen_s(&f, wname, L"rb");
    free(wname);
    if (err) return nullptr;
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    if (len == -1 || len % 4 != 0) {
        fclose(f);
        return nullptr;
    }
    fseek(f, 0, SEEK_SET);
    auto data = (DWORD*)malloc(len);
    if (fread(data, 1, len, f) != len) {
        free(data);
        fclose(f);
        return nullptr;
    }
    fclose(f);
    return data;
}

GMREAL dx8_shader_vertex_create_file(const char* filename) {
    DWORD* data = load_shader_data(filename);
    if (data == nullptr) return -1;
    IDirect3DVertexShader9 *shader;
    if (FAILED((*d3d8_device)->CreateVertexShader(data, &shader))) {
        free(data);
        return -1;
    }
    free(data);
    shader_data.vertex_shaders.insert(std::pair{++shader_data.idcounter_vertex, shader});
    return shader_data.idcounter_vertex;
}

#include <d3dx9.h>

GMREAL dx8_shader_pixel_create_file(const char* filename) {
    DWORD* data = load_shader_data(filename);
    if (data == nullptr) return -1;
    IDirect3DPixelShader9 *shader;
    if (FAILED((*d3d8_device)->CreatePixelShader(data, &shader))) {
        free(data);
        return -1;
    }
    free(data);
    shader_data.pixel_shaders.insert(std::pair{++shader_data.idcounter_pixel, shader});
    return shader_data.idcounter_pixel;
}

GMREAL dx8_shader_pixel_set(double shader_id) {
    if (shader_id < 0) return 1;
    auto it = shader_data.pixel_shaders.find(shader_id);
    if (it == shader_data.pixel_shaders.end()) return 1;
    return FAILED((*d3d8_device)->SetPixelShader(it->second));
}

GMREAL dx8_shader_vertex_set(double shader_id) {
    if (shader_id < 0) return 1;
    auto it = shader_data.vertex_shaders.find(shader_id);
    if (it == shader_data.vertex_shaders.end()) return 1;
    if (FAILED((*d3d8_device)->SetVertexShader(it->second))) return 1;
    using_shader = true;
    return 0;
}

GMREAL dx8_shader_pixel_reset() {
    (*d3d8_device)->SetPixelShader(nullptr);
    return 0;
}

GMREAL dx8_shader_vertex_reset() {
    (*d3d8_device)->SetVertexShader(nullptr);
    using_shader = false;
    return 0;
}

HRESULT WINAPI SetVertexShader(IDirect3DDevice9 *dev, DWORD fvf) {
    if (using_shader) {
#define FVF(flags, name) \
            if (fvf == (flags)) { \
                if (decl_##name == nullptr) { \
                    HRESULT hr = dev->CreateVertexDeclaration(elems_##name, &decl_##name); \
                    if (FAILED(hr)) return hr; \
                } \
                return dev->SetVertexDeclaration(decl_##name); \
            }
        FVF(D3DFVF_XYZ | D3DFVF_DIFFUSE, shape)
        FVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, 2d)
        FVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1, 3d)
        return D3DERR_INVALIDCALL;
#undef FVF
    } else {
        return dev->SetFVF(fvf);
    }
}