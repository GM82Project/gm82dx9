#define __gm82dx8_init
    globalvar __gm82dx8_time;__gm82dx8_time=__gm82dx8_time_now()
    globalvar __gm82dx8_vsync_enabled;__gm82dx8_vsync_enabled=__gm82dx8_not_xp()
    
    if (__gm82dx8_checkstart()) exit
    
    if (execute_string("return get_function_address('display_get_orientation')") <= 0) {
        if (variable_global_get("__gm82core_version")>134) {
            //recent enough core extension: let's work together
            object_event_add(core,ev_other,ev_animation_end,"__gm82dx8_vsync()")
        } else {
            //core extension not detected: let's do it ourselves
            object_event_add(__gm82dx8_obj,ev_destroy,0,"instance_copy(0)")
            object_event_add(__gm82dx8_obj,ev_other,ev_room_end,"persistent=true")
            object_event_add(__gm82dx8_obj,ev_other,ev_animation_end,"__gm82dx8_vsync()")
            object_set_persistent(__gm82dx8_obj,1)
            room_instance_add(room_first,0,0,__gm82dx8_obj)
        }
    } else show_error("Sorry, but Game Maker 8.2 DirectX8 requires Game Maker 8.2.",1)


#define dx8_set_vsync
    ///dx8_set_vsync(enable)
    __gm82dx8_vsync_enabled=!!argument0 && __gm82dx8_not_xp()


#define dx8_set_alphablend
    ///dx8_enable_alphablend(enable)
    YoYo_EnableAlphaBlend(argument0)


#define dx8_make_opaque
    ///dx8_make_opaque()
    draw_set_blend_mode(bm_add)
    draw_rectangle_color(-9999999,-9999999,9999999,9999999,0,0,0,0,0)
    draw_set_blend_mode(0)


#define dx8_surface_engage
    ///dx8_surface_engage(id,width,height)
    var __s;__s=argument0
    var __rw;__rw=round(argument1)
    var __rh;__rh=round(argument2)
    
    if (surface_exists(__s)) {
        if (surface_get_width(__s)==__rw && surface_get_height(__s)==__rh) {
            surface_set_target(__s)
            return __s
        }
    }
    
    __s=surface_create(__rw,__rh)
    if (surface_exists(__s)) {
        var __sw;__sw=surface_get_width(__s)
        var __sh;__sh=surface_get_height(__s)
        if (__sw==__rw && __sh==__rh) {
            surface_set_target(__s)
            return __s
        }
        
        var __str;
        if (__rw>4096 || __rh>4096) __str="Your graphics card may not support very large textures."
        else if (__rw<16 || __rh<16) __str="Your graphics card may not support textures smaller than 8x8."
        else if (__rw != __rh) __str="Your graphics card may not support non-square textures."
        else __str="Your graphics card may not support the requested dimensions."
        
        show_error(
            "Error creating surface of dimensions "+string(argument1)+"x"+string(argument2)+":"
            +chr($0d)+chr($0a)+chr($0d)+chr($0a)
            +"Resulting DX8 surface was actually "+string(__sw)+"x"+string(__sh)+". "+__str
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
    if (variable_global_get("__gm82core_appsurf_interop")) {
        dx8_surface_engage(application_surface,core.__resw,core.__resh)
    } else {
        surface_reset_target()
        dx8_reset_projection()
    }    


#define dx8_reset_projection
    ///dx8_reset_projection()
    if (view_enabled)
        d3d_set_projection_ortho(view_xview[view_current],view_yview[view_current],view_wview[view_current],view_hview[view_current],view_angle[view_current])
    else
        d3d_set_projection_ortho(0,0,room_width,room_height,0)

    
#define dx8_set_fullscreen_ext
    ///dx8_set_exclusive_fullscreen(enabled)
    if (argument0 ^ window_get_fullscreen()) {
        if (window_get_fullscreen()) {
            __gm82dx8_setfullscreen(0)
            window_set_fullscreen(0)
            return 1
        } else {
            window_set_fullscreen(1)
            __gm82dx8_setfullscreen(display_get_frequency())
            return 1
        }
    }    
    return 0


#define dx8_projection_simple
    ///dx8_set_projection_simple(x,y,w,h,angle,dollyzoom,depthmin,depthfocus,depthmax)
    var __xfrom,__yfrom,__zfrom;

    if (argument5<=0) {
        // ¯\_(º_o)_/¯
        d3d_set_projection_ortho(argument0,argument1,argument2,argument3,argument4)
    } else {
        __xfrom=argument0+argument2/2
        __yfrom=argument1+argument3/2    
        __zfrom=min(-tan(degtorad(90*(1-argument5)))*argument3/2,argument6-argument7)

        d3d_set_projection_ext(
            __xfrom,__yfrom,__zfrom+argument7,                           //from
            __xfrom,__yfrom,argument7,                                   //to
            lengthdir_x(1,-argument4+90),lengthdir_y(1,-argument4+90),0, //up
            -point_direction(__zfrom,0,0,argument3/2)*2,                 //angle
            -argument2/argument3,                                        //aspect
            max(1,argument6-argument7-__zfrom),                          //znear
            argument8-argument7-__zfrom                                  //zfar
        )
    }
    

#define __gm82dx8_vsync
    //only activate if vsyncable
    var freq;freq=display_get_frequency()/room_speed
    
    if (abs(freq-round(freq))<0.03 && __gm82dx8_vsync_enabled) {
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


