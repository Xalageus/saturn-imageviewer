#include <jo/jo.h>
#include "gallery.h"
#include "menu.c"
#include <stdio.h>
#include <math.h>

//const float DEFAULT_ZOOM = 0.85
const int DEFAULT_ZOOM = 200;
const int DEFAULT_X_POS = 0;
const int DEFAULT_Y_POS = 0;
//const float MIN_ZOOM = 0
//const float MAX_ZOOM = 6.39
const int MIN_ZOOM = 40;
const int MAX_ZOOM = 1000;

const float DEFAULT_FALLBACK_ZOOM = 1;
const float MIN_FALLBACK_ZOOM = 0;
const float MAX_FALLBACK_ZOOM = 8;

int image = 0;
int max_image = 0;
int zoom_level = 200;
float fallback_zoom = 1;
bool pan_mode = false;
int x_pos = 0;
int y_pos = 0;
bool enable_menu = false;
bool load_failed = false;

jo_palette image_pal;

void clear_prints(){
	jo_printf(0, 0, "                ");
	jo_printf(0, 1, "                   ");
	jo_printf(0, 2, "                ");
	jo_printf(0, 3, "                ");
	jo_printf(0, 5, "           ");
	jo_printf(0, 6, "          ");
	jo_printf(12, 28, "           ");
	jo_printf(2, 28, "          ");
	jo_printf(35, 28, "    ");
}

void draw_failed_msg(){
	jo_printf(12, 28, "Load failed");
}

void sys_draw(void){
	if(enable_menu){
		menu_draw();
	}else{
		clear_prints();

		if(draw_debug){
			double fzoomint;
			double fzoomsub = modf(fallback_zoom, &fzoomint);
			jo_printf(0, 0, "zoom_level=%d", zoom_level);
			jo_printf(0, 1, "fallback_zoom=%d.%d", (int)fzoomint, (int)(fzoomsub * 100));
			jo_printf(0, 2, "x_pos=%d", x_pos);
			jo_printf(0, 3, "y_pos=%d", y_pos);
			jo_printf(0, 5, "height=%d", jo_sprite_get_height(0));
			jo_printf(0, 6, "width=%d", jo_sprite_get_width(0));
		}

		jo_printf(2, 28, "%d/%d", image + 1, max_image);
		jo_printf(35, 28, "%d%%", jo_sprite_usage_percent());
	}

	if(fallback_drawing && !load_failed){
		jo_sprite_draw3D(0, x_pos, y_pos, 500);
	}else if(!load_failed){
		jo_3d_draw_sprite_at(0, x_pos, y_pos, zoom_level);
	}

	if(load_failed){
		draw_failed_msg();
	}
}

void load_image(int imageID){
	jo_img_8bits img;

	if(jo_sprite_count() > 0){
		if(!fallback_drawing){
			jo_3d_free_sprite_quad(0);
		}
		
		jo_sprite_free_all();
	}
	
	load_failed = false;
	img.data = JO_NULL;
	int err = jo_tga_8bits_loader(&img, "ART", imageList[imageID], 0);
	jo_sprite_add_8bits_image(&img);
	jo_sprite_set_palette(image_pal.id);
	jo_free_img(&img);
	//int fail = jo_sprite_add_tga("ART", imageList[imageID], 0);
	//if(fail == -1){
	//	load_failed = true;
	//}
	if(err != 0){
		load_failed = true;
	}

	//Palette handler seems to change printf color, therefore we reset printf color to white
	jo_set_printf_palette_color(JO_COLOR_INDEX_White, JO_COLOR_White);
	jo_set_printf_color_index(JO_COLOR_INDEX_White);
}

void draw_loading(){
	jo_printf(12, 28, "Loading... ");
}

void sys_events(void){
	if(enable_menu){
		menu_events();

		if(menu_disabling){
			cleanup_menu();
			jo_sprite_change_sprite_scale(fallback_zoom);
			enable_menu = false;
		}
	}
}

