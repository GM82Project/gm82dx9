#include "gm82dx9.h"

int has_started = 0;

D3DPRESENT_PARAMETERS* present_params;

DXData dx_data;

D3DVIEWPORT9 viewport;
D3DRASTER_STATUS raster_status;
XMMATRIX world_matrix;
XMMATRIX view_matrix;
XMMATRIX proj_matrix;
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
    MessageBox(NULL,"The GM82 DX8 and DX9 extensions\ncannot be combined.",NULL,MB_ICONSTOP);
    exit(1);
}
GMREAL __gm82dx9_testfunc() {
    return 0;
}

GMREAL argb_get_color(double color) {
    ///argb_get_color(color)
    //color: u32 returned by buffer_get_surface().
    //returns: The color value out of a u32.
    
    return ((unsigned int)color) & 0x00ffffff;
}
GMREAL argb_get_alpha(double color) {
    ///argb_get_alpha(color)
    //color: u32 returned by buffer_get_surface().
    //returns: The alpha value out of a u32.
    
    return ((double)((((unsigned int)color) & 0xff000000) >> 24)) / 0xff;
}
GMREAL argb_make_color(double color, double alpha) {
    ///argb_make_color(color,alpha)
    //color: gm color value.
    //alpha: gm alpha value (0-1).
    //returns: Color converted into a u32 the way surface data expects it. Useful for writing buffers for use with buffer_set_surface().
    
    return (double)((((int)(alpha*0xff))<<24)|((unsigned int)color));
}

GMREAL color_to_d3dcolor(double color, double alpha) {
    ///color_to_d3dcolor(color,alpha)
    //color: gm color value.
    //alpha: gm alpha value (0-1).
    //returns: Color converted into a u32 formatted the way DirectX expects it. Useful for writing vertex buffers.
    
    int col=(unsigned int)color;
    return (double)(((col & 0xff)<<16)+(col & 0xff00)+((col & 0xff0000)>>16)+(((unsigned int)(alpha*0xff))<<24));
}

