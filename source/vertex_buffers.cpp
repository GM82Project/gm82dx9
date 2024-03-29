#include "gm82dx9.h"

#include <vector>

std::vector<D3DVERTEXELEMENT9> vformat_building;
std::array<WORD, 16> vformat_offsets;
std::array<BYTE, 14> usage_indices;

GMREAL __gm82dx9_vertex_create_buffer_from_buffer(double buffer, double length, double stride) {
    // create vertex buffer
    IDirect3DVertexBuffer9* vbuf;
    if (vibe_check(Device->CreateVertexBuffer(length, 0, 0, D3DPOOL_MANAGED, &vbuf, NULL))) {
        return -1;
    }
    // populate vertex buffer
    void* vbuf_data;
    vbuf->Lock(0, length, &vbuf_data, 0);
    memcpy(vbuf_data, (DWORD*)(int)buffer, length);
    vbuf->Unlock();
    // return vertex buffer
    VertexBuffer vbuf_with_stride = {.vbuf = vbuf, .stride = UINT(stride)};
    dx_data.vertex_buffers.insert(std::pair{++dx_data.idcounter_vbuf, vbuf_with_stride});
    return dx_data.idcounter_vbuf;
}

GMREAL vertex_buffer_get_size(double vbuf_id) {
    if (vbuf_id<0) return -1;
    auto it = dx_data.vertex_buffers.find(vbuf_id);
    if (it == dx_data.vertex_buffers.end()) return -1;
    D3DVERTEXBUFFER_DESC desc;
    it->second.vbuf->GetDesc(&desc);
    return (double)desc.Size;
}

GMREAL index_buffer_get_size(double ibuf_id) {
    if (ibuf_id<0) return -1;
    auto it = dx_data.index_buffers.find(ibuf_id);
    if (it == dx_data.index_buffers.end()) return -1;
    D3DINDEXBUFFER_DESC desc;
    it->second->GetDesc(&desc);
    return (double)desc.Size;
}

GMREAL index_buffer_get_format(double ibuf_id) {
    if (ibuf_id<0) return -1;
    auto it = dx_data.index_buffers.find(ibuf_id);
    if (it == dx_data.index_buffers.end()) return -1;
    D3DINDEXBUFFER_DESC desc;
    it->second->GetDesc(&desc);
    return (double)desc.Format;
}

GMREAL vertex_bind_buffer(double vbuf_id, double slot) {
    if (vbuf_id < 0) {
        Device->SetStreamSource(slot, nullptr, 0, 1);
    } else {
        auto it = dx_data.vertex_buffers.find(vbuf_id);
        if (it == dx_data.vertex_buffers.end()) return 1;
        if (vibe_check(Device->SetStreamSource(slot, it->second.vbuf, 0, it->second.stride))) {
            return 1;
        }
    }
    return 0;
}

GMREAL vertex_set_instance_count(double count) {
    if (vibe_check(Device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | (unsigned int)count))) return 1;
    return 0;
}

GMREAL vertex_set_instances_per_vertex(double bufslot, double size) {
    if (vibe_check(Device->SetStreamSourceFreq((unsigned int)bufslot, D3DSTREAMSOURCE_INSTANCEDATA | (unsigned int)size))) return 1;
    return 0;
}

GMREAL vertex_instance_reset() {
	for (int i = 0; i < 16; i++) Device->SetStreamSourceFreq(i, 1);
	return 0;
}

GMREAL __gm82dx9_vertex_draw_buffer(double vbuf_id, double vformat_id, double primitive_type, double texture_id, double vert_count, double indexed) {
    auto bit = dx_data.vertex_buffers.find(vbuf_id);
    if (bit == dx_data.vertex_buffers.end()) return 1;
    auto fit = dx_data.vertex_formats.find(vformat_id);
    if (fit == dx_data.vertex_formats.end()) return 1;
    
    if (vibe_check(Device->SetStreamSource(0, bit->second.vbuf, 0, bit->second.stride))) return 1;
    if (vibe_check(Device->SetVertexDeclaration(fit->second.decl))) return 1;
    
    __gm82dx9_sampler_set(0, texture_id);
    
    int prim_count;
    switch (D3DPRIMITIVETYPE(primitive_type)) {
        case D3DPT_POINTLIST: prim_count = vert_count; break;
        case D3DPT_LINELIST: prim_count = vert_count / 2; break;
        case D3DPT_LINESTRIP: prim_count = vert_count - 1; break;
        case D3DPT_TRIANGLELIST: prim_count = vert_count / 3; break;
        case D3DPT_TRIANGLESTRIP:
        case D3DPT_TRIANGLEFAN: prim_count = vert_count - 2; break;
    }
    
    if (indexed >= 0.5) {
        Device->DrawIndexedPrimitive(D3DPRIMITIVETYPE(primitive_type), 0, 0, vert_count, 0, prim_count);
    } else {
        Device->DrawPrimitive(D3DPRIMITIVETYPE(primitive_type), 0, prim_count);
    }
    return 0;
}

