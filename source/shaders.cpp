#include "gm82dx9.h"

VShaderWithTable current_vshader;
PShaderWithTable current_pshader;

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

extern IDirect3DTexture9 *white_pixel;

bool using_shader = false;

bool shader_bounds_check(const DWORD* data, size_t len) {
    if (len % 4 != 0 || len < 8) {
        return false;
    }
    // skip past header
    unsigned short header_len = ((unsigned short*)(data))[3];
    int data_start = header_len + 2;
    if ((data_start + 1) * 4 >= len) return false;
    // scan the instructions to see if there's an end command before EOF
    const DWORD* data_end = data + len / 4;
    const DWORD* cursor = data + data_start;
    while (true) {
        if ((*cursor & D3DSI_OPCODE_MASK) == D3DSIO_END) {
            break;
        }
        cursor += 1 + ((*cursor & D3DSI_INSTLENGTH_MASK) >> D3DSI_INSTLENGTH_SHIFT);
        if (cursor >= data_end) {
            return false;
        }
    }
    return true;
}

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
    // some integrity checks
    if (len == -1 || len % 4 != 0 || len < 8) {
        fclose(f);
        return nullptr;
    }
    unsigned short header_len;
    fseek(f, 6, SEEK_SET);
    if (fread(&header_len, 2, 1, f) != 1) {
        fclose(f);
        return nullptr;
    }
    int data_start = header_len + 2;
    if ((data_start + 1) * 4 >= len) {
        fclose(f);
        return nullptr;
    }
    // read the data
    fseek(f, 0, SEEK_SET);
    auto data = (DWORD*)malloc(len);
    if (data == nullptr || fread(data, 1, len, f) != len) {
        free(data);
        fclose(f);
        return nullptr;
    }
    fclose(f);
    if (!shader_bounds_check(data, len)) {
        free(data);
        return nullptr;
    }
    return data;
}

GMREAL shader_vertex_create_file(const char* filename) {
    DWORD* data = load_shader_data(filename);
    if (data == nullptr) return -1;
    IDirect3DVertexShader9 *shader;
    if (vibe_check(Device->CreateVertexShader(data, &shader))) {
        free(data);
        return -1;
    }
    ID3DXConstantTable* ppConstantTable;
    if (vibe_check(D3DXGetShaderConstantTable(data,&ppConstantTable))) {
        free(data);
        return -1;
    }
    free(data);
    
    VShaderWithTable shconst;
    
    shconst.shader=shader;
    shconst.constants=ppConstantTable;
    
    dx_data.vertex_shaders.insert(std::pair{++dx_data.idcounter_vertex, shconst});
    return dx_data.idcounter_vertex;
}

GMREAL __gm82dx9_shader_vertex_create_buffer(double buffer, double length) {
    DWORD* data = (DWORD*)(int)buffer;
    if (data == nullptr) return -1;
    if (!shader_bounds_check(data, int(length))) return -1;
    IDirect3DVertexShader9* shader;
    if (vibe_check(Device->CreateVertexShader(data, &shader))) {
        free(data);
        return -1;
    }
    ID3DXConstantTable* ppConstantTable;
    if (vibe_check(D3DXGetShaderConstantTable(data,&ppConstantTable))) {
        free(data);
        return -1;
    }
    
    VShaderWithTable shconst;
    
    shconst.shader=shader;
    shconst.constants=ppConstantTable;
    
    dx_data.vertex_shaders.insert(std::pair{++dx_data.idcounter_vertex, shconst});
    return dx_data.idcounter_vertex;
}

GMREAL shader_pixel_create_file(const char* filename) {
    DWORD* data = load_shader_data(filename);
    if (data == nullptr) return -1;
    IDirect3DPixelShader9 *shader;
    if (vibe_check(Device->CreatePixelShader(data, &shader))) {
        free(data);
        return -1;
    }
    ID3DXConstantTable* ppConstantTable;
    if (vibe_check(D3DXGetShaderConstantTable(data,&ppConstantTable))) {
        free(data);
        return -1;
    }
    free(data);    
    
    PShaderWithTable shconst;
    
    shconst.shader=shader;
    shconst.constants=ppConstantTable;

    dx_data.pixel_shaders.insert(std::pair{++dx_data.idcounter_pixel, shconst});
    return dx_data.idcounter_pixel;
}