GMREAL __gm82dx9_set_color_mask(double red, double green, double blue, double alpha) {
    ///d3d_set_color_mask(red,green,blue,alpha)
    //red,green,blue,alpha: true/false
    //Enables or disables writing to each color channel when drawing.
    
    UINT mask = 0;
    if (alpha>=0.5) mask += D3DCOLORWRITEENABLE_ALPHA;
    if (red>=0.5) mask += D3DCOLORWRITEENABLE_RED;
    if (green>=0.5) mask += D3DCOLORWRITEENABLE_GREEN;
    if (blue>=0.5) mask += D3DCOLORWRITEENABLE_BLUE;
    Device->SetRenderState(D3DRS_COLORWRITEENABLE,*(DWORD *)&mask);
    return 0;
}
GMREAL __gm82dx9_set_fill_mode(double mode) {
    ///d3d_set_fill_mode(mode)
    //mode: fill_ constant
    //Sets the polygon fill mode. Valid modes include:
    //fill_normal: Draw polygons normally.
    //fill_points: Draw only the vertices as points.
    //fill_wireframe: Draw edges as lines.
    
    DWORD newmode=D3DFILL_POINT;
    if (mode>=0.5) newmode=D3DFILL_WIREFRAME;
    if (mode>=1.5) newmode=D3DFILL_SOLID;
    Device->SetRenderState(D3DRS_FILLMODE,newmode);    
    return 0;
}
GMREAL __gm82dx9_set_cull_mode(double mode) {
    ///d3d_set_cull_mode(mode)
    //Sets the backface culling method. Valid methods include:
    //cull_clockwise: Draw faces with clockwise pixel definition.
    //cull_counterclockwise: Draw faces with counterclockwise vertex definition.
    //cull_none: Draw both faces.
    
    DWORD newmode=D3DCULL_NONE;
    if (mode>=0.5) newmode=D3DCULL_CW;
    if (mode>=1.5) newmode=D3DCULL_CCW;
    Device->SetRenderState(D3DRS_CULLMODE,newmode);    
    return 0;
}
GMREAL __gm82dx9_set_zcomp_mode(double cmpfunc) {
    ///d3d_set_zcomp_mode(cm_comparison)
    //Sets the z comparison mode when drawing pixels. Valid comparison modes include:
    //cm_less: Draws pixels if the z value is less than the zbuffer value.
    //cm_equal: Draws pixels if the z value is equal to the zbuffer value.
    //cm_lessequal: Draws pixels if the z value is less or equal to the zbuffer value. This is the default mode.
    //cm_greater: Draws pixels if the z value is greater than the zbuffer value.
    //cm_greaterequal: Draws pixels if the z value is greater or equal to the zbuffer value.
    //cm_notequal: Draws pixels if the z value is different to the zbuffer value.
    
    Device->SetRenderState(D3DRS_ZFUNC,(DWORD)(int)cmpfunc);    
    return 0;
}
GMREAL __gm82dx9_set_zbias(double bias) {
    ///d3d_set_zbias(bias)
    //Sets the zbias value used to sort polygons that are close to each other.
    //The zbias is added to fragment depth when computing overlap, to help polygons sort in front or behind of other polygons.
    //The unit for zbias is in frustum size, so if your znear is 1 and zfar is 1000, then to get a zbias of 1 pixel you would set it to 1/999.
    
    float newbias=(float)bias;
    Device->SetRenderState(D3DRS_DEPTHBIAS,*(DWORD *)(&newbias));    
    return 0;
}
GMREAL __gm82dx9_set_alphatest(double enable,double value,double cmpfunc) {
    ///d3d_set_alphatest(enable,value,cm_comparison)
    //Sets the alpha test reference value and comparison mode. Valid comparison modes include:
    //cm_less: Draws pixels if the alpha value is less than the ref value.
    //cm_equal: Draws pixels if the alpha value is equal to the ref value.
    //cm_lessequal: Draws pixels if the alpha value is less or equal to the ref value.
    //cm_greater: Draws pixels if the alpha value is greater than the ref value.
    //cm_greaterequal: Draws pixels if the alpha value is greater or equal to the ref value.
    //cm_notequal: Draws pixels if the alpha value is different to the ref value.
    
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
    ///d3d_light_define_ext(index,type,x,y,z,dx,dy,dz,range,inner_rad,outer_rad,color_diffuse,diffuse_factor,color_specular,specular_factor)
    //index: light number (use small integers)
    //type: one of lt_point, lt_spotlight, or lt_sun
    //x,y,z: light position (unused for sun lights)
    //dx,dy,dz: direction vector (unused for point lights)
    //range: maximum light range (unused for sun lights)
    //inner_rad: inner aperture angle of spotlights (unused for sun and point)
    //outer_rad: outer penumbra angle of spotlights (unused for sun and point)
    //color_diffuse: light color
    //diffuse_factor: light scaling factor, default 1. can be used for cool effects f.ex. larger values cause "cel shading".
    //color_specular: specular highlight color. Used for shiny reflections. Set to black to disable.
    //specular_factor: exponent factor for specular highlights. Use values between 10 and 50 depending on the intended effect.
    
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
    
    light.Diffuse.r=((col & 0xff)/255.0)*color_factor;
    light.Diffuse.g=(((col & 0xff00)>>8)/255.0)*color_factor;
    light.Diffuse.b=(((col & 0xff0000)>>16)/255.0)*color_factor;
    light.Diffuse.a=1.0f;
    
    col=(int)round(specular);
    
    light.Specular.r=(col & 0xff)/255.0;
    light.Specular.g=((col & 0xff00)>>8)/255.0;
    light.Specular.b=((col & 0xff0000)>>16)/255.0;
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
    ///d3d_set_fog_ext(type,color,start,end)
    //type: fog_ constants
    //color: gm color
    //start,end: fog distance
    //Sets the fog mode. Valid types include:
    //fog_none: Disable fog.
    //fog_table: Use table fog based on fragment w value. The default fog mode.
    //fog_vertex: Use spherical vertex distance-based fog. Looks better, but needs small polygons to work correctly.  
        
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
    ///surface_set_depth(id)
    //Sets the depth buffer to use a different surface.
    
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
    
    surf->Release();
    
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
    present_params->BackBufferWidth = (UINT)iwidth;
    present_params->BackBufferHeight = (UINT)iheight;
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
    Device->GetTransform(D3DTS_WORLD,reinterpret_cast<D3DMATRIX*>(&world_matrix));
    vertex = XMVector3TransformCoord(in_vec,world_matrix);
    return (double)XMVectorGetX(vertex);
}
GMREAL __gm82dx9_projectvertex(double inx, double iny, double inz) {
	XMVECTOR in_vec = XMVectorSet(inx, iny, inz, 0.0);
    Device->GetTransform(D3DTS_VIEW,reinterpret_cast<D3DMATRIX*>(&view_matrix));
    Device->GetTransform(D3DTS_PROJECTION,reinterpret_cast<D3DMATRIX*>(&proj_matrix));    
    XMMATRIX finalmat = XMMatrixMultiply(view_matrix,proj_matrix);    
    vertex = XMVector3TransformCoord(in_vec,finalmat);
    return (double)XMVectorGetX(vertex);
}
GMREAL __gm82dx9_getvertexy() {
    return (double)XMVectorGetY(vertex);
}
GMREAL __gm82dx9_getvertexz() {
    return (double)XMVectorGetZ(vertex);
}
