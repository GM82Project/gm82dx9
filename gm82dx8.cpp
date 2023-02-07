#include "gm82dx8.h"

#define Device (*d3d8_device)

int has_started = 0;

D3DVIEWPORT8 viewport;
D3DRASTER_STATUS raster_status;
XMMATRIX world_matrix,matrix;
XMVECTOR vertex;

IDirect3DDevice8** d3d8_device = (IDirect3DDevice8**)0x6886a8;
IDirect3DDevice8** d3d8_device_8 = (IDirect3DDevice8**)0x58d388;
D3DPRESENT_PARAMETERS* d3d8_present = (D3DPRESENT_PARAMETERS*)0x85b38c;
D3DCAPS8* d3d8_caps = (D3DCAPS8*)0x85aea0;

create_c_function(void,runner_display_reset,0x61f9f4);
create_c_function(void,runner_clear_depth,0x563a8c);

int* dx8_present_param_ms = (int*)0x85af74;
int* dx8_present_param_swap = (int*)0x85af7c;
int* dx8_present_param_window = (int*)0x85b3a8;
int* dx8_present_param_hz = (int*)0x85b3b8;
int* dx8_backbuffer_format = (int*)0x85b394;

DWORD gm_col_to_dx8(double color) {
    int col=(int)round(color);
    return 0xff000000|((col & 0xff)<<16) + (col & 0xff00) + ((col & 0xff0000)>>16);
}
IDirect3DSurface8* get_gm_surface_depthbuffer(double id) {
    return (*(IDirect3DSurface8***)0x84527c)[4+5*int(id)];
}
bool __dx_vibe_check(char* func, HRESULT hr) {
    if (hr==D3D_OK) return 0;
    
    char buf[1024];
    snprintf(buf, 1024, "DirectX8 error in function %s:\n%s\n%s",func,DXGetErrorString8(hr),DXGetErrorDescription8(hr));
    MessageBox(0, buf, "Warning", 0);
    return 1;
}

GMREAL dx8_set_color_mask(double red, double green, double blue, double alpha) {
    UINT mask = 0;
    if (alpha>=0.5) mask += D3DCOLORWRITEENABLE_ALPHA;
    if (red>=0.5) mask += D3DCOLORWRITEENABLE_RED;
    if (green>=0.5) mask += D3DCOLORWRITEENABLE_GREEN;
    if (blue>=0.5) mask += D3DCOLORWRITEENABLE_BLUE;
    Device->SetRenderState(D3DRS_COLORWRITEENABLE,*(DWORD *)&mask);
    return 0;
}
GMREAL dx8_set_fill_mode(double mode) {
    DWORD newmode=D3DFILL_POINT;
    if (mode>=0.5) newmode=D3DFILL_WIREFRAME;
    if (mode>=1.5) newmode=D3DFILL_SOLID;
    Device->SetRenderState(D3DRS_FILLMODE,newmode);    
    return 0;
}
GMREAL dx8_set_cull_mode(double mode) {
    DWORD newmode=D3DCULL_NONE;
    if (mode>=0.5) newmode=D3DCULL_CW;
    if (mode>=1.5) newmode=D3DCULL_CCW;
    Device->SetRenderState(D3DRS_CULLMODE,newmode);    
    return 0;
}
GMREAL dx8_set_zbias(double bias) {
    DWORD newbias=(DWORD)round(max(0.0,min(16.0,bias)));
    Device->SetRenderState(D3DRS_ZBIAS,newbias);    
    return 0;
}

GMREAL __gm82dx8_setrangefog(double type,double color,double start,double end) {
    
    Device->SetRenderState(D3DRS_FOGENABLE,(DWORD)type);
    
    if (type>0) {
        Device->SetRenderState(D3DRS_FOGCOLOR,gm_col_to_dx8(color));
        
        float f_start=(float)start;
        float f_end=(float)end;
        if (type<2) {
            //pixel fog
            Device->SetRenderState(D3DRS_FOGTABLEMODE,D3DFOG_LINEAR);
            Device->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_NONE);
        } else {
            //vertex fog
            Device->SetRenderState(D3DRS_FOGTABLEMODE,D3DFOG_NONE);
            Device->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);
        }        
        
        Device->SetRenderState(D3DRS_FOGSTART,*(DWORD *)(&f_start));
        Device->SetRenderState(D3DRS_FOGEND,*(DWORD *)(&f_end));
        
        Device->SetRenderState(D3DRS_RANGEFOGENABLE,(type==2));
    }
    return 0;
}
GMREAL __gm82dx8_surface_set_depth(double id) {
	IDirect3DSurface8* render_target;
	if (__dx_vibe_check("__gm82dx8_surface_set_depth",Device->GetRenderTarget(&render_target)))
        return -1;
	if (__dx_vibe_check("__gm82dx8_surface_set_depth",Device->SetRenderTarget(render_target, get_gm_surface_depthbuffer(id))))
		return -1;
	return 0;
}

