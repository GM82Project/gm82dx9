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
    shader_vertex_uniform_matrix("rMatrixWVP",mtx_world_view_projection)
    

#define shader_pixel_set_passthrough
    ///shader_pixel_set_passthrough()
    shader_pixel_set(__gm82dx9_default_ps)


#define shader_vertex_uniform_f
    ///shader_vertex_uniform_f(uniform,...)
    var __i,__uni; __i=0
    __uni=__gm82dx9_get_vconst(argument0)
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_vertex_uniform_4f(__uni+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_vertex_uniform_4f(__uni+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_vertex_uniform_4f(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_vertex_uniform_4f(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_pixel_uniform_f
    ///shader_pixel_uniform_f(uniform,...)
    var __i,__uni; __i=0
    __uni=__gm82dx9_get_pconst(argument0)
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_pixel_uniform_4f(__uni+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_pixel_uniform_4f(__uni+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_pixel_uniform_4f(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_pixel_uniform_4f(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_vertex_uniform_i
    ///shader_vertex_uniform_i(uniform,...)
    var __i,__uni; __i=0
    __uni=__gm82dx9_get_vconst(argument0)
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_vertex_uniform_4i(__uni+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_vertex_uniform_4i(__uni+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_vertex_uniform_4i(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_vertex_uniform_4i(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_pixel_uniform_i
    ///shader_pixel_uniform_i(uniform,...)
    var __i,__uni; __i=0
    __uni=__gm82dx9_get_pconst(argument0)
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_pixel_uniform_4i(__uni+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_pixel_uniform_4i(__uni+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_pixel_uniform_4i(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_pixel_uniform_4i(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_vertex_uniform_b
    ///shader_vertex_uniform_b(uniform,...)
    var __i,__uni; __i=0
    __uni=__gm82dx9_get_vconst(argument0)
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_vertex_uniform_4b(__uni+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_vertex_uniform_4b(__uni+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_vertex_uniform_4b(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_vertex_uniform_4b(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_pixel_uniform_b
    ///shader_pixel_uniform_b(uniform,...)
    var __i,__uni; __i=0
    __uni=__gm82dx9_get_pconst(argument0)
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_pixel_uniform_4b(__uni+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_pixel_uniform_4b(__uni+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_pixel_uniform_4b(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_pixel_uniform_4b(__uni+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_vertex_uniform_matrix
    //shader_vertex_uniform_matrix(uniform,mtx_type)
    var __uni;
    __uni=__gm82dx9_get_vconst(argument0)
    switch (argument1) {
        case mtx_world: __gm82dx9_shader_vertex_matrix_w(__uni) break
        case mtx_view: __gm82dx9_shader_vertex_matrix_v(__uni) break
        case mtx_projection: __gm82dx9_shader_vertex_matrix_p(__uni) break
        case mtx_world_view: __gm82dx9_shader_vertex_matrix_wv(__uni) break
        case mtx_world_view_projection: __gm82dx9_shader_vertex_matrix_wvp(__uni) break
    }
//
//