void sys_pad(void){
	if(enable_menu){
		menu_pad();
	}else{
		//Toggle (by holding) panning an image
		if(jo_is_pad1_key_pressed(JO_KEY_A)){
			pan_mode = true;
		}else{
			pan_mode = false;
		}

		//Pan to the right (reversed)
		if(jo_is_pad1_key_pressed(JO_KEY_LEFT)){
			if(pan_mode){
				x_pos++;
			}
		}

		//Pan to the left (reversed)
		if(jo_is_pad1_key_pressed(JO_KEY_RIGHT)){
			if(pan_mode){
				x_pos--;
			}
		}

		//Pan up
		if(jo_is_pad1_key_pressed(JO_KEY_UP)){
			if(pan_mode){
				y_pos++;
			}
		}

		//Pan down
		if(jo_is_pad1_key_pressed(JO_KEY_DOWN)){
			if(pan_mode){
				y_pos--;
			}
		}

		//Load previous image
		if(jo_is_pad1_key_down(JO_KEY_LEFT)){
			if(!pan_mode){
				if(image == 0){
					image = max_image - 1;

				}else{
					image--;
				}

				draw_loading();
				load_image(image);
			}
		//Load next image
		}else if(jo_is_pad1_key_down(JO_KEY_RIGHT)){
			if(!pan_mode){
				if(image == max_image - 1){
					image = 0;
				}else{
					image++;
				}

				draw_loading();
				load_image(image);
			}
		//Go back 10 images
		}else if(jo_is_pad1_key_down(JO_KEY_DOWN)){
			if(!pan_mode){
				image = image - 10;
				if(image < 0){
					image = max_image - 1;
				}

				draw_loading();
				load_image(image);
			}
		//Go forward 10 images
		}else if(jo_is_pad1_key_down(JO_KEY_UP)){
			if(!pan_mode){
				image = image + 10;
				if(image > max_image - 1){
					image = 0;
				}

				draw_loading();
				load_image(image);
			}
		}

		//Zoom out
		if(jo_is_pad1_key_pressed(JO_KEY_B)){
			if(fallback_drawing){
				fallback_zoom -= 0.01f;
				if(fallback_zoom < MIN_FALLBACK_ZOOM){
					fallback_zoom = MIN_FALLBACK_ZOOM;
				}

				jo_sprite_change_sprite_scale(fallback_zoom);
			}else{
				zoom_level += 1;
				if(zoom_level > MAX_ZOOM){
					zoom_level = MAX_ZOOM;
				}
			}
			
		//Zoom in
		}else if(jo_is_pad1_key_pressed(JO_KEY_C)){
			if(fallback_drawing){
				fallback_zoom += 0.01f;
				if(fallback_zoom > MAX_FALLBACK_ZOOM){
					fallback_zoom = MAX_FALLBACK_ZOOM;
				}

				jo_sprite_change_sprite_scale(fallback_zoom);
			}else{
				zoom_level -= 1;
				if(zoom_level < MIN_ZOOM){
					zoom_level = MIN_ZOOM;
				}
			}
		}

		//Reset zoom and pan
		if(jo_is_pad1_key_down(JO_KEY_X)){
			zoom_level = DEFAULT_ZOOM;
			x_pos = DEFAULT_X_POS;
			y_pos = DEFAULT_Y_POS;

			fallback_zoom = DEFAULT_FALLBACK_ZOOM;
			jo_sprite_change_sprite_scale(fallback_zoom);
		}
	}
	
	//Toggle menu
	if(jo_is_pad1_key_down(JO_KEY_START)){
		if(enable_menu){
			cleanup_menu();
			jo_sprite_change_sprite_scale(fallback_zoom);
			enable_menu = false;
		}else{
			enable_menu = true;
			jo_sprite_change_sprite_scale(DEFAULT_FALLBACK_ZOOM);
			clear_prints();
			init_menu();
		}
	}
}

jo_palette * init_tga_palette_handling(void){
	jo_create_palette(&image_pal);
	return &image_pal;
}

void jo_main(void){
	jo_core_init(JO_COLOR_Black);
	jo_set_tga_palette_handling(init_tga_palette_handling);
	max_image = get_image_count();
	draw_loading();
	load_image(image);
	jo_core_add_callback(sys_draw);
	jo_core_add_callback(sys_pad);
	jo_core_add_callback(sys_events);
	jo_core_run();
}
