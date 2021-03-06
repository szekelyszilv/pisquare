#include "menu.h"

#include <stdio.h>

#include "player.h"
#include "text.h"
#include "draw.h"
#include "io.h"

scene_t *menu_scene(void)
{
	scene_t *s = malloc(sizeof(scene_t));

	s->init = &menu_init;
	s->update = &menu_update;
	s->draw = &menu_draw;

	return s;
}

void menu_init(game_state_t *state)
{
	(void)state;
}

int menu_update(game_state_t *state)
{
	if (state->p1_right_clicked) {
		io_handle_click_right(state);
		return 0;
	}


	if (state->p1_left_held) {
		io_handle_hold_left(state);
		state->player_count = (state->player_count) % 4 + 1;
	}

	return 1;
}

void menu_draw(game_state_t *state)
{
	char player_num[8];

	draw_background(state);

	color_t col = (color_t){0, 0, 0, 255};

	/* graphics_background_color(state->device, &col); */

	print_text(state, "MENU",
			&(vector2_t){221, 161});
	print_text(state, "L : SELECT    R : START",
			&(vector2_t){36, 484});

	for (int i = 0; i < 4; i++) {
		col = player_number_color(i + 1);
		sprintf(player_num, "%d PLAYER", i + 1);
		print_text_color(state, player_num,
				&(vector2_t){181, 201 + 30 * i},
					&(color_t){255, 255, 255, 255});
	}

	col = player_number_color(state->player_count);

	sprintf(player_num, "%d PLAYER", state->player_count);
	print_text_color(state, player_num,
		&(vector2_t){181, 201 + 30 * (state->player_count - 1)}, &col);

	draw_square(state,
		&(vector2_t){160, 210 + 30 * (state->player_count - 1)},
				14, (int)(360 * state->time/2.f) % 90, &col);
}
