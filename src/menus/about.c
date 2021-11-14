#include <jo/jo.h>

bool about_disabling = false;
bool first_draw = true;

void clear_about(){
    jo_printf(5, 5, "                  ");
    jo_printf(5, 6, "             ");
    jo_printf(5, 8, "             ");
}

void draw_about_background(){
    //Assuming Menu is already open, we can use
    //BACK.TGA with sprite id 1
    for(int i = 0; i < 234; i += 8){
        for(int j = 0; j < 314; j += 8){
            jo_sprite_draw3D(1, -157 + j, -117 + i, 50);
        }
    }
}

void about_draw(){
    draw_about_background();

    if(first_draw){
        jo_printf(5, 5, "Saturn ImageViewer");
        jo_printf(5, 6, "Version 1.1.1");
        jo_printf(5, 8, "2021 Xalageus");

        first_draw = false;
    }
}

void about_pad(){
    if(jo_is_pad1_key_down(JO_KEY_B)){
        about_disabling = true;
    }
}

void cleanup_about(){
    clear_about();
    about_disabling = false;
    first_draw = true;
}