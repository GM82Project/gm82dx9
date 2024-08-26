#define __gm82dx9_init
    if (__gm82dx9_dllcheck()!=820) {
        show_error('GM8.2 DirectX9 Extension failed to link DLL. Make sure you have the latest DirectX runtimes installed: https://www.microsoft.com/en-us/download/details.aspx?id=35',1)
        exit
    }

    globalvar __gm82dx9_cross_detect;
    
    globalvar __gm82dx9_controller;
    globalvar __gm82dx9_appsurfcompose; __gm82dx9_appsurfcompose=noone

    globalvar __gm82dx9_resw,__gm82dx9_resh;
    globalvar __gm82dx9_default_vs,__gm82dx9_default_ps;
    
    globalvar __gm82dx9_surface_was_new;
    
    globalvar d3d_transform_vertex,d3d_project_vertex,d3d_get_projection_origin;

    //load default shaders
    __gm82dx9_default_vs=shader_vertex_create_file(temp_directory+"\gm82\vs_pass.vs3")
    __gm82dx9_default_ps=shader_pixel_create_file(temp_directory+"\gm82\ps_pass.ps3")

    __gm82dx9_controller=gm82core_object

    if (__gm82dx9_checkstart()) exit
    
    var __i; __i=0 repeat (8) {d3d_light_define_direction(__i,0,0,1,0) __i+=1}
    
    //set target to appsurf at end step, to catch view setup and all draw events
    object_event_add(__gm82dx9_controller,ev_step,ev_step_end,"__gm82dx9_prepare()")
    //finally after all draw events, compose the window
    object_event_add(__gm82dx9_controller,ev_other,ev_animation_end,"__gm82dx9_compose()")  
    //ignore first room frame
    object_event_add(__gm82dx9_controller,ev_other,ev_room_start,"
    if (variable_global_exists('__gm82dx8_controller')) {
        if (variable_global_get('__gm82dx8_controller')!=0) {
            __gm82dx9_abort()
        }
    }
    if (__gm82dx9_appsurfcompose!=noone) {set_automatic_draw(false) alarm[0]=1}")
    object_event_add(__gm82dx9_controller,ev_alarm,0,"
        if (__gm82dx9_appsurfcompose!=noone) set_automatic_draw(true)
    ")


#define __gm82dx9_quit
    shader_vertex_destroy(__gm82dx9_default_vs)
    shader_pixel_destroy(__gm82dx9_default_vs)


#define d3d_set_alphablend
    ///d3d_set_alphablend(enable)
    YoYo_EnableAlphaBlend(argument0)


#define draw_make_opaque
    ///draw_make_opaque()
    var __a;
    __a=draw_get_alpha()
    d3d_set_color_mask(0,0,0,1)
    draw_rectangle(-9999999,-9999999,9999999,9999999,0)
    d3d_set_color_mask(1,1,1,1)
    draw_set_alpha(__a)


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
    //x,y,z: input coordinates in 3d space
    //returns: array[3] with transformed world coordinates
    //This function will take the 3d object-space coordinates and apply the
    //current world-matrix to it.
    
    d3d_transform_vertex[0]=__gm82dx9_transformvertex(argument0,argument1,argument2)
    d3d_transform_vertex[1]=__gm82dx9_getvertexy()
    d3d_transform_vertex[2]=__gm82dx9_getvertexz()
    

#define d3d_get_projection_origin
    ///d3d_get_projection_origin()
    //returns: array[3] with projection's 'from' coordinates
    
    d3d_get_projection_origin[0]=__gm82dx9_geteyepos()
    d3d_get_projection_origin[1]=__gm82dx9_getvertexy()
    d3d_get_projection_origin[2]=__gm82dx9_getvertexz()


#define d3d_project_vertex
    ///d3d_project_vertex(x,y,z)
    //x,y,z: input coordinates in 3d space
    //returns: array[2] with screen-space coordinates
    //This function will take the 3d object-space coordinates and apply the
    //current view-proj-matrix to it, returning a 2d coordinate.
    
    d3d_project_vertex[0]=__gm82dx9_projectvertex(argument0,argument1,argument2)
    d3d_project_vertex[1]=__gm82dx9_getvertexy()


#define d3d_fog_trick
    ///d3d_fog_trick([color,amount])
    ///d3d_fog_trick()
    
    if (argument_count==1) {
        d3d_set_fog(1,argument0,0,0)
    } else if (argument_count==2) {
        d3d_set_fog(1,argument0,0.5-argument1,1.5-argument1)
    } else d3d_set_fog(0,0,0,0)


#define buffer_get_lights
    ///buffer_get_lights(buffer)
    var __buf;
    
    __buf=argument0
    
    if (!buffer_exists(__buf)) {
        show_error("Trying to get lights data into a nonexisting buffer("+string(__buf)+").",0)
        return 0
    }
    
    buffer_set_size(__buf,104*8)
    buffer_set_pos(__buf,0)
    __gm82dx9_buffer_get_lights(buffer_get_address(__buf,0))


#define draw_set_blend_alphamode
    ///draw_set_blend_alphamode(enable,[mode or [src,dest]])
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


#define d3d_model_create_and_load
    ///d3d_model_create_and_load(filename)
    var __m;
    if (!string_pos("\\.\pipe\",argument0)) if (!file_exists(argument0)) return -1
    __m=d3d_model_create()
    if (filename_ext(argument0)==".g3z") d3d_model_load_g3z(__m,argument0)
    else d3d_model_load(__m,argument0)
    return __m


#define d3d_model_bake
    ///d3d_model_bake(model)
    d3d_model_draw(argument0,0,0,0,-1)


#define d3d_set_viewport
    //d3d_set_viewport(x,y,width,height)
    __gm82dx9_setviewport(argument0,argument1,argument2,argument3)
    d3d_set_projection_ortho(0,0,argument2,argument3,0)


#define window_resize_buffer
    ///window_resize_buffer(w,h,reset,aa)
    //w,h: window size
    //reset: whether to reset the device
    //aa: antialias value for device reset
    //Call this with reset true at least once in game start.
    
    __gm82dx9_resize_backbuffer(argument0,argument1)
    if (argument2) display_reset(argument3)
//
//