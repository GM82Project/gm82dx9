#include "gm82dx9.h"

int has_started = 0;

D3DVIEWPORT9 viewport;
D3DRASTER_STATUS raster_status;
XMMATRIX world_matrix;
XMVECTOR vertex;

create_c_function(void,runner_display_reset,0x61f9f4);
create_c_function(void,runner_clear_depth,0x563a8c);

//-//

GMREAL __gm82dx9_dllcheck() {
    return 820;
}
GMREAL __gm82dx9_checkstart() {
    if (has_started) return 1;
    has_started = 1;
    return 0;
}
GMREAL __gm82dx9_abort() {
    exit(1);
}
GMREAL __gm82dx9_testfunc() {
    return 0;
}

GMREAL argb_get_color(double color) {
    return ((unsigned int)color) & 0x00ffffff;
}
GMREAL argb_get_alpha(double color) {
    return ((double)((((unsigned int)color) & 0xff000000) >> 24)) / 0xff;
}
GMREAL argb_make_color(double color, double alpha) {
    return (double)((((int)(alpha*0xff))<<24)|((unsigned int)color));
}

GMREAL __gm82dx9_set_color_mask(double red, double green, double blue, double alpha) {
    UINT mask = 0;
    if (alpha>=0.5) mask += D3DCOLORWRITEENABLE_ALPHA;
    if (red>=0.5) mask += D3DCOLORWRITEENABLE_RED;
    if (green>=0.5) mask += D3DCOLORWRITEENABLE_GREEN;
    if (blue>=0.5) mask += D3DCOLORWRITEENABLE_BLUE;
    Device->SetRenderState(D3DRS_COLORWRITEENABLE,*(DWORD *)&mask);
    return 0;
}
GMREAL __gm82dx9_set_fill_mode(double mode) {
    DWORD newmode=D3DFILL_POINT;
    if (mode>=0.5) newmode=D3DFILL_WIREFRAME;
    if (mode>=1.5) newmode=D3DFILL_SOLID;
    Device->SetRenderState(D3DRS_FILLMODE,newmode);    
    return 0;
}
GMREAL __gm82dx9_set_cull_mode(double mode) {
    DWORD newmode=D3DCULL_NONE;
    if (mode>=0.5) newmode=D3DCULL_CW;
    if (mode>=1.5) newmode=D3DCULL_CCW;
    Device->SetRenderState(D3DRS_CULLMODE,newmode);    
    return 0;
}
GMREAL __gm82dx9_set_zcomp_mode(double cmpfunc) {
    Device->SetRenderState(D3DRS_ZFUNC,(DWORD)(int)cmpfunc);    
    return 0;
}
GMREAL __gm82dx9_set_zbias(double bias) {
    float newbias=(float)bias;
    Device->SetRenderState(D3DRS_DEPTHBIAS,*(DWORD *)(&newbias));    
    return 0;
}
GMREAL __gm82dx9_set_alphatest(double enable,double value,double cmpfunc) {
    Device->SetRenderState(D3DRS_ALPHATESTENABLE,(DWORD)(enable>0.5));    
    Device->SetRenderState(D3DRS_ALPHAREF,(DWORD)(int)value);  
    Device->SetRenderState(D3DRS_ALPHAFUNC,(DWORD)(int)cmpfunc);      
    return 0;
}
GMREAL __gm82dx9_set_light(
    double index, double type,
    double x, double y, double z,
    double dx, double dy, double dz,
    double range, double inner_rad, double outer_rad,
    double color, double color_factor,
    double specular, double specular_factor
) {
    if (range<=0 && (int)type!=D3DLIGHT_DIRECTIONAL) return 0;
    
    D3DLIGHT9 light;
    
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
    D3DMATERIAL9 mat;
    
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
GMREAL __gm82dx9_setrangefog(double type,double color,double start,double end) {
    
    Device->SetRenderState(D3DRS_FOGENABLE,(DWORD)type);
    
    if (type>0) {
        Device->SetRenderState(D3DRS_FOGCOLOR,gm_col_to_dx9(color));
        
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
GMREAL __gm82dx9_surface_set_depth(double id) {
	if (vibe_check(Device->SetDepthStencilSurface(get_gm_surface(id)->zbuffer)))
        return -1;
	return 0;
}
GMREAL __gm82dx9_buffer_to_surface(double buffer, double id, double gm_width, double gm_height) {    
    //parse arguments    
    char* src=(char*)(int)buffer;
    int gmw=(int)gm_width;
    int gmh=(int)gm_height;

    // get surface
    GMSurface* gm_surf = get_gm_surface(id);
    IDirect3DTexture9* tex = get_gm_texture(gm_surf->texture)->texture;
    IDirect3DSurface9* surf = nullptr;
    if (vibe_check(tex->GetSurfaceLevel(0, &surf))) return -1;

    // make rectangle
    RECT rect = {
            .left = 0, .top = 0, .right = gmw, .bottom = gmh
    };

    // load it up
    if (vibe_check(D3DXLoadSurfaceFromMemory(
            surf, nullptr, &rect,
            src, D3DFMT_A8R8G8B8, gmw * 4, nullptr, &rect,
            D3DX_FILTER_NONE, 0
    ))) return -1;

    tex->AddDirtyRect(&rect);
    
    return 0;    
}
GMREAL __gm82dx9_surface_to_buffer(double buffer, double id, double gm_width, double gm_height) {    
    //parse arguments    
    char* dest=(char*)(int)buffer;
    int gmw=(int)gm_width;
    int gmh=(int)gm_height;

    // get surface
    GMSurface* gm_surf = get_gm_surface(id);
    IDirect3DSurface9* surf = nullptr;
    if (vibe_check(get_gm_texture(gm_surf->texture)->texture->GetSurfaceLevel(0, &surf))) return -1;

    //create scratch surface
    IDirect3DSurface9* scratch;    
    if (vibe_check(Device->CreateOffscreenPlainSurface(
        gmw,
        gmh,
        D3DFMT_A8R8G8B8,
		D3DPOOL_SCRATCH,
        &scratch,
		nullptr
    ))) return -1;

    RECT rect = {
            .left = 0, .top = 0, .right = gmw, .bottom = gmh
    };

	if (vibe_check(D3DXLoadSurfaceFromSurface(
		scratch,nullptr,&rect,surf,nullptr,&rect,D3DX_FILTER_NONE,0
	)))
		return -1;

    //"lock" full surface rectangle and get information about it
    D3DLOCKED_RECT pLockedRect;
    if (vibe_check(scratch->LockRect(&pLockedRect,&rect,D3DLOCK_NO_DIRTY_UPDATE|D3DLOCK_NOSYSLOCK|D3DLOCK_READONLY)))
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
GMREAL __gm82dx9_cleardepth() {
    runner_clear_depth();
    return 1;
}
GMREAL __gm82dx9_setfullscreen(double hz) {
    int z = (int)hz;
    
    d3d_parameters.Windowed = !z; //windowed mode
    d3d_parameters.FullScreen_RefreshRateInHz = z; //refresh rate
    
    runner_display_reset();

    return 1;
}
GMREAL __gm82dx9_setalphabuffer(double enable) {
    if (enable>=0.5)
        *dx9_backbuffer_format = D3DFMT_A8R8G8B8; //backbuffer format: 32bit ARGB w/ alpha
    else
        *dx9_backbuffer_format = D3DFMT_X8R8G8B8; //backbuffer format: 32bit opaque
    
    runner_display_reset();

    return 0;
}
GMREAL __gm82dx9_resize_backbuffer(double width, double height) {
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
GMREAL __gm82dx9_setpointscale(double size,double scaling,double minscale,double maxscale,double sprite) {
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
GMREAL __gm82dx9_setsepalphablend(double enabled, double src, double dest) {
    Device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE,(DWORD)enabled>0.5);
    Device->SetRenderState(D3DRS_SRCBLENDALPHA,(DWORD)src);
    Device->SetRenderState(D3DRS_DESTBLENDALPHA,(DWORD)dest);
    
    return 0;
}
GMREAL __gm82dx9_setviewport(double x, double y, double width, double height) {
    Device->GetViewport(&viewport);
    viewport.X=(DWORD)x;
    viewport.Y=(DWORD)y;
    viewport.Width=(DWORD)width;
    viewport.Height=(DWORD)height;
    
    Device->SetViewport(&viewport);
    
    return 0;
}
GMREAL __gm82dx9_getviewportx() {
    Device->GetViewport(&viewport);
    return (double)viewport.X;
}
GMREAL __gm82dx9_getviewporty() {
    Device->GetViewport(&viewport);
    return (double)viewport.Y;
}
GMREAL __gm82dx9_getviewportw() {
    Device->GetViewport(&viewport);
    return (double)viewport.Width;
}
GMREAL __gm82dx9_getviewporth() {
    Device->GetViewport(&viewport);
    return (double)viewport.Height;
}
GMREAL __gm82dx9_setzscale(double znear, double zfar) {
    Device->GetViewport(&viewport);
    viewport.MinZ=(float)znear;
    viewport.MaxZ=(float)zfar;
    Device->SetViewport(&viewport);
    
    return 0;
}
GMREAL __gm82dx9_getvideomem() {
    return (double)(Device->GetAvailableTextureMem()/1048576);
}
GMREAL __gm82dx9_getmaxwidth() {
    return (double)d3d_caps.MaxTextureWidth;
}
GMREAL __gm82dx9_getmaxheight() {
    return (double)d3d_caps.MaxTextureHeight;
}
GMREAL __gm82dx9_transformvertex(double inx, double iny, double inz) {
	XMVECTOR in_vec = XMVectorSet(inx, iny, inz, 0.0);
    Device->GetTransform(D3DTS_WORLDMATRIX(0),reinterpret_cast<D3DMATRIX*>(&world_matrix));
    vertex = XMVector3TransformCoord(in_vec,world_matrix);
    return (double)XMVectorGetX(vertex);
}
GMREAL __gm82dx9_getvertexy() {
    return (double)XMVectorGetY(vertex);
}
GMREAL __gm82dx9_getvertexz() {
    return (double)XMVectorGetZ(vertex);
}
