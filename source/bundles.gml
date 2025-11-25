/*
    string "g3b model bundle v1"

    string "[TEX]"
    u16 count [
        u32 size
        (png file)
    ]

    string "[MDL]"
    u16 count [
        u16 texid
        u32 size
        deflate {
            g3d
        }
    ]
*/

#define d3d_model_bundle_create
    ///d3d_model_bundle_create()
    //Creates and returns an empty model bundle.

    var __bundle;

    __bundle=ds_map_create()

    ds_map_add(__bundle,"g3b model bundle",1)
    ds_map_add(__bundle,"texture",ds_list_create())
    ds_map_add(__bundle,"mesh",ds_list_create())
    ds_map_add(__bundle,"model_own",ds_list_create())

    return __bundle


#define d3d_model_bundle_add
    ///d3d_model_bundle_add(bundle,model,background)
    //Adds a model to a bundle, with a background texture.
    var __bundle,__mdl,__bg;

    __bundle=argument0
    __mdl=argument1
    __bg=argument2

    var __meshlist,__mesh;

    if (ds_map_find_value(__bundle,"g3b model bundle")==1) {//version 1
        __meshlist=ds_map_find_value(__bundle,"mesh")
        
        if (__bg<0 or !background_exists(__bg)) __bg=noone
        
        d3d_model_bake(__mdl)
        
        __mesh=ds_list_create()
        ds_list_add_many(__mesh,
            //model format v1
            __mdl, //model
            __bg, //background
            0 //background index in save list
        )      
        ds_list_add(__meshlist,__mesh)    
        
        return true
    }

    show_error("error in function d3d_model_bundle_add: invalid bundle id ("+string(__bundle)+")",0)
    return false


#define d3d_model_bundle_destroy
    ///d3d_model_bundle_destroy(bundle)
    //Destroys a bundle, freeing any resources associated with it.
    var __bundle,__mdl,__bg;

    __bundle=argument0

    var __meshlist,__texlist,__i,__mesh;

    if (ds_map_find_value(__bundle,"g3b model bundle")==1) {//version 1
        __mesh_own_list=ds_map_find_value(__bundle,"model_own")
        __meshlist=ds_map_find_value(__bundle,"model")
        __texlist=ds_map_find_value(__bundle,"texture")
        
        __i=0 repeat (ds_list_size(__meshlist)) {
            ds_list_destroy(ds_list_find_value(__meshlist,__i) )
        __i+=1}
        ds_list_destroy(__meshlist)
        
        __i=0 repeat (ds_list_size(__mesh_own_list)) {
            d3d_model_destroy(ds_list_find_value(__mesh_own_list,__i))
        __i+=1}
        ds_list_destroy(__mesh_own_list)     
        
        __i=0 repeat (ds_list_size(__texlist)) {
            background_delete(ds_list_find_value(__texlist,__i))
        __i+=1}
        ds_list_destroy(__texlist)
        
        ds_map_destroy(__bundle)
        
        return true
    }

    show_error("error in function d3d_model_bundle_destroy: invalid bundle id ("+string(__bundle)+")",0)
    return false


#define d3d_model_bundle_save
    ///d3d_model_bundle_save(bundle,filename)
    //Saves a model bundle to disk.
    var __bundle,__fn,__mdl,__bg;

    __bundle=argument0
    __fn=argument1

    __tmpfn=temp_directory+"\gm82\bundle_texture.png"
    __tmpmd=temp_directory+"\gm82\bundle_model.g3d"

    if (ds_map_find_value(__bundle,"g3b model bundle")==1) {//version 1
        __meshlist=ds_map_find_value(__bundle,"mesh")
        __texlist=ds_map_find_value(__bundle,"texture")

        __b=buffer_create()
        buffer_write_string(__b,"g3b model bundle v1")
        
        //first we need to make a master list of all involved backgrounds
        __bglist=ds_list_create()
        __i=0 repeat (ds_list_size(__meshlist)) {
            __mesh=ds_list_find_value(__meshlist,__i)
            __bg=ds_list_find_value(__mesh,1)
            if (__bg!=-1) if (!ds_list_find_index(__bglist,__bg)) ds_list_add(__bglist,__bg)
        __i+=1}
        
        //texture block
        buffer_write_string(__b,"[TEX]")
        buffer_write_u16(__b,ds_list_size(__bglist)) //count
        
        __i=0 repeat (ds_list_size(__bglist)) {
            background_save(ds_list_find_value(__bglist,__i),__tmpfn)
            __load=buffer_create(__tmpfn)
            buffer_write_u32(__b,buffer_get_size(__load)) //size
            buffer_copy(__b,__load) //png file
            buffer_destroy(__load)
        __i+=1}     
        
        
        //now we need to add data to each mesh, about which bg id its using off this list...      
        __i=0 repeat (ds_list_size(__meshlist)) {
            __mesh=ds_list_find_value(__meshlist,__i)
            __bg=ds_list_find_value(__mesh,1)
            
            if (bg!=-1)
            __index=ds_list_find_index(__bglist,__bg)
            ds_list_replace(__mesh,2,__index)
        __i+=1}
        ds_list_destroy(__bglist)
        
        //model block                                                      
        buffer_write_string(__b,"[MDL]")
        buffer_write_u16(__b,ds_list_size(__meshlist)) //count
        
        __i=0 repeat (ds_list_size(__meshlist)) {
            __mesh=ds_list_find_value(__meshlist,__i)
            __mdl=ds_list_find_value(__mesh,0)
            
            d3d_model_save(__mdl,__tmpmd)        
            if (file_exists(__tmpmd)) {        
                buffer_write_u16(__b,ds_list_find_value(__mesh,2)) //texid
                
                __load=buffer_create(__tmpmd)
                __str=buffer_read_data(__load,buffer_get_size(__load))
                buffer_clear(__load)
                buffer_write_data(__load,string_replace_all(__str,",","."))
                buffer_deflate(__load)
                buffer_write_u32(__b,buffer_get_size(__load)) //size
                buffer_copy(__b,__load) //deflate {g3d}
                buffer_destroy(__load) 
            }
        __i+=1}
        
        buffer_save(__b,__fn)
        
        buffer_destroy(__b)
        
        return true
    }

    show_error("error in function d3d_model_bundle_save: invalid bundle id ("+string(__bundle)+")",0)
    return false


