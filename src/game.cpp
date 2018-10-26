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

	game->position = Vector2(0.0f, 0.0f);
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

bool game_update(Game *game, Input input, int dt) {

	float speed = 32.0f;
	Vector2 direction(0.0f, 0.0f);
	direction.y -= input.up    * speed * dt;
	direction.y += input.down  * speed * dt;
	direction.x -= input.left  * speed * dt;
	direction.x += input.right * speed * dt;
	direction.normalize();

	game->position += direction;
	game->dt = dt;
	return true;
}

bool game_render(Game *game) {

	// Clear.
	SDL_SetRenderDrawColor(game->renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(game->renderer);

	// Dynamic text.
	SDL_Color white;
	white.r = 0xff;
	white.g = 0xff;
	white.b = 0xff;
	white.a = 0xff;
	if (!game_draw_text(game, "i", game->position.x, game->position.y, white)) return false;

	// More dynamic text.
	SDL_Color blue;
	blue.r = 0x88;
	blue.g = 0x88;
	blue.b = 0xff;
	blue.a = 0xff;
	char buf[32];
	snprintf(buf, sizeof(buf), "fps=%f", 1000.0f / game->dt);
	if (!game_draw_text(game, buf, 32, 32, blue)) return false;
	snprintf(buf, sizeof(buf), "dt=%d", game->dt);
	if (!game_draw_text(game, buf, 32, 72+32, blue)) return false;

	SDL_RenderPresent(game->renderer);
	return true;

}
