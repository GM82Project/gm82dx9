#define __gm82dx9_init
    if (__gm82dx9_dllcheck()!=820) {
        show_error('GM8.2 DirectX9 Extension failed to link DLL.',1)
        exit
    }
    
    if (__gm82dx9_checkstart()) exit
    
    globalvar __gm82dx9_cross_detect;
    
    globalvar __gm82dx9_controller;     __gm82dx9_controller=__gm82dx9_obj
    globalvar __gm82dx9_appsurfcompose; __gm82dx9_appsurfcompose=noone

    globalvar __gm82dx9_resw,__gm82dx9_resh;
    globalvar __gm82dx9_default_vs,__gm82dx9_default_ps;
    
    globalvar d3d_transform_vertex;
    
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
    //load default shaders
    __gm82dx9_default_vs=shader_vertex_create_file(temp_directory+"\gm82\vs_pass.vs3")
    __gm82dx9_default_ps=shader_pixel_create_file(temp_directory+"\gm82\ps_pass.ps3")   


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

#define shader_vertex_create_buffer
    ///shader_vertex_create_buffer(buffer)
    if (!variable_global_exists("__gm82net_cross_detect")) {
        show_error("Add the Game Maker 8.2 Network extension to your project in order to use shader_vertex_create_buffer().",0)
        return 0
    }
    
    return execute_string("return __gm82dx9_shader_vertex_create_buffer(buffer_get_address("+string(buf)+",0))")    


#define shader_pixel_create_buffer
    ///shader_pixel_create_buffer(buffer)
    if (!variable_global_exists("__gm82net_cross_detect")) {
        show_error("Add the Game Maker 8.2 Network extension to your project in order to use shader_pixel_create_buffer().",0)
        return 0
    }
    
    return execute_string("return __gm82dx9_shader_pixel_create_buffer(buffer_get_address("+string(buf)+",0))")    


#define shader_vertex_set_passthrough
    ///shader_vertex_set_passthrough()
    shader_vertex_set(__gm82dx9_default_vs)
    shader_vertex_matrix_wvp(0)
    

#define shader_pixel_set_passthrough
    ///shader_pixel_set_passthrough()
    shader_pixel_set(__gm82dx9_default_ps)


#define shader_vertex_uniform_f
    ///shader_vertex_uniform_f(reg,...)
    var __i; __i=0
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_vertex_uniform_4f(argument[0]+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_vertex_uniform_4f(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_vertex_uniform_4f(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_vertex_uniform_4f(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_pixel_uniform_f
    ///shader_pixel_uniform_f(reg,...)
    var __i; __i=0
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_pixel_uniform_4f(argument[0]+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_pixel_uniform_4f(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_pixel_uniform_4f(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_pixel_uniform_4f(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_vertex_uniform_i
    ///shader_vertex_uniform_i(reg,...)
    var __i; __i=0
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_vertex_uniform_4i(argument[0]+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_vertex_uniform_4i(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_vertex_uniform_4i(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_vertex_uniform_4i(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_pixel_uniform_i
    ///shader_pixel_uniform_i(reg,...)
    var __i; __i=0
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_pixel_uniform_4i(argument[0]+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_pixel_uniform_4i(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_pixel_uniform_4i(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_pixel_uniform_4i(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_vertex_uniform_b
    ///shader_vertex_uniform_b(reg,...)
    var __i; __i=0
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_vertex_uniform_4b(argument[0]+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_vertex_uniform_4b(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_vertex_uniform_4b(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_vertex_uniform_4b(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_pixel_uniform_b
    ///shader_pixel_uniform_b(reg,...)
    var __i; __i=0
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_pixel_uniform_4b(argument[0]+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_pixel_uniform_4b(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_pixel_uniform_4b(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_pixel_uniform_4b(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)
//
//