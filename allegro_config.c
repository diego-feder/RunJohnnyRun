#include "global.h"

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	set_window_title("RUN JOHNNY RUN!");
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, X_MAX, Y_MAX, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}
	install_timer();
	install_keyboard();
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0);
	/* add other initializations here */
}

void deinit() {
	clear_keybuf();
	/* add other deinitializations here */


}