GMREAL __gm82dx9_shader_pixel_create_buffer(double buffer, double length) {
    DWORD* data = (DWORD*)(int)buffer;
    if (data == nullptr) return -1;
    if (!shader_bounds_check(data, int(length))) return -1;
    IDirect3DPixelShader9 *shader;
    if (vibe_check(Device->CreatePixelShader(data, &shader))) {
        free(data);
        return -1;
    }
    ID3DXConstantTable* ppConstantTable;
    if (vibe_check(D3DXGetShaderConstantTable(data,&ppConstantTable))) {
        free(data);
        return -1;
    }
    
    PShaderWithTable shconst;
    
    shconst.shader=shader;
    shconst.constants=ppConstantTable;

    dx_data.pixel_shaders.insert(std::pair{++dx_data.idcounter_pixel, shconst});
    return dx_data.idcounter_pixel;
}

GMREAL shader_vertex_destroy(double shader_id) {
    auto it = dx_data.vertex_shaders.find(shader_id);
    if (it == dx_data.vertex_shaders.end()) return 1;
    
    VShaderWithTable vsh=it->second;    
    if (current_vshader.shader == vsh.shader) {
        Device->SetVertexShader(nullptr);
        using_shader = false;
    }    
    
    vsh.shader->Release();
    vsh.constants->Release();
    dx_data.vertex_shaders.erase(it);
    return 0;
}

GMREAL shader_pixel_destroy(double shader_id) {
    auto it = dx_data.pixel_shaders.find(shader_id);
    if (it == dx_data.pixel_shaders.end()) return 1;
    
    PShaderWithTable psh=it->second;    
    if (&current_pshader==&psh) {
        Device->SetPixelShader(nullptr);
    }
    
    psh.shader->Release();
    psh.constants->Release();
    dx_data.pixel_shaders.erase(it);
    return 0;
}

GMREAL shader_pixel_set(double shader_id) {
    if (shader_id < 0) return 1;
    auto it = dx_data.pixel_shaders.find(shader_id);
    if (it == dx_data.pixel_shaders.end()) return 1;
    current_pshader = it->second;
    vibe_check(Device->SetPixelShader(current_pshader.shader));
    current_pshader.constants->SetDefaults(Device);
    return 0;
}

GMREAL shader_vertex_set(double shader_id) {
    if (shader_id < 0) return 1;
    auto it = dx_data.vertex_shaders.find(shader_id);
    if (it == dx_data.vertex_shaders.end()) return 1;
    current_vshader = it->second;
    if (vibe_check(Device->SetVertexShader(current_vshader.shader))) return 1;
    using_shader = true;
    current_vshader.constants->SetDefaults(Device);
    return 0;
}

GMREAL shader_set(double vshader, double pshader) {
    shader_pixel_set(pshader);
    shader_vertex_set(vshader);
    return 0;
}

GMREAL shader_pixel_reset() {
    Device->SetPixelShader(nullptr);
    return 0;
}

GMREAL shader_vertex_reset() {
    Device->SetVertexShader(nullptr);
    using_shader = false;
    return 0;
}

GMREAL shader_reset() {
    shader_pixel_reset();
    shader_vertex_reset();
    return 0;
}

GMREAL __gm82dx9_get_vconst(const char* name) {
    unsigned int count;
    D3DXCONSTANT_DESC desc;

    if (!current_vshader.shader) return -4;
    if (current_vshader.constants->GetConstantDesc(current_vshader.constants->GetConstantByName(0, name), &desc, &count)!=D3D_OK) {
        return -4;        
    }
    
    return desc.RegisterIndex;
}

GMREAL __gm82dx9_get_vconst_size(const char* name) {
    unsigned int count;
    D3DXCONSTANT_DESC desc;

    if (!current_vshader.shader) return -4;
    if (current_vshader.constants->GetConstantDesc(current_vshader.constants->GetConstantByName(0, name), &desc, &count)!=D3D_OK) {
        return -4;        
    }
    
    return desc.RegisterCount;
}

GMREAL __gm82dx9_get_pconst(const char* name) {
    unsigned int count;
    D3DXCONSTANT_DESC desc;

    if (!current_pshader.shader) return -4;
    if (current_pshader.constants->GetConstantDesc(current_pshader.constants->GetConstantByName(0, name), &desc, &count)!=D3D_OK) {
        return -4;        
    }
    
    return desc.RegisterIndex;
}

