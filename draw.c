#include "draw.h"

#include <math.h>

#include "renderer.h"
#include "assets/assets.h"


void draw_background(const game_state_t *state)
{
	graphics_draw_image(state->device, &(vector2_t){0, 0},
			    background_bin, NULL);
}

void draw_rect(const game_state_t *state, const rect_t *rect,
	       const color_t *col)
{
	graphics_draw_rectangle(state->device, col,
				&rect->origin, &rect->size);
}

void draw_square(const game_state_t *state, vector2_t *pos, float width,
					float degrees, const color_t *color) {
	cvertex_t vs[6];
	float s = sinf((float)M_PI * degrees/180.f);
	float c = cosf((float)M_PI * degrees/180.f);

	vs[0].pos = (vector2_t){pos->x + (c - s) * width/2,
					pos->y + (s + c) * width/2};
	vs[1].pos = (vector2_t){pos->x - (c + s) * width/2,
					pos->y + (c - s) * width/2};
	vs[2].pos = (vector2_t){pos->x + (s - c) * width/2,
					pos->y - (s + c) * width/2};
	vs[3].pos = (vector2_t){pos->x + (s + c) * width/2,
						pos->y + (s - c) * width/2};


	vs[0].color = *color;
	vs[1].color = *color;
	vs[2].color = *color;
	vs[3].color = *color;

	graphics_draw_line(state->device, &vs[0], &vs[1]);
	graphics_draw_line(state->device, &vs[1], &vs[2]);
	graphics_draw_line(state->device, &vs[2], &vs[3]);
	graphics_draw_line(state->device, &vs[3], &vs[0]);

	/* graphics_draw(state->device, vs, 6); */
}

void draw_box(const game_state_t *state, const box_t *box)
{
	graphics_draw_rectangle_outline(state->device, &box->color,
			&box->entity->pos, &box->entity->size);
}

void draw_bullet(const game_state_t *state, const bullet_t *bullet)
{
	graphics_draw_rectangle(state->device, &bullet->color,
			&bullet->entity->pos, &bullet->entity->size);
}

void draw_powerup(const game_state_t *state, const powerup_t *powerup)
{
	graphics_draw_rectangle(state->device, &powerup->color,
			&powerup->entity->pos, &powerup->entity->size);
	vector2_t pos = (vector2_t){powerup->entity->pos.x
					+ powerup->entity->size.x/2,
					powerup->entity->pos.y
					+ powerup->entity->size.y/2};
	draw_square(state, &pos, powerup->entity->size.x + 3,
			360 * state->time/1.0f, &powerup->color);
}

void draw_player(const game_state_t *state, const player_t *player)
{
	/* Motion Trail */
	for (int i = 0; i < PLAYER_TRAIL_SEGMENTS - 1; i++)
		if (player->trail[i+1].pos.x > 0)
			graphics_draw_line(state->device, &player->trail[i],
							&player->trail[i + 1]);
	/* Player */
	if (player->powerup_ghost)
		graphics_draw_rectangle_dither(state->device, &player->color,
				&player->entity->pos, &player->entity->size);
	else if (player->normal)
		graphics_draw_rectangle(state->device, &player->color,
				&player->entity->pos, &player->entity->size);
	else
		graphics_draw_rectangle_outline(state->device, &player->color,
				&player->entity->pos, &player->entity->size);

	/* Low-Life Flashing */
	if (player->lives == 1 && player->timer_flash < PLAYER_TIMER_FLASH/2)
		graphics_draw_rectangle_outline(state->device,
			&(color_t){0, 0, 255, 255}, &player->entity->pos,
							&player->entity->size);

	/* Direction Pointer */
	vector2_t dir = player_direction_vector(player);
	vector2_t pos = (vector2_t){player->entity->pos.x
				+ player->entity->size.x/2 - 3 + 15 * dir.x,
					player->entity->pos.y
				+ player->entity->size.y/2 - 3 + 15 * dir.y};

	vector2_t size = (vector2_t){6, 6};

	graphics_draw_rectangle_dither(state->device, &player->color, &pos,
							&size);

	/* Powerup Effect */
	/*
	*if (player->powerup_bullets) {
	*	pos = (vector2_t){player->entity->pos.x
	*					+ player->entity->size.x/2,
	*					player->entity->pos.y
	*					+ player->entity->size.y/2};
	*	draw_square(state, &pos, player->entity->size.x + 3,
	*			360 * state->time/0.5f, &player->color);
	*}
	*/

	/* Shield */
	if (player->powerup_shield) {
		vector2_t pos = (vector2_t){player->entity->pos.x - 1,
						player->entity->pos.y - 1};
		vector2_t size = (vector2_t){player->entity->size.x + 2,
						player->entity->size.y + 2};

		graphics_draw_rectangle_outline(state->device,
			&(color_t){255, 255, 255, 255}, &pos, &size);
	}


	/* generate vertices */
	/*cvertex_t vs[3];
	*float s = sinf((float)M_PI * player->dir/180.f);
	*float c = cosf((float)M_PI * player->dir/180.f);
	*vector2_t pos = player->entity->pos;
	*vector2_t size = player->entity->size;

	*vs[0].pos = (vector2_t){pos.x + c * size.x/2, pos.y + s * size.y/2};
	*vs[1].pos = (vector2_t){pos.x - (c + s) * size.x/2,
	*					pos.y + (c - s) * size.y/2};
	*vs[2].pos = (vector2_t){pos.x + (s - c) * size.x/2,
	*					pos.y - (s + c) * size.y/2};

	*vs[0].color = player->color;
	*vs[1].color = player->color;
	*vs[2].color = player->color;

	*graphics_draw(state->device, vs, 3);
	*/
	/*draw_square(state, &player->entity->pos, player->entity->size.x,
	*		player->dir, &player->color);
	*/
}
