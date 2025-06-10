#define draw_quad
    ///draw_quad(x,y,w,h,tex,[u1,v1,u2,v2])
    //Draws a simple textured rectangle.
    //Useful for drawing effect shaders.
    
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


#define draw_quad_color
    ///draw_quad_color(x1,y1,c1,a1,x2,y2,c2,a2,x3,y3,c3,a3,x4,y4,c4,a4)
    //Draws a color quad with no texture.
    
    draw_primitive_begin_texture(pr_trianglestrip,__gm82core_pixel_tex)
    draw_vertex_texture_color(argument0 ,argument1 ,0.5,0.5,argument2 ,argument3 )
    draw_vertex_texture_color(argument4 ,argument5 ,0.5,0.5 ,argument6 ,argument7 )
    draw_vertex_texture_color(argument12,argument13,0.5,0.5,argument14,argument15)
    draw_vertex_texture_color(argument8 ,argument9 ,0.5,0.5 ,argument10,argument11)
    draw_primitive_end()


#define draw_tri
    ///draw_tri(x1,y1,u1,v1,x2,y2,u2,v2,x3,y3,u3,v3,tex,color,alpha)
    //Draws a textured triangle with blending.

    texture_set_repeat(1)
    draw_primitive_begin_texture(pr_trianglestrip,argument12)
    draw_vertex_texture_color(argument0 ,argument1 ,argument2 ,argument3 ,argument13,argument14)
    draw_vertex_texture_color(argument4 ,argument5 ,argument6 ,argument7 ,argument13,argument14)
    draw_vertex_texture_color(argument8 ,argument9 ,argument10,argument11,argument13,argument14) 
    draw_primitive_end()
//
//