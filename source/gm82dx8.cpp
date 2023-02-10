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
GMREAL dx8_set_alphatest(double enable,double value,double cmpfunc) {
    Device->SetRenderState(D3DRS_ALPHATESTENABLE,(DWORD)(enable>0.5));    
    Device->SetRenderState(D3DRS_ALPHAREF,(DWORD)(int)value);  
    Device->SetRenderState(D3DRS_ALPHAFUNC,(DWORD)(int)cmpfunc);      
    return 0;
}
GMREAL dx8_set_texture_repeat(double hrepeat, double vrepeat, double bordercolor) {
    Device->SetTextureStageState(0,D3DTSS_ADDRESSU,(DWORD)(int)hrepeat);    
    Device->SetTextureStageState(0,D3DTSS_ADDRESSV,(DWORD)(int)vrepeat);    
    Device->SetTextureStageState(0,D3DTSS_BORDERCOLOR,gm_col_to_dx8(bordercolor));    
    return 0;
}
GMREAL dx8_set_texture_mode(double mode) {
    Device->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,(DWORD)(int)mode);    
    return 0;
}
GMREAL dx8_set_light(
    double index, double type,
    double x, double y, double z,
    double dx, double dy, double dz,
    double range, double inner_rad, double outer_rad,
    double color, double color_factor,
    double specular, double specular_factor
) {
    if (range<=0) return 0;
    
    D3DLIGHT8 light;
    
    ZeroMemory(&light, sizeof(light));
    
    light.Type=(D3DLIGHTTYPE)type;
    
    light.Position.x=x;
    light.Position.y=y;
    light.Position.z=z;
    light.Direction.x=dx;
    light.Direction.y=dy;
    light.Direction.z=dz;
    
    light.Falloff=1.0f;
    light.Theta=inner_rad/180*M_PI;
    light.Phi=outer_rad/180*M_PI;
    
    light.Attenuation0=0.8f;
    light.Attenuation1=0.0f;
    light.Attenuation2=0.1/range;
    light.Range=range;
    
    int col=(int)round(color);
    
    light.Diffuse.r=((col & 0xff)/0xff)*color_factor;
    light.Diffuse.g=(((col & 0xff00)>>8)/0xff)*color_factor;
    light.Diffuse.b=(((col & 0xff0000)>>16)/0xff)*color_factor;
    light.Diffuse.a=1.0f;
    
    col=(int)round(specular);
    
    light.Specular.r=(col & 0xff)/0xff;
    light.Specular.g=((col & 0xff00)>>8)/0xff;
    light.Specular.b=((col & 0xff0000)>>16)/0xff;
    light.Specular.a=1.0f;
    
    Device->SetLight((int)index,&light);
    Device->LightEnable((int)index,true);
    
    //set up material so specular actually works
    D3DMATERIAL8 mat;
    
    Device->GetMaterial(&mat);
    
    mat.Specular.r = 1.0f;
    mat.Specular.g = 1.0f;
    mat.Specular.b = 1.0f;
    mat.Specular.a = 1.0f;
    mat.Power = specular_factor;

    Device->SetMaterial(&mat);
    Device->SetRenderState(D3DRS_SPECULARENABLE,true);
    
    return 0;
}

