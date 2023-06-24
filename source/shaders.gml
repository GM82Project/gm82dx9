#define shader_vertex_create_buffer
    ///shader_vertex_create_buffer(buffer)
    return __gm82dx9_shader_vertex_create_buffer(buffer_get_address(argument0,0),buffer_get_size(argument0))


#define shader_pixel_create_buffer
    ///shader_pixel_create_buffer(buffer)
    return __gm82dx9_shader_pixel_create_buffer(buffer_get_address(argument0,0),buffer_get_size(argument0))


#define shader_vertex_set_passthrough
    ///shader_vertex_set_passthrough()
    shader_vertex_set(__gm82dx9_default_vs)
    shader_vertex_uniform_matrix("rMatrixWVP",mtx_world_view_projection)
    

#define shader_pixel_set_passthrough
    ///shader_pixel_set_passthrough()
    shader_pixel_set(__gm82dx9_default_ps)


#define __gm82dx9_vertex_uniform
    var __addr;
    if (is_string(argument0)) {
        __addr=shader_vertex_uniform_get_address(argument0)
        if (__addr==noone) show_error("Uniform "+argument0+" doesn't exist in the set vertex shader.",0)
        return __addr
    }
    return argument0


#define __gm82dx9_pixel_uniform
    var __addr;
    if (is_string(argument0)) {
        __addr=shader_pixel_uniform_get_address(argument0)
        if (__addr==noone) show_error("Uniform "+argument0+" doesn't exist in the set pixel shader.",0)
        return __addr
    }
    return argument0


