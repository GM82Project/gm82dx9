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


#define shader_vertex_uniform_f
    ///shader_vertex_uniform_f(uniform,...)
    var __i,__uni; __i=0
    __uni=argument0 if (is_string(__uni)) __uni=shader_vertex_uniform_get_address(__uni)
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
    __uni=argument0 if (is_string(__uni)) __uni=shader_pixel_uniform_get_address(__uni)
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
    __uni=argument0 if (is_string(__uni)) __uni=shader_vertex_uniform_get_address(__uni)
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
    __uni=argument0 if (is_string(__uni)) __uni=shader_pixel_uniform_get_address(__uni)
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
    __uni=argument0 if (is_string(__uni)) __uni=shader_vertex_uniform_get_address(__uni)
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
    __uni=argument0 if (is_string(__uni)) __uni=shader_pixel_uniform_get_address(__uni)
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: case 3: case 4: __gm82dx9_shader_pixel_uniform_b(__uni+__i,argument[1+__i]) __i+=1 continue
        default:
            __gm82dx9_shader_pixel_uniform_4b(__uni+__i,argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (false)


#define shader_vertex_uniform_matrix
    //shader_vertex_uniform_matrix(uniform,mtx_type)
    var __uni;
    __uni=argument0 if (is_string(__uni)) __uni=shader_vertex_uniform_get_address(__uni)
    __gm82dx9_shader_vertex_uniform_matrix(__uni,argument1)


#define shader_pixel_uniform_matrix
    //shader_pixel_uniform_matrix(uniform,mtx_type)
    var __uni;
    __uni=argument0 if (is_string(__uni)) __uni=shader_pixel_uniform_get_address(__uni)
    __gm82dx9_shader_pixel_uniform_matrix(__uni,argument1)


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
    var __uni;
    __uni=argument0 if (is_string(__uni)) __uni=shader_pixel_uniform_get_address(__uni)
    __gm82dx9_texture_stage_set(__uni,argument1)


#define texture_set_stage_interpolation
    ///texture_set_stage_interpolation(sampler,texture)
    var __uni;
    __uni=argument0 if (is_string(__uni)) __uni=shader_pixel_uniform_get_address(__uni)
    __gm82dx9_texture_set_stage_interpolation(__uni,argument1)


#define texture_set_stage_repeat
    ///texture_set_stage_repeat(sampler,hrepeat,vrepeat,bordercolor)
    var __uni;
    __uni=argument0 if (is_string(__uni)) __uni=shader_pixel_uniform_get_address(__uni)
    __gm82dx9_texture_set_stage_repeat(__uni,argument1,argument2,argument3)


#define texture_set_stage_vertex
    ///texture_set_stage_vertex(sampler,texture)
    var __uni;
    __uni=argument0 if (is_string(__uni)) __uni=shader_vertex_uniform_get_address(__uni)
    __gm82dx9_texture_stage_vertex_set(__uni,argument1)


#define texture_set_stage_vertex_interpolation
    ///texture_set_stage_vertex_interpolation(sampler,texture)
    var __uni;
    __uni=argument0 if (is_string(__uni)) __uni=shader_vertex_uniform_get_address(__uni)
    __gm82dx9_texture_set_stage_vertex_interpolation(__uni,argument1)


#define texture_set_stage_vertex_repeat
    ///texture_set_stage_vertex_repeat(sampler,hrepeat,vrepeat,bordercolor)
    var __uni;
    __uni=argument0 if (is_string(__uni)) __uni=shader_vertex_uniform_get_address(__uni)
    __gm82dx9_texture_set_stage_vertex_repeat(__uni,argument1,argument2,argument3)
//
//