GMREAL __gm82dx8_testfunc() {
    D3DLIGHT8 light;

    Device->GetLight(0,&light);
 
    return light.Attenuation2;
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
GMREAL __gm82dx8_buffer_to_surface(double buffer, double id, double gm_width, double gm_height) {    
    //parse arguments    
    char* src=(char*)(int)buffer;
    int gmw=(int)gm_width;
    int gmh=(int)gm_height;
    
    //get current render target
    //(surface by id is confusing)
    IDirect3DSurface8* surf;
    //surf=get_gm_surface_depthbuffer(id);
    if (__dx_vibe_check("__gm82dx8_buffer_to_surface",Device->GetRenderTarget(&surf))) return -1;  

    //query for actual width and height, and get gm w/h
    D3DSURFACE_DESC desc;
    if (__dx_vibe_check("__gm82dx8_buffer_to_surface",surf->GetDesc(&desc))) return -1;
    int width=desc.Width;
    int height=desc.Height;

    //create scratch surface
    IDirect3DSurface8* scratch;    
    if (__dx_vibe_check("__gm82dx8_buffer_to_surface",Device->CreateImageSurface(
        desc.Width,
        desc.Height,
        desc.Format,
        &scratch
    ))) return -1;    

    //"lock" full surface rectangle and get information about it
    D3DLOCKED_RECT pLockedRect;
    if (__dx_vibe_check("__gm82dx8_buffer_to_surface",scratch->LockRect(&pLockedRect,NULL,0)))
        return -1;

    char* dest=(char*)pLockedRect.pBits;
    int surfpitch=pLockedRect.Pitch;
    
    int srcpos=0;
    int dstpos=0;
    int bufstride=gmw*4;
    for (int i=0;i<gmh;i+=1) {
        memcpy(&dest[dstpos],&src[srcpos],bufstride);
        srcpos+=bufstride;
        dstpos+=surfpitch;
    }
    
    //free the lock.
    scratch->UnlockRect();
    
    if (__dx_vibe_check("__gm82dx8_buffer_to_surface",Device->CopyRects(scratch,NULL,0,surf,NULL)))
        return -1;
    
    scratch->Release();
    surf->Release();
    
    return 0;    
}
GMREAL __gm82dx8_surface_to_buffer(double buffer, double id, double gm_width, double gm_height) {    
    //parse arguments    
    char* dest=(char*)(int)buffer;
    int gmw=(int)gm_width;
    int gmh=(int)gm_height;
    
    //get current render target
    //(surface by id is confusing)
    IDirect3DSurface8* surf;
    //surf=get_gm_surface_depthbuffer(id);
    if (__dx_vibe_check("__gm82dx8_surface_to_buffer",Device->GetRenderTarget(&surf))) return -1;  

    //query for actual width and height, and get gm w/h
    D3DSURFACE_DESC desc;
    if (__dx_vibe_check("__gm82dx8_surface_to_buffer",surf->GetDesc(&desc))) return -1;
    int width=desc.Width;
    int height=desc.Height; 

    //create scratch surface
    IDirect3DSurface8* scratch;    
    if (__dx_vibe_check("__gm82dx8_surface_to_buffer",Device->CreateImageSurface(
        desc.Width,
        desc.Height,
        desc.Format,
        &scratch
    ))) return -1;

    if (__dx_vibe_check("__gm82dx8_surface_to_buffer",Device->CopyRects(surf,NULL,0,scratch,NULL)))
        return -1;

    //"lock" full surface rectangle and get information about it
    D3DLOCKED_RECT pLockedRect;
    if (__dx_vibe_check("__gm82dx8_surface_to_buffer",scratch->LockRect(&pLockedRect,NULL,D3DLOCK_NO_DIRTY_UPDATE|D3DLOCK_NOSYSLOCK|D3DLOCK_READONLY)))
        return -1;

    char* src=(char*)pLockedRect.pBits;
    int surfpitch=pLockedRect.Pitch;
    
    int srcpos=0;
    int dstpos=0;
    int bufstride=gmw*4;
    for (int i=0;i<gmh;i+=1) {
        memcpy(&dest[dstpos],&src[srcpos], bufstride);
        srcpos+=surfpitch;
        dstpos+=bufstride;
    }
    
    //free the lock and scratch surface.
    scratch->UnlockRect();
    scratch->Release();
    surf->Release();
    
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
GMREAL __gm82dx8_setalphabuffer(double enable) {
    if (enable>=0.5)
        *dx8_backbuffer_format = D3DFMT_A8R8G8B8; //backbuffer format: 32bit ARGB w/ alpha
    else
        *dx8_backbuffer_format = D3DFMT_X8R8G8B8; //backbuffer format: 32bit opaque
    
    runner_display_reset();

    return 0;
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