#define shader_vertex_uniform_f
    ///shader_vertex_uniform_f(uniform,...)
    var __i,__uni; __i=0
    __uni=__gm82dx9_vertex_uniform(argument0)
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_vertex_uniform_4f(__uni+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_vertex_uniform_4f(__uni+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_vertex_uniform_4f(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_vertex_uniform_4f(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (false)


#define shader_pixel_uniform_f
    ///shader_pixel_uniform_f(uniform,...)
    var __i,__uni; __i=0
    __uni=__gm82dx9_pixel_uniform(argument0)
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_pixel_uniform_4f(__uni+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_pixel_uniform_4f(__uni+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_pixel_uniform_4f(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_pixel_uniform_4f(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (false)


#define shader_vertex_uniform_i
    ///shader_vertex_uniform_i(uniform,...)
    var __i,__uni; __i=0
    __uni=__gm82dx9_vertex_uniform(argument0)
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_vertex_uniform_4i(__uni+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_vertex_uniform_4i(__uni+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_vertex_uniform_4i(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_vertex_uniform_4i(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (false)


#define shader_pixel_uniform_i
    ///shader_pixel_uniform_i(uniform,...)
    var __i,__uni; __i=0
    __uni=__gm82dx9_pixel_uniform(argument0)
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_pixel_uniform_4i(__uni+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_pixel_uniform_4i(__uni+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_pixel_uniform_4i(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_pixel_uniform_4i(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (false)


#define shader_vertex_uniform_b
    ///shader_vertex_uniform_b(uniform,...)
    var __i,__uni; __i=0
    __uni=__gm82dx9_vertex_uniform(argument0)
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: case 3: case 4: __gm82dx9_shader_vertex_uniform_b(__uni+__i,argument[1+__i]) __i+=1 continue
        default:
            __gm82dx9_shader_vertex_uniform_4b(__uni+__i,argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (false)


#define shader_pixel_uniform_b
    ///shader_pixel_uniform_b(uniform,...)
    var __i,__uni; __i=0
    __uni=__gm82dx9_pixel_uniform(argument0)
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: case 3: case 4: __gm82dx9_shader_pixel_uniform_b(__uni+__i,argument[1+__i]) __i+=1 continue
        default:
            __gm82dx9_shader_pixel_uniform_4b(__uni+__i,argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (false)


#define shader_vertex_uniform_matrix
    //shader_vertex_uniform_matrix(uniform,mtx_type)
    __gm82dx9_shader_vertex_uniform_matrix(__gm82dx9_vertex_uniform(argument0),argument1)


#define shader_pixel_uniform_matrix
    //shader_pixel_uniform_matrix(uniform,mtx_type)
    __gm82dx9_shader_pixel_uniform_matrix(__gm82dx9_pixel_uniform(argument0),argument1)


#define shader_vertex_uniform_color
    ///shader_vertex_uniform_color(uniform,color,[alpha])
    if (argument_count==3)
        shader_vertex_uniform_f(argument[0],color_get_red(argument[1])/255,color_get_green(argument[1])/255,color_get_blue(argument[1])/255,argument[2])
    else
        shader_vertex_uniform_f(argument[0],color_get_red(argument[1])/255,color_get_green(argument[1])/255,color_get_blue(argument[1])/255,1)


#define shader_pixel_uniform_color
    ///shader_pixel_uniform_color(uniform,color,[alpha])
    if (argument_count==3)
        shader_pixel_uniform_f(argument[0],color_get_red(argument[1])/255,color_get_green(argument[1])/255,color_get_blue(argument[1])/255,argument[2])
    else
        shader_pixel_uniform_f(argument[0],color_get_red(argument[1])/255,color_get_green(argument[1])/255,color_get_blue(argument[1])/255,1)


#define shader_pixel_create_base64
    ///shader_pixel_create_base64(string)
    var __buf,__shd;
    __buf=buffer_create()
    buffer_write_base64(__buf,argument0)
    buffer_inflate(__buf)
    __shd=shader_pixel_create_buffer(__buf)
    buffer_destroy(__buf)
    return __shd


#define shader_vertex_create_base64
    ///shader_vertex_create_base64(string)
    var __buf,__shd;
    __buf=buffer_create()
    buffer_write_base64(__buf,argument0)
    buffer_inflate(__buf)
    __shd=shader_vertex_create_buffer(__buf)
    buffer_destroy(__buf)
    return __shd


#define texture_set_repeat_ext
    ///texture_set_repeat_ext(hrepeat,vrepeat,bordercolor)
    __gm82dx9_texture_set_stage_repeat(0,argument0,argument1,argument2)


#define texture_set_stage
    ///texture_set_stage(sampler,texture)
    __gm82dx9_texture_stage_set(__gm82dx9_pixel_uniform(argument0),argument1)


#define texture_set_stage_interpolation
    ///texture_set_stage_interpolation(sampler,texture)
    __gm82dx9_texture_set_stage_interpolation(__gm82dx9_pixel_uniform(argument0),argument1)


#define texture_set_stage_repeat
    ///texture_set_stage_repeat(sampler,hrepeat,vrepeat,bordercolor)
    __gm82dx9_texture_set_stage_repeat(__gm82dx9_pixel_uniform(argument0),argument1,argument2,argument3)


#define texture_set_stage_vertex
    ///texture_set_stage_vertex(sampler,texture)
    __gm82dx9_texture_stage_vertex_set(__gm82dx9_vertex_uniform(argument0),argument1)


#define texture_set_stage_vertex_interpolation
    ///texture_set_stage_vertex_interpolation(sampler,texture)
    __gm82dx9_texture_set_stage_vertex_interpolation(__gm82dx9_vertex_uniform(argument0),argument1)


#define texture_set_stage_vertex_repeat
    ///texture_set_stage_vertex_repeat(sampler,hrepeat,vrepeat,bordercolor)
    __gm82dx9_texture_set_stage_vertex_repeat(__gm82dx9_vertex_uniform(argument0),argument1,argument2,argument3)


#define shader_draw_shadertoy
    ///shader_draw_shadertoy(shader,x,y,w,h,time)
    
    shader_vertex_set_passthrough()

    shader_pixel_set(argument0)
    shader_pixel_uniform_f(shader_pixel_uniform_get_address("_iGlobalTime"),argument5)
    shader_pixel_uniform_f(shader_pixel_uniform_get_address("_iTime"),argument5)
    shader_pixel_uniform_f(shader_pixel_uniform_get_address("_iResolution"),argument3,argument4,0)
    shader_pixel_uniform_f(shader_pixel_uniform_get_address("_iMouse"),x,y,0,0)
    shader_pixel_uniform_f(shader_pixel_uniform_get_address("_iFrameRate"),room_speed)

    draw_primitive_begin_texture(pr_trianglestrip,-1)
    draw_vertex_texture(argument1,argument2,0,1)
    draw_vertex_texture(argument1+argument3,argument2,1,1)
    draw_vertex_texture(argument1,argument2+argument4,0,0)
    draw_vertex_texture(argument1+argument3,argument2+argument4,1,0)
    draw_primitive_end()
    
    shader_reset()


#define draw_quad
    ///draw_quad(x,y,w,h,tex,[u1,v1,u2,v2])
    
    if (argument_count!=9 && argument_count!=5) {
        show_error("Wrong number of arguments for draw_quad().",0)
        exit
    }
    
    texture_set_repeat(1)
    draw_primitive_begin_texture(pr_trianglestrip,argument4)
    if (argument_count==9) {    
        draw_vertex_texture(argument0,argument1,argument5,argument6)
        draw_vertex_texture(argument0+argument2,argument1,argument7,argument6)
        draw_vertex_texture(argument0,argument1+argument3,argument5,argument8)
        draw_vertex_texture(argument0+argument2,argument1+argument3,argument7,argument8)
    } else {    
        draw_vertex_texture(argument0,argument1,0,0)
        draw_vertex_texture(argument0+argument2,argument1,1,0)
        draw_vertex_texture(argument0,argument1+argument3,0,1)
        draw_vertex_texture(argument0+argument2,argument1+argument3,1,1)
    }
    draw_primitive_end()


#define surface_set_target_ext
    ///surface_set_target_ext(target,surface)
    var surf;
    if (is_string(argument1)) {
        if (!ds_map_exists(__gm82dx9_surfmap,argument1)) {
            show_error("Error trying to set target to unknown surface: "+string(argument1),0)
            return noone
        }
        surf=ds_map_find_value(__gm82dx9_surfmap,argument1)-1
    } else surf=argument1
    
    if (!surface_exists(surf)) {
        show_error("Trying to set target to nonexisting surface("+string(argument1)+")",0)
        return 0
    }
    if (argument0==0) surface_set_target(surf)
    else __gm82dx9_surface_set_target_ext(argument0,surf)


#define surface_reset_target_ext
    ///surface_reset_target_ext(target)
    if (argument0==0) surface_reset_target()
    else __gm82dx9_surface_set_target_ext(argument0,-1)


#define vertex_buffer_create
    ///vertex_buffer_create(buffer,stride)
    __gm82dx9_vertex_create_buffer_from_buffer(buffer_get_address(argument0,0),buffer_get_size(argument0),argument1)


#define vertex_format_add_position
    ///vertex_format_add_position(slot)
    vertex_format_add_custom(vf_type_float4,vf_usage_position,argument0)


#define vertex_format_add_texcoord
    ///vertex_format_add_texcoord(slot)
    vertex_format_add_custom(vf_type_float2,vf_usage_texcoord,argument0)


#define vertex_format_add_colour
    ///vertex_format_add_colour(slot)
    vertex_format_add_custom(vf_type_d3dcolor,vf_usage_colour,argument0)


#define vertex_format_add_normal
    ///vertex_format_add_normal(slot)
    vertex_format_add_custom(vf_type_float4,vf_usage_normal,argument0)


#define buffer_write_format_default
    ///buffer_write_format_default(buffer,x,y,z,nx,ny,nz,u,v,color,alpha)

    buffer_write_float(argument0,argument1)
    buffer_write_float(argument0,argument2)
    buffer_write_float(argument0,argument3)
    buffer_write_float(argument0,0)
    
    buffer_write_float(argument0,argument4)
    buffer_write_float(argument0,argument5)
    buffer_write_float(argument0,argument6)
    buffer_write_float(argument0,0)
    
    buffer_write_u32(argument0,color_to_d3dcolor(argument9,argument10))
    
    buffer_write_float(argument0,argument7)
    buffer_write_float(argument0,argument8)


#define buffer_write_format_position
    ///buffer_write_format_position(buffer,x,y,z)
    buffer_write_float(argument0,argument1)
    buffer_write_float(argument0,argument2)
    buffer_write_float(argument0,argument3)
    buffer_write_float(argument0,0)


#define buffer_write_format_texcoord
    ///buffer_write_format_texcoord(buffer,u,v)
    buffer_write_float(argument0,argument1)
    buffer_write_float(argument0,argument2)


#define buffer_write_format_colour
    ///buffer_write_format_colour(buffer,color,alpha)
    buffer_write_u32(argument0,color_to_d3dcolor(argument1,argument2))


#define vertex_format_create_simple
    ///vertex_format_create_simple(vf_usage_...)
    vertex_format_begin()
    i=0 repeat (argument_count) {
        if (argument[i]==vf_usage_position) vertex_format_add_position(0)
        else if (argument[i]==vf_usage_texcoord) vertex_format_add_texcoord(0)
        else if (argument[i]==vf_usage_normal) vertex_format_add_normal(0)
        else if (argument[i]==vf_usage_colour) vertex_format_add_colour(0)
        else show_error("invalid usage semantic passed to vertex_format_simple",0)
    i+=1}
    return vertex_format_end()


#define vertex_format_create_default
    ///vertex_format_create_default()
    vertex_format_begin()
    vertex_format_add_position(0)
    vertex_format_add_normal(0)
    vertex_format_add_colour(0)
    vertex_format_add_texcoord(0)    
    return vertex_format_end()


#define vertex_buffer_draw
    ///vertex_buffer_draw(vbuffer,vformat,primitive,texture)
    var vertices,count;
    
    vertices=vertex_buffer_get_size(argument0)/vertex_format_get_size(argument1,0)
        
    if (argument2==pr_pointlist    ) count=vertices
    if (argument2==pr_linelist     ) count=vertices div 2
    if (argument2==pr_linestrip    ) count=vertices-1    
    if (argument2==pr_trianglelist ) count=vertices div 3
    if (argument2==pr_trianglefan
    ||  argument2==pr_trianglestrip) count=vertices-2      

    __gm82dx9_vertex_draw_buffer(argument0,argument1,argument2,argument3,count,false)
//
//