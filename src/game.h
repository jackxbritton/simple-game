#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "vector_math.h"

struct Game {

	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_Font *font;

	Vector2 position;
	int dt;

};

bool game_init(Game *game);
void game_deinit(Game *game);

bool game_draw_text(Game *game, const char *str, int x, int y, SDL_Color color);

struct Input {
	bool up, down, left, right;
};

bool game_update(Game *game, Input input, int dt);
bool game_render(Game *game);

#endif
