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


#define shader_vertex_uniform_exists
    ///shader_vertex_uniform_exists(name)
    return (__gm82dx9_get_vconst(argument0)!=noone) 


#define shader_pixel_uniform_exists
    ///shader_pixel_uniform_exists(name)
    return (__gm82dx9_get_pconst(argument0)!=noone) 


#define shader_vertex_uniform_get_address
    ///shader_vertex_uniform_get_address(uniform)
    var __addr;
    if (is_string(argument0)) {
        __addr=__gm82dx9_get_vconst(argument0)
        if (__addr==noone) show_error("Uniform "+argument0+" doesn't exist in the set vertex shader.",0)
        return __addr
    }
    return argument0


#define shader_pixel_uniform_get_address
    ///shader_pixel_uniform_get_address(uniform)
    var __addr;
    if (is_string(argument0)) {
        __addr=__gm82dx9_get_pconst(argument0)
        if (__addr==noone) show_error("Uniform "+argument0+" doesn't exist in the set pixel shader.",0)
        return __addr
    }
    return argument0


#define shader_vertex_uniform_f
    ///shader_vertex_uniform_f(uniform,...)
    var __i,__uni; __i=0
    __uni=shader_vertex_uniform_get_address(argument0)
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
    __uni=shader_pixel_uniform_get_address(argument0)
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
    __uni=shader_vertex_uniform_get_address(argument0)
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
    __uni=shader_pixel_uniform_get_address(argument0)
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
    __uni=shader_vertex_uniform_get_address(argument0)
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
    __uni=shader_pixel_uniform_get_address(argument0)
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: case 3: case 4: __gm82dx9_shader_pixel_uniform_b(__uni+__i,argument[1+__i]) __i+=1 continue
        default:
            __gm82dx9_shader_pixel_uniform_4b(__uni+__i,argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (false)


#define shader_vertex_uniform_matrix
    ///shader_vertex_uniform_matrix(uniform,mtx_type,[size])
    var __address,__size;
    if (argument_count<=1 || argument_count>=3) {
        show_error("Error in function shader_vertex_uniform_matrix: wrong number of arguments.",0)
        exit
    }    
    if (argument_count==3) {
        __address=shader_vertex_uniform_get_address(argument[0])
        __size=argument[2]
    } else {
        if (!is_string(argument[0])) {
            show_error("Error in function shader_vertex_uniform_matrix: 'uniform' must be passed as a string when 'size' is unspecified.",0)
            exit
        }
        __address=shader_vertex_uniform_get_address(argument[0])
        __size=shader_vertex_uniform_get_size(argument[0])
    }
    __gm82dx9_shader_vertex_uniform_matrix(__address,argument[1],__size)


#define shader_pixel_uniform_matrix
    ///shader_pixel_uniform_matrix(uniform,mtx_type,[size])
    var __address,__size;
    if (argument_count<=1 || argument_count>=3) {
        show_error("Error in function shader_pixel_uniform_matrix: wrong number of arguments.",0)
        exit
    }    
    if (argument_count==3) {
        __address=shader_pixel_uniform_get_address(argument[0])
        __size=argument[2]
    } else {
        if (!is_string(argument[0])) {
            show_error("Error in function shader_pixel_uniform_matrix: 'uniform' must be passed as a string when 'size' is unspecified.",0)
            exit
        }
        __address=shader_pixel_uniform_get_address(argument[0])
        __size=shader_pixel_uniform_get_size(argument[0])
    }
    __gm82dx9_shader_pixel_uniform_matrix(__address,argument[1],__size)


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


#define shader_vertex_uniform_f_buffer
    ///shader_vertex_uniform_f_buffer(uniform,buffer)
    __gm82dx9_shader_vertex_uniform_f_buffer(shader_vertex_uniform_get_address(argument0),buffer_get_address(argument1,0),buffer_get_size(argument1))


#define shader_pixel_uniform_f_buffer
    ///shader_pixel_uniform_f_buffer(uniform,buffer)
    __gm82dx9_shader_pixel_uniform_f_buffer(shader_vertex_uniform_get_address(argument0),buffer_get_address(argument1,0),buffer_get_size(argument1))


#define shader_vertex_uniform_i_buffer
    ///shader_vertex_uniform_i_buffer(uniform,buffer)
    __gm82dx9_shader_vertex_uniform_i_buffer(shader_vertex_uniform_get_address(argument0),buffer_get_address(argument1,0),buffer_get_size(argument1))


#define shader_pixel_uniform_i_buffer
    ///shader_pixel_uniform_i_buffer(uniform,buffer)
    __gm82dx9_shader_pixel_uniform_i_buffer(shader_vertex_uniform_get_address(argument0),buffer_get_address(argument1,0),buffer_get_size(argument1))


#define shader_vertex_uniform_b_buffer
    ///shader_vertex_uniform_b_buffer(uniform,buffer)
    __gm82dx9_shader_vertex_uniform_b_buffer(shader_vertex_uniform_get_address(argument0),buffer_get_address(argument1,0),buffer_get_size(argument1))


#define shader_pixel_uniform_b_buffer
    ///shader_pixel_uniform_b_buffer(uniform,buffer)
    __gm82dx9_shader_pixel_uniform_b_buffer(shader_vertex_uniform_get_address(argument0),buffer_get_address(argument1,0),buffer_get_size(argument1))

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
    __gm82dx9_texture_stage_set(shader_pixel_uniform_get_address(argument0),argument1)


#define texture_set_stage_interpolation
    ///texture_set_stage_interpolation(sampler,texture)
    __gm82dx9_texture_set_stage_interpolation(shader_pixel_uniform_get_address(argument0),argument1)


#define texture_set_stage_repeat
    ///texture_set_stage_repeat(sampler,hrepeat,vrepeat,bordercolor)
    __gm82dx9_texture_set_stage_repeat(shader_pixel_uniform_get_address(argument0),argument1,argument2,argument3)


#define texture_set_stage_vertex
    ///texture_set_stage_vertex(sampler,texture)
    __gm82dx9_texture_stage_vertex_set(shader_vertex_uniform_get_address(argument0),argument1)


#define texture_set_stage_vertex_interpolation
    ///texture_set_stage_vertex_interpolation(sampler,texture)
    __gm82dx9_texture_set_stage_vertex_interpolation(shader_vertex_uniform_get_address(argument0),argument1)


#define texture_set_stage_vertex_repeat
    ///texture_set_stage_vertex_repeat(sampler,hrepeat,vrepeat,bordercolor)
    __gm82dx9_texture_set_stage_vertex_repeat(shader_vertex_uniform_get_address(argument0),argument1,argument2,argument3)


#define shader_draw_shadertoy
    ///shader_draw_shadertoy(shader,x,y,w,h,time)
    
    shader_vertex_set_passthrough()

    shader_pixel_set(argument0)
    if (shader_pixel_uniform_exists("_iGlobalTime")) shader_pixel_uniform_f("_iGlobalTime",argument5)
    if (shader_pixel_uniform_exists("_iTime")) shader_pixel_uniform_f("_iTime",argument5)
    if (shader_pixel_uniform_exists("_iResolution")) shader_pixel_uniform_f("_iResolution",argument3,argument4,0)
    if (shader_pixel_uniform_exists("_iMouse")) shader_pixel_uniform_f("_iMouse",x,y,0,0)
    if (shader_pixel_uniform_exists("_iFrameRate")) shader_pixel_uniform_f("_iFrameRate",room_speed)

    draw_primitive_begin_texture(pr_trianglestrip,-1)
    draw_vertex_texture(argument1,argument2,0,1)
    draw_vertex_texture(argument1+argument3,argument2,1,1)
    draw_vertex_texture(argument1,argument2+argument4,0,0)
    draw_vertex_texture(argument1+argument3,argument2+argument4,1,0)
    draw_primitive_end()
    
    shader_reset()


#define surface_set_target_ext
    ///surface_set_target_ext(target,surface)
    var __surf;
    if (is_string(argument1)) {
        if (!ds_map_exists(__gm82dx9_surfmap,argument1)) {
            show_error("Error trying to set target to unknown surface: "+string(argument1),0)
            return noone
        }
        __surf=ds_map_find_value(__gm82dx9_surfmap,argument1)-1
    } else __surf=argument1
    
    if (!surface_exists(__surf)) {
        show_error("Trying to set target to nonexisting surface("+string(argument1)+")",0)
        return 0
    }
    if (argument0==0) surface_set_target(__surf)
    else __gm82dx9_surface_set_target_ext(argument0,__surf)


#define surface_reset_target_ext
    ///surface_reset_target_ext(target)
    if (argument0==0) surface_reset_target()
    else __gm82dx9_surface_set_target_ext(argument0,-1)


#define index_buffer_create
    ///index_buffer_create(buffer,ib_format_...)
    __gm82dx9_index_create_buffer_from_buffer(buffer_get_address(argument0,0),buffer_get_size(argument0),argument1)


#define vertex_buffer_create
    ///vertex_buffer_create(buffer,format,slot)
    __gm82dx9_vertex_create_buffer_from_buffer(buffer_get_address(argument0,0),buffer_get_size(argument0),vertex_format_get_size(argument1,argument2))


#define vertex_format_add_position
    ///vertex_format_add_position(slot)
    vertex_format_add_custom(vf_type_float4,vf_usage_position,argument0)


#define vertex_format_add_texcoord
    ///vertex_format_add_texcoord(slot)
    vertex_format_add_custom(vf_type_float2,vf_usage_texcoord,argument0)


#define vertex_format_add_color
    ///vertex_format_add_color(slot)
    vertex_format_add_custom(vf_type_d3dcolor,vf_usage_color,argument0)


#define vertex_format_add_normal
    ///vertex_format_add_normal(slot)
    vertex_format_add_custom(vf_type_float4,vf_usage_normal,argument0)


#define buffer_write_format_default
    ///buffer_write_format_default(buffer,x,y,z,nx,ny,nz,u,v,color,alpha)
    buffer_write_float4(argument0,argument1,argument2,argument3,0)
    
    buffer_write_float4(argument0,argument4,argument5,argument6,0)
    
    buffer_write_u32(argument0,color_to_d3dcolor(argument9,argument10))
    
    buffer_write_float2(argument0,argument7,argument8)


#define buffer_write_format_position
    ///buffer_write_format_position(buffer,x,y,z)
    buffer_write_float4(argument0,argument1,argument2,argument3,0)


#define buffer_write_format_texcoord
    ///buffer_write_format_texcoord(buffer,u,v)
    buffer_write_float2(argument0,argument1,argument2)


#define buffer_write_format_color
    ///buffer_write_format_color(buffer,color,alpha)
    buffer_write_u32(argument0,color_to_d3dcolor(argument1,argument2))


#define vertex_format_create_simple
    ///vertex_format_create_simple(vf_usage_...)
    vertex_format_begin()
        i=0 repeat (argument_count) {
            if (argument[i]==vf_usage_position) vertex_format_add_position(0)
            else if (argument[i]==vf_usage_texcoord) vertex_format_add_texcoord(0)
            else if (argument[i]==vf_usage_normal) vertex_format_add_normal(0)
            else if (argument[i]==vf_usage_color) vertex_format_add_color(0)
            else show_error("invalid usage semantic passed to vertex_format_simple",0)
        i+=1}
    return vertex_format_end()


#define vertex_format_create_default
    ///vertex_format_create_default()
    vertex_format_begin()
        vertex_format_add_position(0)
        vertex_format_add_normal(0)
        vertex_format_add_color(0)
        vertex_format_add_texcoord(0)    
    return vertex_format_end()


#define vertex_buffer_draw
    ///vertex_buffer_draw(vbuffer,vformat,primitive,texture,[ibuffer])
    var __vertices,__indexed;
    
    if (argument_count>4) {
        __gm82dx9_index_buffer_set(argument4)
        if (__gm82dx9_index_buffer_get_format(argument4)==ib_format_32) __vertices=__gm82dx9_index_buffer_get_size(argument4)/4
        else __vertices=__gm82dx9_index_buffer_get_size(argument4)/2
        __indexed=true
    } else {
        __vertices=vertex_buffer_get_size(argument0)/vertex_format_get_size(argument1,0)
        __indexed=false
    }
    
    __gm82dx9_vertex_draw_buffer(argument0,argument1,argument2,argument3,__vertices,__indexed)


#define vertex_instance_set
    ///vertex_instance_set(instbuffer,format,slot)

    __gm82dx9_vertex_set_instance_count(vertex_buffer_get_size(argument0)/vertex_format_get_size(argument1,argument2))
    __gm82dx9_vertex_set_instances_per_vertex(1,1)
    
    vertex_buffer_bind(argument0,argument2)

//
//