GMREAL vertex_delete_buffer(double vbuf_id) {
    auto it = dx_data.vertex_buffers.find(vbuf_id);
    if (it == dx_data.vertex_buffers.end()) return 1;

    it->second.vbuf->Release();
    dx_data.vertex_buffers.erase(it);
    return 0;
}

GMREAL vertex_format_begin() {
    vformat_building.clear();
    vformat_offsets.fill(0);
	usage_indices.fill(0);
    return 0;
}

GMREAL vertex_format_add_custom(double type, double usage, double bufslot) {
    if (bufslot<0 || bufslot>15) {
        show_error("Incorrect slot for vertex format custom (0-15).");
        return 0;
    }
    vformat_building.push_back({
        .Stream = WORD(bufslot),
        .Offset = vformat_offsets[int(bufslot)],
        .Type = BYTE(type),
        .Method = D3DDECLMETHOD_DEFAULT,
        .Usage = BYTE(usage),
        .UsageIndex = usage_indices[usage]++,
    });
    int size = 0;
    switch (D3DDECLTYPE(type)) {
        case D3DDECLTYPE_FLOAT1:
        case D3DDECLTYPE_D3DCOLOR:
        case D3DDECLTYPE_UBYTE4:
        case D3DDECLTYPE_SHORT2:
        case D3DDECLTYPE_UBYTE4N:
        case D3DDECLTYPE_SHORT2N:
        case D3DDECLTYPE_USHORT2N:
        case D3DDECLTYPE_FLOAT16_2:
        case D3DDECLTYPE_UDEC3:
        case D3DDECLTYPE_DEC3N:
            size = 4;
            break;
        case D3DDECLTYPE_FLOAT2:
        case D3DDECLTYPE_SHORT4:
        case D3DDECLTYPE_SHORT4N:
        case D3DDECLTYPE_USHORT4N:
        case D3DDECLTYPE_FLOAT16_4:
            size = 8;
            break;
        case D3DDECLTYPE_FLOAT3:
            size = 12;
            break;
        case D3DDECLTYPE_FLOAT4:
            size = 16;
            break;
    }
    vformat_offsets[int(bufslot)] += size;
    return 0;
}

GMREAL vertex_format_end() {
    vformat_building.push_back(D3DDECL_END());
    IDirect3DVertexDeclaration9* vformat;
    if (vibe_check(Device->CreateVertexDeclaration(vformat_building.data(), &vformat))) {
        return -1;
    }
    // remove terminator
    vformat_building.pop_back();
    VertexFormat format = {vformat, vformat_offsets};
    dx_data.vertex_formats.insert(std::pair{++dx_data.idcounter_vformat, format});
    return dx_data.idcounter_vformat;
}

// TODO
GMREAL vertex_format_get_size(double vformat_id, double bufslot) {
    if (bufslot<0 || bufslot>15) {
        show_error("Incorrect slot for vertex format custom (0-15).");
        return -1;
    }
    auto it = dx_data.vertex_formats.find(vformat_id);
    if (it == dx_data.vertex_formats.end()) return -1;

    return it->second.sizes[bufslot];
}

GMREAL vertex_format_delete(double vformat_id) {
    auto it = dx_data.vertex_formats.find(vformat_id);
    if (it == dx_data.vertex_formats.end()) return 1;

    it->second.decl->Release();
    dx_data.vertex_formats.erase(it);
    return 0;
}

GMREAL __gm82dx9_index_create_buffer_from_buffer(double buffer, double length, double format) {
    // create index buffer
    IDirect3DIndexBuffer9* ibuf;
    if (vibe_check(Device->CreateIndexBuffer(length, D3DUSAGE_WRITEONLY, D3DFORMAT(format), D3DPOOL_MANAGED, &ibuf, 0))) {
        return -1;
    }
    // populate index buffer
    void* ibuf_data;
    ibuf->Lock(0, length, &ibuf_data, 0);
    memcpy(ibuf_data, (DWORD*)(int)buffer, length);
    ibuf->Unlock();
    // return index buffer
    dx_data.index_buffers.insert(std::pair{++dx_data.idcounter_ibuf, ibuf});
    return dx_data.idcounter_ibuf;
}

GMREAL index_set_buffer(double ibuf_id) {
    if (ibuf_id < 0) {
        Device->SetIndices(nullptr);
    } else {
        auto it = dx_data.index_buffers.find(ibuf_id);
        if (it == dx_data.index_buffers.end()) return 1;
        Device->SetIndices(it->second);
    }
    return 0;
}

GMREAL index_delete_buffer(double ibuf_id) {
    auto it = dx_data.index_buffers.find(ibuf_id);
    if (it == dx_data.index_buffers.end()) return 1;

    it->second->Release();
    dx_data.index_buffers.erase(it);
    return 0;
}
