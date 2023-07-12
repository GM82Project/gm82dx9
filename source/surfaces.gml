#define surface_discard
    ///surface_discard(id)
    if (surface_exists(argument0)) {
        surface_free(argument0)
    }


#define surface_reset
    ///surface_reset()
    if (__gm82dx9_appsurfcompose!=noone) {
        application_surface=surface_get("application_surface",__gm82dx9_resw,__gm82dx9_resh)
    } else {
        surface_reset_target()
        d3d_set_projection_default()
    }    


#define surface_forget_all
    ///surface_forget_all()
    var __s,__key;
    __key=ds_map_find_first(__gm82dx9_surfmap)
    repeat (ds_map_size(__gm82dx9_surfmap)) {
        __s=ds_map_find_value(__gm82dx9_surfmap,__key)
        if (surface_exists(__s-1)) surface_free(__s-1)
        __key=ds_map_find_next(__gm82dx9_surfmap,__key)
    }
    ds_map_clear(__gm82dx9_surfmap)


#define surface_get
    ///(name,width,height)
    var __s,__name,__w,__h;

    __name=argument0
    __w=argument1
    __h=argument2
    
    __gm82dx9_surface_was_new=false

    __s=ds_map_find_value(__gm82dx9_surfmap,__name)
    if (__s) {
        if (!surface_exists(__s-1)) {
            surface_forget_all()
            __s=0
        }
    }

    if (!__s) {
        //create surf
        __s=surface_create(__w,__h)+1
        ds_map_add(__gm82dx9_surfmap,__name,__s)
        __gm82dx9_surface_was_new=true
    }

    return __s-1


#define surface_resize
    ///surface_resize(name,w,h,scale,filter)
    var __old,__new,__i;
    
    if (!ds_map_exists(__gm82dx9_surfmap,argument0)) {
        show_error("Error trying to resize unknown surface: "+string(argument0),0)
        return noone
    }
    __old=ds_map_find_value(__gm82dx9_surfmap,argument0)-1
    
    __new=surface_create(argument1,argument2)
    
    if (argument3) {
        surface_set_target(__new)
        d3d_set_projection_ortho(0,0,argument1,argument2,0)
        __i=texture_get_interpolation()
        texture_set_interpolation(argument4)
        draw_surface_stretched(__old,0,0,argument1,argument2)
        texture_set_interpolation(__i)
        surface_reset()
    } else {
        surface_copy_part(__new,0,0,__old,0,0,min(surface_get_width(__old),argument1),min(surface_get_height(__old),argument2))
    }
    
    surface_discard(__old) 
    
    ds_map_replace(__gm82dx9_surfmap,argument0,__new+1)    


#define surface_is_new
    ///surface_is_new()
    return __gm82dx9_surface_was_new


#define surface_set
    ///(name,width,height)
    var __s;

    __s=surface_get(argument0,argument1,argument2)

    surface_set_target(__s)

    return __s


#define surface_forget
    ///(name)
    var __s,__name;

    __name=argument0

    __s=ds_map_find_value(__gm82dx9_surfmap,__name)

    if (__s!=0) {
        if (surface_exists(__s-1)) surface_free(__s-1)
        ds_map_delete(__gm82dx9_surfmap,__name)
    }


#define surface_set_depth
	if (surface_exists(argument0)) __gm82dx9_surface_set_depth(argument0)


#define application_surface_enable
    ///application_surface_enable(postdraw script id)
    //envelope engine v5
    //renex 2023

    if (is_string(argument0)) {
        show_error("application_surface_enable() requires a valid script id (not in quotes)",1)
    } else if (!script_exists(argument0)) {
        show_error("application_surface_enable() was given a nonexisting script id",1)
    } else {
        __gm82dx9_appsurfcompose=argument0
        __gm82dx9_resw=window_get_region_width()
        __gm82dx9_resh=window_get_region_height()
        globalvar application_surface;
        application_surface=surface_get("application_surface",__gm82dx9_resw,__gm82dx9_resh)
    }


#define application_surface_disable
    ///application_surface_disable()
    if (__gm82dx9_appsurfcompose!=noone) {
        __gm82dx9_appsurfcompose=noone
        surface_forget("application_surface")
        application_surface=noone
    }


#define application_surface_is_enabled
    ///application_surface_is_enabled()
    return __gm82dx9_appsurfcompose!=noone


#define application_surface_resize
    ///application_surface_resize(width,height)
    if (__gm82dx9_appsurfcompose!=noone) {
        __gm82dx9_resw=argument0
        __gm82dx9_resh=argument1
        surface_resize("application_surface",__gm82dx9_resw,__gm82dx9_resh,1,1)
    }


#define application_surface_get_width
    ///application_surface_get_width()
    return __gm82dx9_resw


#define application_surface_get_height
    ///application_surface_is_enabled()
    return __gm82dx9_resh


#define __gm82dx9_prepare
    if (__gm82dx9_appsurfcompose!=noone) {
        application_surface=surface_set("application_surface",__gm82dx9_resw,__gm82dx9_resh)
    }


#define __gm82dx9_compose
    if (__gm82dx9_appsurfcompose!=noone) {
        d3d_set_depth(0)      
        surface_reset_target()        
        application_surface=surface_get("application_surface",__gm82dx9_resw,__gm82dx9_resh)        
        d3d_set_projection_ortho(0,0,__gm82dx9_resw,__gm82dx9_resh,0)
        script_execute(__gm82dx9_appsurfcompose)
    }


#define buffer_set_surface
    ///buffer_set_surface(buffer,surface)
    var __buf,__surf;
    
    __buf=argument0
    __surf=argument1
    
    if (!surface_exists(__surf)) {
        show_error("Trying to set nonexisting surface("+string(__surf)+") data from a buffer("+string(__buf)+").",0)
        return 0
    }
    
    if (!buffer_exists(__buf)) {
        show_error("Trying to set surface("+string(__surf)+") data from a nonexisting buffer("+string(__buf)+").",0)
        return 0
    }
    
    if (buffer_get_size(__buf)<surface_get_width(__surf)*surface_get_height(__surf)) {
        show_error("Trying to set surface("+string(__surf)+") data from a buffer("+string(__buf)+") that is too small.",0)
        return 0
    }        
    
    __gm82dx9_buffer_to_surface(buffer_get_address(__buf,0),__surf,surface_get_width(__surf),surface_get_height(__surf))
    return 1


#define buffer_get_surface
    ///buffer_get_surface(buffer,surface)
    var __buf,__surf;
    
    __buf=argument0
    __surf=argument1
    
    if (!surface_exists(__surf)) {
        show_error("Trying to get nonexisting surface("+string(__surf)+") data into a buffer("+string(__buf)+").",0)
        return 0
    }
    
    if (!buffer_exists(__buf)) {
        show_error("Trying to get surface("+string(__surf)+") data into a nonexisting buffer("+string(__buf)+").",0)
        return 0
    }
    
    buffer_set_size(__buf,surface_get_width(__surf)*surface_get_height(__surf)*4)
    buffer_set_pos(__buf,0)         
    
    __gm82dx9_surface_to_buffer(buffer_get_address(__buf,0),__surf,surface_get_width(__surf),surface_get_height(__surf))
    return 1
//
//