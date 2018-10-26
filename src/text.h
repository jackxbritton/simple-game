#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Text {
	int width, height;
	SDL_Texture *texture;
};

bool text_init(Text *text, SDL_Renderer *renderer, TTF_Font *font, const char *str, SDL_Color color);
void text_deinit(Text *text);

#endif
