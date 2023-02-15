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
    
    shader_data.vertex_shaders.insert(std::pair{++shader_data.idcounter_vertex, shconst});
    return shader_data.idcounter_vertex;
}

GMREAL __gm82dx9_shader_vertex_create_buffer(double buffer) {
    DWORD* data = (DWORD*)(int)buffer;
    if (data == nullptr) return -1;
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
    free(data);
    
    VShaderWithTable shconst;
    
    shconst.shader=shader;
    shconst.constants=ppConstantTable;
    
    shader_data.vertex_shaders.insert(std::pair{++shader_data.idcounter_vertex, shconst});
    return shader_data.idcounter_vertex;
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

    shader_data.pixel_shaders.insert(std::pair{++shader_data.idcounter_pixel, shconst});
    return shader_data.idcounter_pixel;
}

GMREAL __gm82dx9_shader_pixel_create_buffer(double buffer) {
    DWORD* data = (DWORD*)(int)buffer;
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

    shader_data.pixel_shaders.insert(std::pair{++shader_data.idcounter_pixel, shconst});
    return shader_data.idcounter_pixel;
}

GMREAL shader_vertex_get_uniform(double shader_id, const char* name) {
    if (shader_id < 0) return 1;
    auto it = shader_data.vertex_shaders.find(shader_id);
    if (it == shader_data.vertex_shaders.end()) return 1;
    
    ID3DXConstantTable* ctable=it->second.constants;
    
    D3DXHANDLE handle = ctable->GetConstantByName(0, name);
    
    unsigned int count;
    D3DXCONSTANT_DESC desc;    
    ctable->GetConstantDesc(handle, &desc, &count);   
    
    return desc.RegisterIndex;
}

GMREAL shader_pixel_get_uniform(double shader_id, const char* name) {
    if (shader_id < 0) return 1;
    auto it = shader_data.pixel_shaders.find(shader_id);
    if (it == shader_data.pixel_shaders.end()) return 1;
    
    ID3DXConstantTable* ctable=it->second.constants;
    
    D3DXHANDLE handle = ctable->GetConstantByName(0, name);
    
    unsigned int count;
    D3DXCONSTANT_DESC desc;    
    ctable->GetConstantDesc(handle, &desc, &count);   
    
    return desc.RegisterIndex;
}

GMREAL shader_pixel_set(double shader_id) {
    if (shader_id < 0) return 1;
    auto it = shader_data.pixel_shaders.find(shader_id);
    if (it == shader_data.pixel_shaders.end()) return 1;
    vibe_check(Device->SetPixelShader(it->second.shader));
    it->second.constants->SetDefaults(Device);
    return 0;
}

GMREAL shader_vertex_set(double shader_id) {
    if (shader_id < 0) return 1;
    auto it = shader_data.vertex_shaders.find(shader_id);
    if (it == shader_data.vertex_shaders.end()) return 1;
    if (vibe_check(Device->SetVertexShader(it->second.shader))) return 1;
    using_shader = true;
    it->second.constants->SetDefaults(Device);
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
    Device->SetPixelShader(nullptr);
    Device->SetVertexShader(nullptr);
    using_shader = false;
    return 0;
}

#define CONSTANT_FUNC(st_lo,st_up,ty_name,ty_lo,ty_up) \
    GMREAL shader_ ## st_lo ## _uniform_1 ## ty_lo (double reg, double v1) { \
        ty_name data[] = {(ty_name)v1, 0, 0, 0};            \
        Device->Set ## st_up ## ShaderConstant ## ty_up (reg, data, 1);   \
        return 0;\
    } \
    GMREAL shader_ ## st_lo ## _uniform_2 ## ty_lo (double reg, double v1, double v2) { \
        ty_name data[] = {(ty_name)v1, (ty_name)v2, 0, 0};            \
        Device->Set ## st_up ## ShaderConstant ## ty_up (reg, data, 1);   \
        return 0;\
    } \
    GMREAL shader_ ## st_lo ## _uniform_3 ## ty_lo (double reg, double v1, double v2, double v3) { \
        ty_name data[] = {(ty_name)v1, (ty_name)v2, (ty_name)v3, 0};            \
        Device->Set ## st_up ## ShaderConstant ## ty_up (reg, data, 1);   \
        return 0;\
    } \
    GMREAL shader_ ## st_lo ## _uniform_4 ## ty_lo (double reg, double v1, double v2, double v3, double v4) { \
        ty_name data[] = {(ty_name)v1, (ty_name)v2, (ty_name)v3, (ty_name)v4};            \
        Device->Set ## st_up ## ShaderConstant ## ty_up (reg, data, 1);       \
        return 0;\
    } \
    GMREAL shader_ ## st_lo ## _uniform_8 ## ty_lo (double reg, double v1, double v2, double v3, double v4, double v5, double v6, double v7, double v8) { \
        ty_name data[] = {(ty_name)v1, (ty_name)v2, (ty_name)v3, (ty_name)v4, (ty_name)v5, (ty_name)v6, (ty_name)v7, (ty_name)v8};            \
        Device->Set ## st_up ## ShaderConstant ## ty_up (reg, data, 2);       \
        return 0;\
    }

CONSTANT_FUNC(pixel,Pixel,float,f,F)
CONSTANT_FUNC(vertex,Vertex,float,f,F)

#define COPY_MATRIX(func,cons) \
    GMREAL shader_vertex_matrix_ ## func(double reg) { \
        D3DMATRIX mat; \
        Device->GetTransform(cons, &mat); \
        Device->SetVertexShaderConstantF(reg, mat.m[0], 4); \
        return 0;                  \
    }

COPY_MATRIX(w, D3DTS_WORLD)
COPY_MATRIX(v, D3DTS_VIEW)
COPY_MATRIX(p, D3DTS_PROJECTION)

GMREAL shader_vertex_matrix_wv(double reg) {
    XMMATRIX world, view;
    Device->GetTransform(D3DTS_WORLD, (D3DMATRIX*)&world);
    Device->GetTransform(D3DTS_VIEW, (D3DMATRIX*)&view);
    world = DirectX::XMMatrixMultiply(world, view);
    Device->SetVertexShaderConstantF(reg, world.r->m128_f32, 4);
    return 0;
}

GMREAL shader_vertex_matrix_vp(double reg) {
    XMMATRIX view, projection;
    Device->GetTransform(D3DTS_VIEW, (D3DMATRIX*)&view);
    Device->GetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&projection);
    view = DirectX::XMMatrixMultiply(view, projection);
    Device->SetVertexShaderConstantF(reg, view.r->m128_f32, 4);
    return 0;
}

GMREAL shader_vertex_matrix_wvp(double reg) {
    XMMATRIX world, view, projection;
    Device->GetTransform(D3DTS_WORLD, (D3DMATRIX*)&world);
    Device->GetTransform(D3DTS_VIEW, (D3DMATRIX*)&view);
    Device->GetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&projection);
    world = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(world, view), projection);
    Device->SetVertexShaderConstantF(reg, world.r->m128_f32, 4);
    return 0;
}

GMREAL texture_stage_set(double stage, double tex_f) {
    int tex = tex_f;
    if (tex < 0) {
        Device->SetTexture(stage, nullptr);
        return 0;
    }
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
    return 0;
}

GMREAL texture_stage_interpolation(double stage, double linear_d) {
	if (linear_d >= 0.5) {
		Device->SetSamplerState(stage,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
		Device->SetSamplerState(stage,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	} else {
		Device->SetSamplerState(stage,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
		Device->SetSamplerState(stage,D3DSAMP_MINFILTER,D3DTEXF_POINT);
	}
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