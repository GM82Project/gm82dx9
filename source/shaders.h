#pragma once

#include "gm82dx9.h"
#include <map>

struct GMTexture {
    IDirect3DTexture9* texture;
    unsigned int width;
    unsigned int height;
    unsigned int width_pow2;
    unsigned int height_pow2;
    bool exists;
};

struct VShaderWithTable {
    IDirect3DVertexShader9* shader;
    ID3DXConstantTable* constants;
};

struct PShaderWithTable {
    IDirect3DPixelShader9* shader;
    ID3DXConstantTable* constants;
};

struct ShaderData {
    unsigned int idcounter_vertex, idcounter_pixel;

    std::map<unsigned int, VShaderWithTable> vertex_shaders;
    std::map<unsigned int, PShaderWithTable> pixel_shaders;

    ~ShaderData() {
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
