#include <jo/jo.h>
#include "menus/about.c"

int cursor_line = 0;
int max_line = 3;
bool menu_disabling = false;
bool enable_about = false;

//Menu controlled Main vars
bool draw_debug = false;
bool fallback_drawing = false;

void clear_menu(){
    jo_printf(7, 6, "                        ");
    jo_printf(7, 7, "                        ");
    jo_printf(7, 8, "                        ");
    jo_printf(7, 9, "                        ");
}

void draw_menu_background(){
    for(int i = 0; i < 150; i += 8){
        for(int j = 0; j < 190; j += 8){
            jo_sprite_draw3D(1, -100 + j, -80 + i, 50);
        }
    }
}

void menu_draw(){
    if(enable_about){
        about_draw();
    }else{
        draw_menu_background();
        clear_menu();

        jo_printf(10, 6, "Fallback drawing");
        if(fallback_drawing){
            jo_printf(29, 6, "On");
        }else{
            jo_printf(28, 6, "Off");
        }

        jo_printf(10, 7, "Show debug");
        if(draw_debug){
            jo_printf(29, 7, "On");
        }else{
            jo_printf(28, 7, "Off");
        }

        jo_printf(10, 8, "About");
        jo_printf(10, 9, "Exit to BIOS");

        jo_printf(7, 6 + cursor_line, ">>");
    }
}

void select_item(int line){
    if(line == 0){
        if(fallback_drawing){
            fallback_drawing = false;
        }else{
            fallback_drawing = true;
        }
    } else if(line == 1){
        if(draw_debug){
            draw_debug = false;
        }else{
            draw_debug = true;
        }
    } else if(line == 2){
        enable_about = true;
        clear_menu();
    } else if(line == 3){
        jo_goto_boot_menu();
    }
}

void menu_events(){
    if(enable_about){
        if(about_disabling){
            cleanup_about();
            enable_about = false;
        }
    }
}

void menu_pad(){
    if(enable_about){
        about_pad();
    }else{
        if(jo_is_pad1_key_down(JO_KEY_UP)){
            if(cursor_line != 0){
                cursor_line--;
            }
        }else if(jo_is_pad1_key_down(JO_KEY_DOWN)){
            if(cursor_line != max_line){
                cursor_line++;
            }
        }

        if(jo_is_pad1_key_down(JO_KEY_A)){
            select_item(cursor_line);
        }else if(jo_is_pad1_key_down(JO_KEY_B)){
            menu_disabling = true;
        }
    }
    
}

void init_menu(){
    jo_sprite_add_tga("TEX", "BACK.TGA", JO_COLOR_Transparent);
}

void cleanup_menu(){
    clear_menu();
    jo_sprite_free_from(1);
    menu_disabling = false;

    if(enable_about){
        cleanup_about();
        enable_about = false;
    }
}