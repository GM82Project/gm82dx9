#define __gm82dx8_init
    if (__gm82dx8_dllcheck()!=820) {
        show_error('GM8.2 DirectX8 Extension failed to link DLL.',1)
        exit
    }
    
    if (__gm82dx8_checkstart()) exit
    
    globalvar __gm82dx8_cross_detect;
    
    globalvar __gm82dx8_time;           __gm82dx8_time=__gm82dx8_time_now()
    globalvar __gm82dx8_vsync_enabled;
    globalvar __gm82dx8_vpatched;       __gm82dx8_vpatched=false
    globalvar __gm82dx8_controller;     __gm82dx8_controller=__gm82dx8_obj
    globalvar __gm82dx8_appsurfcompose; __gm82dx8_appsurfcompose=noone
    globalvar __gm82dx8_isntxp;
    globalvar __gm82dx8_resw,__gm82dx8_resh;
    globalvar dx8_transform_vertex;
    
    //check for gm 8.1.141
    if (execute_string("return get_function_address('display_get_orientation')") <= 0) {
        if (variable_global_get("__gm82core_version")>134) {
            //recent enough core extension: we can work together
            __gm82dx8_controller=core
        } else {
            //core extension not detected: let's do it ourselves
            object_event_add(__gm82dx8_obj,ev_destroy,0,"instance_copy(0)")
            object_event_add(__gm82dx8_obj,ev_other,ev_room_end,"persistent=true")
            object_set_persistent(__gm82dx8_obj,1)
            room_instance_add(room_first,0,0,__gm82dx8_obj)
        }
        object_event_add(__gm82dx8_controller,ev_other,ev_game_start,"
            //set a variable to prevent conflicts with the gm82vpatch extension
            if (variable_global_exists('__gm82vpatch_time')) __gm82dx8_vpatched=true
        ")
        //set target to appsurf at end step, to catch view setup and all draw events
        object_event_add(__gm82dx8_controller,ev_step,ev_step_end,"__gm82dx8_prepare()")
        //finally after all draw events, compose the window
        object_event_add(__gm82dx8_controller,ev_other,ev_animation_end,"__gm82dx8_compose()")  
        //ignore first room frame
        object_event_add(__gm82dx8_controller,ev_other,ev_room_start,"if (__gm82dx8_appsurfcompose!=noone) {set_automatic_draw(false) alarm[0]=1}")
        object_event_add(__gm82dx8_controller,ev_alarm,0,"if (__gm82dx8_appsurfcompose!=noone) set_automatic_draw(true)")
        
        __gm82dx8_isntxp=__gm82dx8_not_xp()
        __gm82dx8_vsync_enabled=__gm82dx8_isntxp
    } else {
        //we say 8.2 even though it only technically needs 8.1 because why would you use 8.1
        show_error("Sorry, but Game Maker 8.2 DirectX8 requires Game Maker 8.2.",1)
    }


#define __gm82dx8_vsync
    //only activate if vsyncable
    var freq;freq=display_get_frequency()/room_speed
    
    if (abs(freq-round(freq))<0.03 && __gm82dx8_vsync_enabled && !__gm82dx8_vpatched) {
        set_synchronization(false)
        //we do timed wakeups every 1ms to check the time
        while (!__gm82dx8_waitvblank()) {
             __gm82dx8_sleep(1)
             if (__gm82dx8_time_now()-__gm82dx8_time>1000000/room_speed-2000) {
                //Oh my fur and whiskers! I'm late, I'm late, I'm late!
                break
            }
        }

        //busywait for vblank
        while (!__gm82dx8_waitvblank()) {/*òwó*/}
        __gm82dx8_time=__gm82dx8_time_now()

        //sync DWM
        __gm82dx8_sync_dwm()

        //epic win
    }


#define __gm82dx8_prepare
    if (__gm82dx8_appsurfcompose!=noone) {
        if (!surface_exists(application_surface)) {
            application_surface=surface_create(__gm82dx8_resw,__gm82dx8_resh)
        } else {
            __gm82dx8_resw=surface_get_width(application_surface)
            __gm82dx8_resh=surface_get_height(application_surface)
        }
        surface_set_target(application_surface)
    }


#define __gm82dx8_compose
    if (__gm82dx8_appsurfcompose!=noone) {
        d3d_set_depth(0)      
        if (!surface_exists(application_surface)) {
            application_surface=surface_create(__gm82dx8_resw,__gm82dx8_resh)
        } else {
            dx8_make_opaque()
        }
        surface_reset_target()        
        d3d_set_projection_ortho(0,0,__gm82dx8_resw,__gm82dx8_resh,0)
        script_execute(__gm82dx8_appsurfcompose)
    }
    __gm82dx8_vsync()


#define dx8_set_vsync
    ///dx8_set_vsync(enable)
    __gm82dx8_vsync_enabled=!!argument0 && __gm82dx8_isntxp
    
    
#define dx8_set_alphablend
    ///dx8_set_alphablend(enable)
    YoYo_EnableAlphaBlend(argument0)


#define dx8_make_opaque
    ///dx8_make_opaque()
    draw_set_blend_mode(bm_add)
    draw_rectangle_color(-9999999,-9999999,9999999,9999999,0,0,0,0,0)
    draw_set_blend_mode(0)


#define dx8_set_fullscreen_ext
    ///dx8_set_fullscreen_ext(enabled)
    //exclusive fullscreen
    //warning: buggy    
    if (argument0 ^ window_get_fullscreen()) {
        if (window_get_fullscreen()) {
            __gm82dx8_setfullscreen(0)
            window_set_fullscreen(0)
        } else {
            window_set_fullscreen(1)
            __gm82dx8_setfullscreen(display_get_frequency())
        }
    }


#define dx8_reset_projection
    ///dx8_reset_projection()
    if (view_enabled)
        d3d_set_projection_ortho(view_xview[view_current],view_yview[view_current],view_wview[view_current],view_hview[view_current],view_angle[view_current])
    else
        d3d_set_projection_ortho(0,0,room_width,room_height,0)


#define dx8_projection_simple
    ///dx8_projection_simple(x,y,w,h,tilt,dollyzoom,depthmin,depthfocus,depthmax,spin)
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
    

#define dx8_transform_vertex
    ///dx8_transform_vertex(x,y,z)
    dx8_transform_vertex[0]=__gm82dx8_transformvertex(argument0,argument1,argument2)
    dx8_transform_vertex[1]=__gm82dx8_getvertexy()
    dx8_transform_vertex[2]=__gm82dx8_getvertexz()


#define fog_trick
    ///fog_trick(color,amount)
    ///fog_trick()

    if (argument_count==2) {
        d3d_set_fog(1,argument[0],0.5-argument[1],1.5-argument[1])
    } else d3d_set_fog(0,0,0,0)
//
//