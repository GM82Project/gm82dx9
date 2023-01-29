#pragma once

#include "gm82dx8.h"
#include <map>

struct ShaderData {
    unsigned int idcounter_vertex, idcounter_pixel;

    std::map<unsigned int, IDirect3DVertexShader9*> vertex_shaders;
    std::map<unsigned int, IDirect3DPixelShader9*> pixel_shaders;

    ~ShaderData() {
        for (auto& shader : vertex_shaders) {
            shader.second->Release();
        }
        for (auto& shader : pixel_shaders) {
            shader.second->Release();
        }
    }
};
