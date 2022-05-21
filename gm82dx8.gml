#define __gm82dx8_init
    if (__gm82dx8_checkstart()) exit
    
    globalvar __gm82dx8_time;           __gm82dx8_time=__gm82dx8_time_now()
    globalvar __gm82dx8_vsync_enabled;
    globalvar __gm82dx8_vpatched;       __gm82dx8_vpatched=false
    globalvar __gm82dx8_controller;     __gm82dx8_controller=__gm82dx8_obj
    globalvar __gm82dx8_appsurfcompose; __gm82dx8_appsurfcompose=noone
    globalvar __gm82dx8_isntxp;
    globalvar __gm82dx8_resw,__gm82dx8_resh;
    
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


#define dx8_set_vsync
    ///dx8_set_vsync(enable)
    __gm82dx8_vsync_enabled=!!argument0 && __gm82dx8_isntxp


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


#define dx8_set_alphablend
    ///dx8_set_alphablend(enable)
    YoYo_EnableAlphaBlend(argument0)


#define dx8_make_opaque
    ///dx8_make_opaque()
    draw_set_blend_mode(bm_add)
    draw_rectangle_color(-9999999,-9999999,9999999,9999999,0,0,0,0,0)
    draw_set_blend_mode(0)


#define dx8_surface_engage
    ///dx8_surface_engage(id,width,height)
    var __surface;__surface=dx8_surface_ensure(argument0,argument1,argument2)
    if (__surface!=noone) {
        surface_set_target(__surface)
    }
    return __surface


#define dx8_surface_ensure
    ///dx8_surface_ensure(id,width,height)
    var __s;__s=argument0
    var __rw;__rw=round(argument1)
    var __rh;__rh=round(argument2)
    
    if (surface_exists(__s)) {
        if (surface_get_width(__s)==__rw && surface_get_height(__s)==__rh) {
            return __s
        }
    }
    
    var __mw;__mw=dx8_texture_get_max_width()
    var __mh;__mh=dx8_texture_get_max_height()
    
    if (__rw>__mw || __rh>__mh) {
        show_error(
            "Error creating surface of dimensions "+string(__rw)+"x"+string(__rh)+":"
            +chr($0d)+chr($0a)+chr($0d)+chr($0a)
            +"Maximum texture size allowed is "+string(__mw)+"x"+string(__mh)+"."
            ,1
        )
        return noone
    }
    
    var __vram;__vram=(__rw*__rh*8)/1048576
    if (dx8_get_free_video_memory()<__vram) {
        show_error(
            "Error creating surface of dimensions "+string(__rw)+"x"+string(__rh)+":"
            +chr($0d)+chr($0a)+chr($0d)+chr($0a)
            +"There is not enough available video memory ("+string(__vram)+"MB)."
            ,1
        )
        return noone
    }
    
    __s=surface_create(__rw,__rh)
    if (surface_exists(__s)) {
        var __sw;__sw=surface_get_width(__s)
        var __sh;__sh=surface_get_height(__s)
        if (__sw==__rw && __sh==__rh) {
            return __s
        }
        
        var __str;
        if (__rw>4096 || __rh>4096) __str="Your graphics card may not support very large textures."
        else if (__rw<16 || __rh<16) __str="Your graphics card may not support very small textures."
        else __str="Your graphics card may not support the requested dimensions."
        
        show_error(
            "Error creating surface of dimensions "+string(argument1)+"x"+string(argument2)+":"
            +chr($0d)+chr($0a)+chr($0d)+chr($0a)
            +"Returned DX8 surface was actually "+string(__sw)+"x"+string(__sh)+". "+__str
            ,1
        )
        return noone
    }
    show_error(
        "Error creating surface of dimensions "+string(argument1)+"x"+string(argument2)+":"
        +chr($0d)+chr($0a)+chr($0d)+chr($0a)
        +"Surface failed to allocate. You might've ran out of video memory."
        ,1
    )
    return noone


#define dx8_surface_discard
    ///dx8_surface_discard(id)
    if (surface_exists(argument0)) {
        surface_free(argument0)
    }


#define dx8_surface_disengage
    ///dx8_surface_disengage()
    if (__gm82dx8_appsurfcompose!=noone) {
        application_surface=dx8_surface_engage(application_surface,__gm82dx8_resw,__gm82dx8_resh)
    } else {
        surface_reset_target()
        dx8_reset_projection()
    }    


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
    var __xfrom,__yfrom,__zfrom;

    if (argument5<=0) {
        // ¯\_(º_o)_/¯
        d3d_set_projection_ortho(argument0,argument1,argument2,argument3,argument4)
        __xfrom=argument0+argument2/2
        __yfrom=argument1+argument3/2    
        
        d3d_set_projection(
            __xfrom-lengthdir_x(1,argument9),__yfrom,-lengthdir_y(1,argument9),
            __xfrom,__yfrom,0,
            -lengthdir_x(1,-argument4+90),-lengthdir_y(1,-argument4+90),0
        )
    } else {
        __xfrom=argument0+argument2/2
        __yfrom=argument1+argument3/2    
        __zfrom=min(-tan(degtorad(90*(1-argument5)))*argument3/2,argument6-argument7)

        d3d_set_projection_ext(
            __xfrom+lengthdir_x(__zfrom,argument9),                      //xfrom
            __yfrom,                                                     //yfrom
            argument7+lengthdir_y(__zfrom,argument9),                    //zfrom
            __xfrom,__yfrom,argument7,                                   //to
            lengthdir_x(1,-argument4+90),lengthdir_y(1,-argument4+90),0, //up
            -point_direction(__zfrom,0,0,argument3/2)*2,                 //angle
            -argument2/argument3,                                        //aspect
            max(1,argument6-argument7-__zfrom),                          //znear
            argument8-argument7-__zfrom                                  //zfar
        )
    }
    

#define application_surface_enable
    ///application_surface_enable(postdraw script id)
    //envelope engine v5
    //renex 2021

    if (is_string(argument0)) {
        show_error("application_surface_enable() requires a valid script id (not in quotes)",1)
    } else if (!script_exists(argument0)) {
        show_error("application_surface_enable() was given a nonexisting script id",1)
    } else {
        __gm82dx8_appsurfcompose=argument0
        __gm82dx8_resw=window_get_region_width()
        __gm82dx8_resh=window_get_region_height()
        globalvar application_surface;
        application_surface=dx8_surface_engage(application_surface,__gm82dx8_resw,__gm82dx8_resh)
    }


#define application_surface_disable
    ///application_surface_disable()
    if (__gm82dx8_appsurfcompose!=noone) {
        __gm82dx8_appsurfcompose=noone
        dx8_surface_discard(application_surface)
        application_surface=noone
    }


#define application_surface_is_enabled
    ///application_surface_is_enabled()
    return __gm82dx8_appsurfcompose!=noone


#define application_surface_resize
    ///application_surface_resize(width,height)
    if (__gm82dx8_appsurfcompose!=noone) {
        dx8_surface_discard(application_surface)
        __gm82dx8_resw=argument0
        __gm82dx8_resh=argument1
        application_surface=surface_create(__gm82dx8_resw,__gm82dx8_resh)
    }


#define application_surface_get_width
    ///application_surface_get_width()
    return __gm82dx8_resw


#define application_surface_get_height
    ///application_surface_is_enabled()
    return __gm82dx8_resh


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


#define fog_trick
    ///fog_trick(color,amount)
    ///fog_trick()

    if (argument_count==2) {
        d3d_set_fog(1,argument[0],0.5-argument[1],1.5-argument[1])
    } else d3d_set_fog(0,0,0,0)
//
//