GMREAL __gm82dx8_dllcheck() {
    return 820;
}
GMREAL __gm82dx8_checkstart() {
    if (has_started) return 1;
    has_started = 1;
    return 0;
}
GMREAL __gm82dx8_cleardepth() {
    runner_clear_depth();
    return 1;
}
GMREAL __gm82dx8_setfullscreen(double hz) {
    int z = (int)hz;
    
    *dx8_present_param_ms = 0;  //multisample off
    *dx8_present_param_swap = 3;  //swap effect copy
    *dx8_present_param_window = !z; //windowed mode
    *dx8_present_param_hz = z;  //refresh rate
    
    runner_display_reset();

    return 1;
}
GMREAL __gm82dx8_resize_backbuffer(double width, double height) {
    int iwidth = (int)round(width);
    int iheight = (int)round(height);
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
    Device->SetRenderState(D3DRS_POINTSIZE,*(DWORD *)&ps);
    Device->SetRenderState(D3DRS_POINTSPRITEENABLE,(DWORD)sprite);
    Device->SetRenderState(D3DRS_POINTSCALEENABLE,(DWORD)scaling);
    if (scaling) {
        float ps = (float)minscale;
        Device->SetRenderState(D3DRS_POINTSIZE_MIN,*(DWORD *)&ps);
        ps = (float)maxscale;
        Device->SetRenderState(D3DRS_POINTSIZE_MAX,*(DWORD *)&ps);
        ps = 1.0f;
        Device->SetRenderState(D3DRS_POINTSCALE_B,*(DWORD *)&ps);        
        Device->SetRenderState(D3DRS_POINTSCALE_C,*(DWORD *)&ps); 
    } else {
        Device->SetRenderState(D3DRS_POINTSIZE_MIN,*(DWORD *)&ps);
        Device->SetRenderState(D3DRS_POINTSIZE_MAX,*(DWORD *)&ps);
    }
    return 0;
}
GMREAL __gm82dx8_setviewport(double x, double y, double width, double height) {
    Device->GetViewport(&viewport);
    viewport.X=(DWORD)x;
    viewport.Y=(DWORD)y;
    viewport.Width=(DWORD)width;
    viewport.Height=(DWORD)height;
    
    Device->SetViewport(&viewport);
    
    return 0;
}
GMREAL __gm82dx8_getviewportx() {
    Device->GetViewport(&viewport);
    return (double)viewport.X;
}
GMREAL __gm82dx8_getviewporty() {
    Device->GetViewport(&viewport);
    return (double)viewport.Y;
}
GMREAL __gm82dx8_getviewportw() {
    Device->GetViewport(&viewport);
    return (double)viewport.Width;
}
GMREAL __gm82dx8_getviewporth() {
    Device->GetViewport(&viewport);
    return (double)viewport.Height;
}
GMREAL __gm82dx8_setzscale(double znear, double zfar) {
    Device->GetViewport(&viewport);
    viewport.MinZ=(float)znear;
    viewport.MaxZ=(float)zfar;
    Device->SetViewport(&viewport);
    
    return 0;
}
GMREAL __gm82dx8_getvideomem() {
    return (double)(Device->GetAvailableTextureMem()/1048576);
}
GMREAL __gm82dx8_getmaxwidth() {
    return (double)d3d8_caps->MaxTextureWidth;
}
GMREAL __gm82dx8_getmaxheight() {
    return (double)d3d8_caps->MaxTextureHeight;
}
GMREAL __gm82dx8_transformvertex(double inx, double iny, double inz) {
	XMVECTOR in_vec = XMVectorSet(inx, iny, inz, 0.0);
    Device->GetTransform(D3DTS_WORLDMATRIX(0),reinterpret_cast<D3DMATRIX*>(&world_matrix));
    vertex = XMVector3TransformCoord(in_vec,world_matrix);
    return (double)XMVectorGetX(vertex);
}
GMREAL __gm82dx8_getvertexy() {
    return (double)XMVectorGetY(vertex);
}
GMREAL __gm82dx8_getvertexz() {
    return (double)XMVectorGetZ(vertex);
}
