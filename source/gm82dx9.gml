#define __gm82dx9_init
    if (__gm82dx9_dllcheck()!=820) {
        show_error('GM8.2 DirectX9 Extension failed to link DLL. Make sure you have the latest DirectX runtimes installed: https://www.microsoft.com/en-us/download/details.aspx?id=35',1)
        exit
    }

    if (variable_global_exists("__gm82dx8_controller")) {
        if (variable_global_get("__gm82dx8_controller")!=0) {
            show_error("The GM82 DX8 and DX9 extensions cannot be combined.",true)
            __gm82dx9_abort()
        }
    }
    
    globalvar __gm82dx9_cross_detect;
    
    globalvar __gm82dx9_controller;     __gm82dx9_controller=__gm82dx9_obj
    globalvar __gm82dx9_appsurfcompose; __gm82dx9_appsurfcompose=noone

    globalvar __gm82dx9_resw,__gm82dx9_resh;
    globalvar __gm82dx9_default_vs,__gm82dx9_default_ps;
    
    globalvar __gm82dx9_surface_was_new;
    
    globalvar d3d_transform_vertex;

    //load default shaders
    __gm82dx9_default_vs=shader_vertex_create_file(temp_directory+"\gm82\vs_pass.vs3")
    __gm82dx9_default_ps=shader_pixel_create_file(temp_directory+"\gm82\ps_pass.ps3")

    if (__gm82dx9_checkstart()) exit
    
    if (variable_global_get("__gm82core_version")>134) {
        //recent enough core extension: we can work together
        __gm82dx9_controller=gm82core_object
    } else {
        //core extension not detected: let's do it ourselves
        object_event_add(__gm82dx9_controller,ev_destroy,0,"instance_copy(0)")
        object_event_add(__gm82dx9_controller,ev_other,ev_room_end,"persistent=true")
        object_set_persistent(__gm82dx9_controller,1)
        room_instance_add(room_first,0,0,__gm82dx9_controller)
    }
    //set target to appsurf at end step, to catch view setup and all draw events
    object_event_add(__gm82dx9_controller,ev_step,ev_step_end,"__gm82dx9_prepare()")
    //finally after all draw events, compose the window
    object_event_add(__gm82dx9_controller,ev_other,ev_animation_end,"__gm82dx9_compose()")  
    //ignore first room frame
    object_event_add(__gm82dx9_controller,ev_other,ev_room_start,"if (__gm82dx9_appsurfcompose!=noone) {set_automatic_draw(false) alarm[0]=1}")
    object_event_add(__gm82dx9_controller,ev_alarm,0,"if (__gm82dx9_appsurfcompose!=noone) set_automatic_draw(true)")


#define __gm82dx9_quit
    shader_vertex_destroy(__gm82dx9_default_vs)
    shader_pixel_destroy(__gm82dx9_default_vs)


#define d3d_set_alphablend
    ///d3d_set_alphablend(enable)
    YoYo_EnableAlphaBlend(argument0)


#define draw_make_opaque
    ///draw_make_opaque()
    draw_set_blend_mode(bm_add)
    draw_rectangle_color(-9999999,-9999999,9999999,9999999,0,0,0,0,0)
    draw_set_blend_mode(0)


#define window_set_fullscreen_ext
    ///window_set_fullscreen_ext(enabled)
    //exclusive fullscreen
    //warning: VERY buggy    
    if (argument0 ^ window_get_fullscreen()) {
        if (window_get_fullscreen()) {
            __gm82dx9_setfullscreen(0)
            window_set_fullscreen(0)
        } else {
            window_set_fullscreen(1)
            __gm82dx9_setfullscreen(display_get_frequency())
        }
    }


#define d3d_set_projection_default
    ///d3d_set_projection_default()
    if (view_enabled)
        d3d_set_projection_ortho(view_xview[view_current],view_yview[view_current],view_wview[view_current],view_hview[view_current],view_angle[view_current])
    else
        d3d_set_projection_ortho(0,0,room_width,room_height,0)


