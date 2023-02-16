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
    __gm82dx9_shader_vertex_matrix_wvp(0)
    

#define shader_pixel_set_passthrough
    ///shader_pixel_set_passthrough()
    shader_pixel_set(__gm82dx9_default_ps)


#define shader_vertex_uniform_f
    ///shader_vertex_uniform_f(reg,...)
    var __i; __i=0
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_vertex_uniform_4f(argument[0]+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_vertex_uniform_4f(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_vertex_uniform_4f(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_vertex_uniform_4f(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_pixel_uniform_f
    ///shader_pixel_uniform_f(reg,...)
    var __i; __i=0
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_pixel_uniform_4f(argument[0]+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_pixel_uniform_4f(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_pixel_uniform_4f(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_pixel_uniform_4f(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_vertex_uniform_i
    ///shader_vertex_uniform_i(reg,...)
    var __i; __i=0
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_vertex_uniform_4i(argument[0]+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_vertex_uniform_4i(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_vertex_uniform_4i(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_vertex_uniform_4i(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_pixel_uniform_i
    ///shader_pixel_uniform_i(reg,...)
    var __i; __i=0
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_pixel_uniform_4i(argument[0]+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_pixel_uniform_4i(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_pixel_uniform_4i(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_pixel_uniform_4i(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_vertex_uniform_b
    ///shader_vertex_uniform_b(reg,...)
    var __i; __i=0
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_vertex_uniform_4b(argument[0]+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_vertex_uniform_4b(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_vertex_uniform_4b(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_vertex_uniform_4b(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_pixel_uniform_b
    ///shader_pixel_uniform_b(reg,...)
    var __i; __i=0
    do switch (argument_count-__i) {
        case 0: case 1: exit
        case 2: __gm82dx9_shader_pixel_uniform_4b(argument[0]+(__i div 4),argument[1+__i],0,0,0) exit
        case 3: __gm82dx9_shader_pixel_uniform_4b(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],0,0) exit
        case 4: __gm82dx9_shader_pixel_uniform_4b(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],0) exit
        default:
            __gm82dx9_shader_pixel_uniform_4b(argument[0]+(__i div 4),argument[1+__i],argument[2+__i],argument[3+__i],argument[4+__i])
            __i+=4
    } until (__i <= 0)


#define shader_vertex_uniform_matrix
    //shader_vertex_uniform_matrix(reg,mtx_type)
    switch (argument1) {
        case mtx_world: __gm82dx9_shader_vertex_matrix_w(argument0) break
        case mtx_view: __gm82dx9_shader_vertex_matrix_v(argument0) break
        case mtx_projection: __gm82dx9_shader_vertex_matrix_p(argument0) break
        case mtx_world_view: __gm82dx9_shader_vertex_matrix_wv(argument0) break
        case mtx_world_view_projection: __gm82dx9_shader_vertex_matrix_wvp(argument0) break
    }
//
//