#define d3d_model_bundle_load
    ///d3d_model_bundle_load(filename)
    //Loads a model bundle from disk.
    var __fn,__b,__mdl;

    __fn=string(argument0)
    
    if (!file_exists(__fn)) {
        show_error("error in function d3d_model_bundle_load: file doesn't exist ("+__fn+")",0)
        return noone
    }
    
    __b=buffer_create(__fn)
    __mdl=d3d_model_bundle_load_buffer(__b)
    buffer_destroy(__b)
    
    return __mdl


#define d3d_model_bundle_load_buffer
    ///d3d_model_bundle_load_buffer(buffer)
    //Loads a model bundle from a buffer.
    var __mdl,__bg,__bundle,__tmpfn,__b;

    __b=argument0
    
    __tmpfn=temp_directory+"\gm82\bundle_texture.png"
    
    __bundle=d3d_model_bundle_create()
    
    __ver=buffer_read_string(__b)

    if (__ver=="g3b model bundle v1") {
        __meshlist=ds_map_find_value(__bundle,"mesh")
        __mesh_own_list=ds_map_find_value(__bundle,"mesh_own")
        __texlist=ds_map_find_value(__bundle,"texture")
        
        //textures
        if (buffer_read_string(__b)!="[TEX]") {
            buffer_destroy(__b)
            d3d_model_bundle_destroy(__bundle)
            show_error("error in function d3d_model_bundle_load: no texture block",0)
            return noone
        }
        
        __count=buffer_read_u16(__b) //count
        repeat (__count) {
            __size=buffer_read_u32(__b) //size
            __tmp=buffer_create()
            buffer_copy_part(__tmp,__b,buffer_get_pos(__b),__size) //data
            buffer_set_pos(__b,buffer_get_pos(__b)+__size)
            buffer_save(__tmp,__tmpfn)
            __bg=background_add(__tmpfn,0,0)
            ds_list_add(__texlist,__bg)
            buffer_destroy(__tmp)
        }
        
        //models
        if (buffer_read_string(__b)!="[MDL]") {
            buffer_destroy(__b)
            d3d_model_bundle_destroy(__bundle)
            show_error("error in function d3d_model_bundle_load: no model block",0)
            return noone
        }
        
        __count=buffer_read_u16(__b) //count
        repeat (__count) {
            __tex=buffer_read_u16(__b) //texid
            __bg=ds_list_find_value(__texlist,__tex)
            
            __size=buffer_read_u32(__b) //size
            __tmp=buffer_create()
            buffer_copy_part(__tmp,__b,buffer_get_pos(__b),__size) //data
            buffer_set_pos(__b,buffer_get_pos(__b)+__size)
            
            buffer_inflate(__tmp)
            __mdl=d3d_model_create()
            d3d_model_load(__mdl,buffer_make_pipe(__tmp))
            d3d_model_bake(__mdl)
            
            __mesh=ds_list_create()
            ds_list_add_many(__mesh,
                //model format v1
                __mdl, //model
                __bg, //background
                0 //background index in save list
            )      
            ds_list_add(__meshlist,__mesh)   
            ds_list_add(__mesh_own_list,__mdl)
            buffer_destroy(__tmp)
        }
        
        return __bundle
    }

    show_error("error loading bundle: invalid bundle version ("+string(__ver)+")",0)
    return noone


#define d3d_model_bundle_draw
    ///d3d_model_bundle_draw(bundle)
    //Draws a model bundle.
    var __bundle,__i,__meshlist;

    __bundle=argument0

    var __meshlist,__i,__mesh,__mtl;
    
    texture_set_repeat(1)

    if (ds_map_find_value(__bundle,"g3b model bundle")==1) {//version 1
        __meshlist=ds_map_find_value(__bundle,"mesh")
        
        __i=0 repeat (ds_list_size(__meshlist)) {
            __mesh=ds_list_find_value(__meshlist,__i)
            __mtl=ds_list_find_value(__mesh,1)
            if (__mtl>=0) __mtl=background_get_texture(__mtl) else __mtl=-1
            d3d_model_draw(ds_list_find_value(__mesh,0),0,0,0,__mtl)
        __i+=1}

        return true
    }

    show_error("error in function d3d_model_bundle_draw: invalid bundle id ("+string(__bundle)+")",0)
    return false
//
//