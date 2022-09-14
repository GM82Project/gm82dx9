
      Game Maker 8.2 DirectX8
      ===================
      version 1.2
  
--------------------------------------------------------------------------------

    This extension aims to extend your control over the DirectX8 interface
    Game Maker 8.2 uses for rendering.

    Contains contributions by Floogle and Lovey01.


-> What this enables

    - vpatch for stable framerates when room speed % monitor Hz
    - resize the damn backbuffer so gm8 can have proper resizing and fullscreen
    - experimental exclusive fullscreen support
    - control rendering properties such as point mode, fill mode, point sprites
    - toggle alpha blend, color write channels, clear the depth buffer
    - set the screen clipping region (scissor test) plus convenience functions


--------------------------------------------------------------------------------

    Quick function reference
    ------------------------    
    
application_surface_disable()
application_surface_enable(postdraw script id)
application_surface_get_height()
application_surface_get_width()
application_surface_is_enabled()
application_surface_resize(width,height)
dx8_clear_depth_buffer()
dx8_get_free_video_memory()
dx8_get_viewport_h()
dx8_get_viewport_w()
dx8_get_viewport_x()
dx8_get_viewport_y()
dx8_make_opaque()
dx8_projection_simple(x,y,w,h,angle,dollyzoom,depthmin,depthfocus,depthmax,spin)
dx8_reset_projection()
dx8_resize_buffer(w,h)
dx8_set_alphablend(enable)
dx8_set_clip_region(x,y,width,height)
dx8_set_color_mask(red,green,blue,alpha)
dx8_set_cull_mode(mode)
dx8_set_depth_scale(near,far)
dx8_set_fill_mode(mode)
dx8_set_fullscreen_ext(enable)
dx8_set_point_options(size,scaled,minscale,maxscale,spritemode)
dx8_set_vsync(enable)
dx8_set_zbias(bias)
dx8_surface_discard(id)
dx8_surface_disengage()
dx8_surface_engage(id,width,height)
dx8_surface_ensure(id,width,height)
dx8_texture_get_max_height()
dx8_texture_get_max_width()
dx8_transform_vertex(x,y,z)
dx8_transform_xst(rx,sx,sy,sz,x,y,z)
dx8_transform_xt(rx,x,y,z)
dx8_transform_xyst(rx,ry,sx,sy,sz,x,y,z)
dx8_transform_xyt(rx,ry,x,y,z)
dx8_transform_xyzst(rx,ry,rz,sx,sy,sz,x,y,z)
dx8_transform_xyzt(rx,ry,rz,x,y,z)
dx8_transform_xzst(rx,rz,sx,sy,sz,x,y,z)
dx8_transform_xzt(rx,rz,x,y,z)
dx8_transform_xzyst(rx,rz,ry,sx,sy,sz,x,y,z)
dx8_transform_xzyt(rx,rz,ry,x,y,z)
dx8_transform_yst(ry,sx,sy,sz,x,y,z)
dx8_transform_yt(ry,x,y,z)
dx8_transform_yxst(ry,rx,sx,sy,sz,x,y,z)
dx8_transform_yxt(ry,rx,x,y,z)
dx8_transform_yxzst(ry,rx,rz,sx,sy,sz,x,y,z)
dx8_transform_yxzt(ry,rx,rz,x,y,z)
dx8_transform_yzst(ry,rz,sx,sy,sz,x,y,z)
dx8_transform_yzt(ry,rz,x,y,z)
dx8_transform_yzxst(ry,rz,rx,sx,sy,sz,x,y,z)
dx8_transform_yzxt(ry,rz,rx,x,y,z)
dx8_transform_zst(rz,sx,sy,sz,x,y,z)
dx8_transform_zt(rz,x,y,z)
dx8_transform_zxst(rz,rx,sx,sy,sz,x,y,z)
dx8_transform_zxt(rz,rx,x,y,z)
dx8_transform_zxyst(rz,rx,ry,sx,sy,sz,x,y,z)
dx8_transform_zxyt(rz,rx,ry,x,y,z)
dx8_transform_zyst(rz,ry,sx,sy,sz,x,y,z)
dx8_transform_zyt(rz,ry,x,y,z)
dx8_transform_zyxst(rz,ry,rx,sx,sy,sz,x,y,z)
dx8_transform_zyxt(rz,ry,rx,x,y,z)
fog_trick([color,amount])
  
--------------------------------------------------------------------------------