#define surface_discard
    ///surface_discard(id)
    if (surface_exists(argument0)) {
        surface_free(argument0)
    }


#define surface_reset
    ///surface_reset()
    if (__gm82dx9_appsurfcompose!=noone) {
        application_surface=surface_set("application_surface",__gm82dx9_resw,__gm82dx9_resh)
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
    ds_map_clear(__gm82dx9_surfidmap)


#define surface_get
    ///surface_get(name,width,height)
    var __s,__name,__w,__h;

    __name=argument0
    __w=argument1
    __h=argument2
    
    __gm82dx9_surface_was_new=false

    __s=ds_map_find_value(__gm82dx9_surfmap,__name)
    if (__s) {
        if (!surface_exists(__s-1)) {
            show_debug_message("surface "+__name+" went missing")
            surface_forget_all()
            __s=0
        } else {
            if (surface_get_address(__s-1)!=ds_map_find_value(__gm82dx9_surfidmap,__name)) {
                show_debug_message("surface "+__name+" changed id")
                surface_forget_all()
                __s=0
            }
        }
    }

    if (!__s) {
        //create surf
        __s=surface_create(__w,__h)+1
        if (!surface_exists(__s-1)) {
            if (d3d_get_free_video_memory()<(__w*__h*4*2)) {
                if (debug_mode) show_error("Error in function surface_get: couldn't create a surface of dimensions "+string(argument1)+"x"+string(argument2)+". Reason: Out of memory!",0)
                else show_message("Out of memory!")
            } else
                if (debug_mode) show_error("Error in function surface_get: couldn't create a surface of dimensions "+string(argument1)+"x"+string(argument2)+". Reason: Possibly too big!",0)
            return noone
        }
        ds_map_add(__gm82dx9_surfmap,__name,__s)
        ds_map_add(__gm82dx9_surfidmap,__name,surface_get_address(__s-1))
        __gm82dx9_surface_was_new=true
    }

    return __s-1


#define surface_resize
    ///surface_resize(name,w,h,scale,filter)
    //name: name of the surface to resize
    //w,h: new dimensions to resize to
    //scale: whether to scale the image to fit the new surface
    //filter: whether to use a bilinear filter when scaling the image
    //returns: id of the new surface generated for the resize operation
    
    var __old,__new,__i;
    
    if (!ds_map_exists(__gm82dx9_surfmap,argument0)) {
        show_error("Error trying to resize unknown surface: "+string(argument0),0)
        return noone
    }
    __old=ds_map_find_value(__gm82dx9_surfmap,argument0)-1
    
    __new=surface_create(argument1,argument2)
    
    if (!surface_exists(__new-1)) {
        if (d3d_get_free_video_memory()<(__w*__h*4*2)) {
                if (debug_mode) show_error("Error in function surface_get: couldn't create a surface of dimensions "+string(argument1)+"x"+string(argument2)+". Reason: Out of memory!",0)
                else show_message("Out of memory!")
        } else
            show_error("Error in function surface_resize: couldn't create a surface of dimensions "+string(argument1)+"x"+string(argument2)+". Reason: Possibly too big!",0)
        return noone
    }
    
    if (surface_exists(__old)) {
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
        
        surface_free(__old) 
    }
    
    ds_map_replace(__gm82dx9_surfmap,argument0,__new+1)
    ds_map_replace(__gm82dx9_surfidmap,argument0,surface_get_address(__new))
    
    return __new    


#define surface_is_new
    ///surface_is_new()
    return __gm82dx9_surface_was_new


#define surface_load
    ///surface_load(id,fname)
    var __bg;
    
    if (surface_exists(argument0)) {
        if (file_exists(argument1)) {
            __bg=background_add(argument1,0,0)
            if (background_exists(__bg)) {
                surface_set_target(argument0)
                draw_clear_alpha(0,0)
                d3d_set_projection_ortho(0,0,surface_get_width(argument0),surface_get_height(argument0),0)
                draw_background(__bg,0,0)
                background_delete(__bg)
                surface_reset()
                return true
            }
            show_error("error in function surface_load: trying to load invalid image file file ("+string(argument1)+") into surface ("+string(argument0)+")",0)
            return false
        }
        show_error("error in function surface_load: trying to load nonexisting file ("+string(argument1)+") into surface ("+string(argument0)+")",0)
        return false        
    }
    show_error("error in function surface_load: trying to load file ("+string(argument1)+") into nonexisting surface ("+string(argument0)+")",0)
    return false


#define surface_set
    ///surface_set(name,width,height)
    var __s;

    __s=surface_get(argument0,argument1,argument2)

    surface_set_target(__s)

    return __s


#define surface_forget
    ///surface_forget(name)
    var __s,__name;

    __name=argument0

    __s=ds_map_find_value(__gm82dx9_surfmap,__name)

    if (__s!=0) {
        if (surface_exists(__s-1)) surface_free(__s-1)
        ds_map_delete(__gm82dx9_surfmap,__name)
        ds_map_delete(__gm82dx9_surfidmap,__name)
    }


#define surface_set_depth
	if (surface_exists(argument0)) __gm82dx9_surface_set_depth(argument0)


#define application_surface_enable
    ///application_surface_enable(postdraw script id)
    //Enables the automatic use and management of an application surface.
    //The postdraw script will be called when the screen is being composed, so any post-processing effects can be applied to it.
    
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
    //Disables the application surface.
    
    if (__gm82dx9_appsurfcompose!=noone) {
        __gm82dx9_appsurfcompose=noone
        surface_forget("application_surface")
        application_surface=noone
    }


#define application_surface_is_enabled
    ///application_surface_is_enabled()
    //returns: whether the application surface is enabled.
    
    return __gm82dx9_appsurfcompose!=noone


#define application_surface_resize
    ///application_surface_resize(width,height)
    //width,height: new size
    //Resizes the application surface.
    if (__gm82dx9_appsurfcompose!=noone) {
        __gm82dx9_resw=argument0
        __gm82dx9_resh=argument1
        surface_resize("application_surface",__gm82dx9_resw,__gm82dx9_resh,1,1)
    }


#define application_surface_get_width
    ///application_surface_get_width()
    //returns: The current width of the application surface.
    return __gm82dx9_resw


#define application_surface_get_height
    ///application_surface_get_height()
    //returns: The current height of the application surface.
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
    //buffer: a gm82net buffer with the color data.
    //surface: a surface to receive the contents.
    //This function will push the buffer's color data to the surface.
    //The bufer must have enough data in it (at least width*height*4 bytes).
    //You can pack a color and alpha value into u32s for use with buffers using argb_make_color(color,alpha).
    //Do note that this function is very slow, so use it sparingly.
    
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
    //buffer: a gm82net buffer to receive the surface contents.
    //surface: a surface to get the contents from.
    //This function will retrieve the surface's color data into the buffer.
    //Each pixel is stored as an u32 0xAABBGGRR color value.
    //The buffer will be resized to fit the surface's contents (new size is width*height*4 bytes).
    //-> To get a specific (x,y) pixel from the buffer, the address would be: ((y * width) + x) * 4
    //To get the alpha and color values of a pixel:
    //   pixel_u32 = buffer_read_u32(b)
    //   alpha = argb_get_alpha(pixel_u32)
    //   color = argb_get_color(pixel_u32)
    //Do note that this function is very slow, so use it sparingly.
    
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


#define surface_get_address
    ///surface_get_address(surface)
    //surface: surface id
    //returns: 32 bit address of the IDirect3DSurface9 structure associated with the surface.
    
    if (surface_exists(argument0)) {
        return __gm82dx9_surface_ptr(argument0)
    }
    return noone

//
//