#include "text.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

bool text_init(Text *text, SDL_Renderer *renderer, TTF_Font *font, const char *str, SDL_Color color) {

	// Render to a surface.
	// TODO This is extremely slow when compared to the OpenGL font rendering code I wrote last spring.
	//      We could generate a font atlas and still use SDL for cross-platform rendering,
	//      but it's not a performance necessity at this time so I'm putting it off.
	SDL_Surface *surface = TTF_RenderText_Blended(font, str, color);
	if (surface == NULL) {
		fprintf(stderr, "TTF_RenderText_Blended: %s\n", TTF_GetError());
		return false;
	}

	text->width = surface->w;
	text->height = surface->h;

	// Render to a texture.
	text->texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (text->texture == NULL) {
		fprintf(stderr, "SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
		return false;
	}

	return true;

}

void text_deinit(Text *text) {
	SDL_DestroyTexture(text->texture);
}
