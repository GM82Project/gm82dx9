#define dx8_surface_engage
    ///dx8_surface_engage(id,width,height)
    var __surface;__surface=dx8_surface_ensure(argument0,argument1,argument2)
    if (__surface!=noone) {
        surface_set_target(__surface)
    }
    return __surface


#define __gm82dx8_flushsurfaces
    ///__gm82dx8_flushsurfaces()
    var __i;
    __i=0 repeat (100) if (surface_exists(__i)) {surface_free(__i) __i+=1}


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
    var __freemem;__freemem=dx8_get_free_video_memory()
    if (__freemem<__vram) {
        show_error(
            "Error creating surface of dimensions "+string(__rw)+"x"+string(__rh)+":"
            +chr($0d)+chr($0a)+chr($0d)+chr($0a)
            +"There is not enough available video memory (needs "+string(__vram)+"MB, had "+string(__freemem)+"MB)."
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


#define surface_get
    ///(name,width,height)
    var s,name,w,h,key;

    name=argument0
    w=argument1
    h=argument2

    s=ds_map_find_value(__gm82dx8_surfmap,name)
    if (s) {
        if (!surface_exists(s-1)) {
            //clear map & surfaces
            key=ds_map_find_first(__gm82dx8_surfmap)
            repeat (ds_map_size(__gm82dx8_surfmap)) {
                s=ds_map_find_value(__gm82dx8_surfmap,key)
                if (surface_exists(s-1)) surface_free(s-1)
                key=ds_map_find_next(__gm82dx8_surfmap,key)
            }
            ds_map_clear(__gm82dx8_surfmap)
            s=0
        }
    }

    if (!s) {
        //create surf
        s=surface_create(w,h)+1
        ds_map_add(__gm82dx8_surfmap,name,s)
    }

    return s-1


#define surface_set
    ///(name,width,height)
    var s;

    s=surface_get(argument0,argument1,argument2)

    surface_set_target(s)

    return s


#define surface_forget
    ///(name)
    var s,name;

    name=argument0

    s=ds_map_find_value(__gm82dx8_surfmap,name)

    if (s!=0) {
        if (surface_exists(s-1)) surface_free(s-1)
        ds_map_delete(__gm82dx8_surfmap,name)
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
//
//