GMREAL __gm82dx9_get_pconst_size(const char* name) {
    unsigned int count;
    D3DXCONSTANT_DESC desc;

    if (!current_pshader.shader) return -4;
    if (current_pshader.constants->GetConstantDesc(current_pshader.constants->GetConstantByName(0, name), &desc, &count)!=D3D_OK) {
        return -4;        
    }
    
    return desc.RegisterCount;
}

#define CONSTANT_FUNC(st_lo,st_up,ty_name,ty_lo,ty_up) \
    GMREAL __gm82dx9_shader_ ## st_lo ## _uniform_4 ## ty_lo (double reg, double v1, double v2, double v3, double v4) { \
        ty_name data[] = {(ty_name)v1, (ty_name)v2, (ty_name)v3, (ty_name)v4};            \
        Device->Set ## st_up ## ShaderConstant ## ty_up ((UINT)reg, data, 1);       \
        return 0;\
    }

CONSTANT_FUNC(vertex,Vertex,float,f,F)
CONSTANT_FUNC(pixel,Pixel,float,f,F)
CONSTANT_FUNC(vertex,Vertex,int,i,I)
CONSTANT_FUNC(pixel,Pixel,int,i,I)

#undef CONSTANT_FUNC

#define CONSTANT_BOOL(st_lo, st_up) \
    GMREAL __gm82dx9_shader_ ## st_lo ## _uniform_b(double reg, double b) { \
        BOOL data = b >= 0.5; \
        Device->Set ## st_up ## ShaderConstantB(reg, &data, 1); \
        return 0; \
    } \
    GMREAL __gm82dx9_shader_ ## st_lo ##_uniform_4b(double reg, double b1, double b2, double b3, double b4) { \
        BOOL data[] = {b1 >= 0.5, b2 >= 0.5, b3 >= 0.5, b4 >= 0.5}; \
        Device->Set ## st_up ## ShaderConstantB(reg, data, 4); \
        return 0; \
    }

CONSTANT_BOOL(vertex,Vertex)
CONSTANT_BOOL(pixel,Pixel)

#undef CONSTANT_BOOL

#define UNI_MATRIX(st_lo, st_up) \
    GMREAL __gm82dx9_shader_ ## st_lo ## _uniform_matrix(double reg, double mask_f, double size_f) { \
        int mask = mask_f; \
        int size = size_f; \
        XMMATRIX matrix; \
        if (mask & 1) { \
            Device->GetTransform(D3DTS_WORLD, (D3DMATRIX*)&matrix); \
        } else { \
            matrix = DirectX::XMMatrixIdentity(); \
        } \
        if (mask & 2) { \
            XMMATRIX view; \
            Device->GetTransform(D3DTS_VIEW, (D3DMATRIX*)&view); \
            matrix = DirectX::XMMatrixMultiply(matrix, view); \
        } \
        if (mask & 4) { \
            XMMATRIX projection; \
            Device->GetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&projection); \
            matrix = DirectX::XMMatrixMultiply(matrix, projection); \
        } \
        Device->Set ## st_up ## ShaderConstantF(reg, matrix.r->m128_f32, size); \
        return 0; \
    }
    
UNI_MATRIX(vertex, Vertex)
UNI_MATRIX(pixel, Pixel)
#undef UNI_MATRIX

#define CONSTANT_FUNC(st_lo,st_up,ty_name,ty_lo,ty_up) \
    GMREAL __gm82dx9_shader_ ## st_lo ## _uniform_ ## ty_lo ## _buffer(double reg, double buffer, double size) { \
        ty_name *data = (ty_name*)(int)buffer; \
        Device->Set ## st_up ## ShaderConstant ## ty_up ((UINT)reg, data, (UINT)size / 16); \
        return 0; \
    }

CONSTANT_FUNC(vertex,Vertex,float,f,F)
CONSTANT_FUNC(pixel,Pixel,float,f,F)
CONSTANT_FUNC(vertex,Vertex,int,i,I)
CONSTANT_FUNC(pixel,Pixel,int,i,I)
CONSTANT_FUNC(vertex,Vertex,BOOL,b,B)
CONSTANT_FUNC(pixel,Pixel,BOOL,b,B)

#undef CONSTANT_FUNC

GMREAL texture_get_interpolation() {
    DWORD retval;
    
    Device->GetSamplerState(0,D3DSAMP_MAGFILTER,&retval);
    
    if (retval==D3DTEXF_LINEAR) return 1;
    
    return 0;
}

