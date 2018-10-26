#include "game.h"
#include "text.h"

bool game_init(Game *game) {

	// Window.
	game->window = SDL_CreateWindow(
		"jack",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		0
	);
	if (game->window == NULL) {
		fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
		return false;
	}

	// Renderer.
	game->renderer = SDL_CreateRenderer(
		game->window,
		-1,
		SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC
	);
	if (game->renderer == NULL) {
		fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(game->window);
		return false;
	}

	// Font.
	game->font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf", 72);
	if (game->font == NULL) {
		fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
		SDL_DestroyRenderer(game->renderer);
		SDL_DestroyWindow(game->window);
		return false;
	}

	return true;

}

void game_deinit(Game *game) {
	TTF_CloseFont(game->font);
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
}

bool game_draw_text(Game *game, const char *str, int x, int y, SDL_Color color) {
	Text text;
	if (!text_init(&text, game->renderer, game->font, str, color)) return false;
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = text.width;
	dst.h = text.height;
	SDL_RenderCopy(game->renderer, text.texture, NULL, &dst);
	return true;
}