#define d3d_set_projection_simple
    ///d3d_projection_simple(x,y,w,h,tilt,dollyzoom,depthmin,depthfocus,depthmax,spin)
    var __xfrom,__yfrom,__zfrom,__spin;
    
    __spin=argument9-90

    if (argument5<=0) {
        // ¯\_(º_o)_/¯
        d3d_set_projection_ortho(argument0,argument1,argument2,argument3,argument4)
        __xfrom=argument0+argument2/2
        __yfrom=argument1+argument3/2    
        
        d3d_set_projection(
            __xfrom-lengthdir_x(1,__spin),__yfrom,-lengthdir_y(1,__spin),
            __xfrom,__yfrom,0,
            -lengthdir_x(1,-argument4+90),-lengthdir_y(1,-argument4+90),0
        )
    } else {
        __xfrom=argument0+argument2/2
        __yfrom=argument1+argument3/2    
        __zfrom=min(-tan(degtorad(90*(1-argument5)))*argument3/2,argument6-argument7)

        d3d_set_projection_ext(
            __xfrom+lengthdir_x(__zfrom,__spin),                      //xfrom
            __yfrom,                                                     //yfrom
            argument7+lengthdir_y(__zfrom,__spin),                    //zfrom
            __xfrom,__yfrom,argument7,                                   //to
            lengthdir_x(1,-argument4+90),lengthdir_y(1,-argument4+90),0, //up
            -point_direction(__zfrom,0,0,argument3/2)*2,                 //angle
            -argument2/argument3,                                        //aspect
            max(1,argument6-argument7-__zfrom),                          //znear
            argument8-argument7-__zfrom                                  //zfar
        )
    }
    

#define d3d_transform_vertex
    ///d3d_transform_vertex(x,y,z)
    d3d_transform_vertex[0]=__gm82dx9_transformvertex(argument0,argument1,argument2)
    d3d_transform_vertex[1]=__gm82dx9_getvertexy()
    d3d_transform_vertex[2]=__gm82dx9_getvertexz()


#define d3d_fog_trick
    ///d3d_fog_trick(color,amount)
    ///d3d_fog_trick()

    if (argument_count==2) {
        d3d_set_fog(1,argument[0],0.5-argument[1],1.5-argument[1])
    } else d3d_set_fog(0,0,0,0)


#define draw_set_blend_alphamode
    ///draw_set_blend_alphamode(enable,mode or [src,dest])
    if (argument_count<2) {
        __gm82dx9_setsepalphablend(0,0,0)
    }
    if (argument_count==2) {
        src=bm_src_alpha
        dst=bm_inv_src_alpha
        if (argument[1]==bm_add) {
            dst=bm_one
        }
        if (argument[1]==bm_subtract) {
            dst=bm_inv_src_color
        }
        if (argument[1]==bm_max) {
            src=bm_zero
            dst=bm_inv_src_color
        }
        
        __gm82dx9_setsepalphablend(1,src,dst)        
    }
    if (argument_count==3) {
        __gm82dx9_setsepalphablend(1,argument[1],argument[2])        
    }


#define d3d_model_save_g3z
    ///d3d_model_save_g3z(model,filename)
    var __b;
    d3d_model_save(argument0,argument1)
    __b=buffer_create()
    buffer_load(__b,argument1)
    buffer_deflate(__b)
    buffer_save(__b,argument1)
    buffer_destroy(__b)
    return 0


#define d3d_model_load_g3z
    ///d3d_model_load_g3z(model,filename)
    var __b,__m;
    __b=buffer_create()
    buffer_load(__b,argument1)
    buffer_inflate(__b)
    if (argument0<0) __m=d3d_model_create() else __m=argument0
    d3d_model_load(__m,buffer_make_pipe(__b))
    buffer_destroy(__b)
    return __m
//
//