void __gm82dx9_sampler_set(double stage, double tex_f) {
    int tex = tex_f;
    if (tex < 0) {
        Device->SetTexture(stage, white_pixel);
    } else {
        bool exists;
        __asm {
            mov eax, tex
            mov edx, 0x620ff8
            call edx
            mov exists, al
        }
        if (exists) {
            auto textures = (GMTexture**)0x85b3c4;
            Device->SetTexture(stage, (*textures)[tex].texture);
        }
    }
}

GMREAL __gm82dx9_texture_set_mode(double mode) {
    Device->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,(DWORD)(int)mode);    
    return 0;
}

GMREAL __gm82dx9_texture_stage_set(double stage, double tex_f) {
    if (stage<0 || stage>7) {
        show_error("Trying to set out-of-bounds sampler (0-7).");
        return 0;
    }
    
    __gm82dx9_sampler_set(stage,tex_f);
    return 0;
}

GMREAL __gm82dx9_texture_stage_vertex_set(double stage, double tex_f) {
    if (stage<0 || stage>3) {
        show_error("Trying to set out-of-bounds vertex shader sampler (0-3).");
        return 0;
    }
    
    __gm82dx9_sampler_set(stage+257,tex_f);
    return 0;
}

GMREAL __gm82dx9_texture_set_stage_interpolation(double stage, double linear_d) {
    if (stage<0 || stage>7) {
        show_error("Trying to set interpolation for out-of-bounds sampler (0-7).");
        return 0;
    }
    
    if (linear_d >= 0.5) {
        Device->SetSamplerState(stage,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
        Device->SetSamplerState(stage,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
    } else {
        Device->SetSamplerState(stage,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
        Device->SetSamplerState(stage,D3DSAMP_MINFILTER,D3DTEXF_POINT);
    }
    return 0;
}

GMREAL __gm82dx9_texture_set_stage_vertex_interpolation(double stage, double linear_d) {
    if (stage<0 || stage>3) {
        show_error("Trying to set interpolation for out-of-bounds vertex shader sampler (0-3).");
        return 0;
    }
    
    if (linear_d >= 0.5) {
        Device->SetSamplerState(stage+257,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
        Device->SetSamplerState(stage+257,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
    } else {
        Device->SetSamplerState(stage+257,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
        Device->SetSamplerState(stage+257,D3DSAMP_MINFILTER,D3DTEXF_POINT);
    }
    return 0;
}

GMREAL __gm82dx9_texture_set_stage_repeat(double stage, double hrepeat, double vrepeat, double bordercolor) {
    if (stage<0 || stage>7) {
        show_error("Trying to set repeat for out-of-bounds sampler (0-7).");
        return 0;
    }
    
    Device->SetSamplerState(stage,D3DSAMP_ADDRESSU,(DWORD)(int)hrepeat);    
    Device->SetSamplerState(stage,D3DSAMP_ADDRESSV,(DWORD)(int)vrepeat);    
    Device->SetSamplerState(stage,D3DSAMP_BORDERCOLOR,gm_col_to_dx9(bordercolor));    
    return 0;
}

GMREAL __gm82dx9_texture_set_stage_vertex_repeat(double stage, double hrepeat, double vrepeat, double bordercolor) {
    if (stage<0 || stage>3) {
        show_error("Trying to set repeat for out-of-bounds vertex shader sampler (0-3).");
        return 0;
    }
    
    Device->SetSamplerState(stage+257,D3DSAMP_ADDRESSU,(DWORD)(int)hrepeat);    
    Device->SetSamplerState(stage+257,D3DSAMP_ADDRESSV,(DWORD)(int)vrepeat);    
    Device->SetSamplerState(stage+257,D3DSAMP_BORDERCOLOR,gm_col_to_dx9(bordercolor));    
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

GMREAL __gm82dx9_surface_set_target_ext(double target, double surface) {
    DWORD index = max(1,min(3,(DWORD)target));    
    IDirect3DSurface9* surf = nullptr;
    
    if (surface>=0) {
        if (vibe_check(get_gm_texture(get_gm_surface(surface)->texture)->texture->GetSurfaceLevel(0, &surf))) return 0;
    }
    
    if (vibe_check(Device->SetRenderTarget(index,surf))) return 0;
    
    if (surf!=NULL) surf->Release();
    
    return 1;
}