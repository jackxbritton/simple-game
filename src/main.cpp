#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <signal.h>
#include <string.h>

#include "pool.h"
#include "game.h"
#include "text.h"

// Signal handler stuff.
volatile sig_atomic_t stop = 0;
void sighandler(int signum) {
	stop = 1;
}
// init_sighandler initializes the signal handler to capture SIGTERM and SIGINT.
// Returns 0 on success.
// On error, returns -1 and sets errno appropriately.
int init_sighandler() {
	struct sigaction act;
	act.sa_handler = sighandler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (sigaction(SIGTERM, &act, NULL) == -1) return -1;
	if (sigaction(SIGINT,  &act, NULL) == -1) return -1;
	return 0;
}

bool do_stuff(Game *game);

int main(int argc, char *argv[]) {

	if (init_sighandler() == -1) {
		perror("init_sighandler");
		return 1;
	}

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
		return 1;
	}

	if (TTF_Init() != 0) {
		fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
		SDL_Quit();
		return 1;
	}

	Game game;
	if (!game_init(&game)) {
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	bool success = do_stuff(&game);

	game_deinit(&game);
	TTF_Quit();
	SDL_Quit();
	return success ? 0 : 1;
}

bool do_stuff(Game *game) {

	bool success = true;

	// Initialize static text.
	SDL_Color white;
	white.r = 0xff;
	white.g = 0xff;
	white.b = 0xff;
	white.a = 0xff;
	Text text;
	if (!text_init(&text, game->renderer, game->font, "whatever", white)) return false;

	int ticks_last = SDL_GetTicks();

	while (!stop) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				stop = 1;
				break;
			}
		}

		int ticks_now = SDL_GetTicks();
		int ticks_diff = ticks_now - ticks_last;
		ticks_last = ticks_now;

		// Render.
		SDL_SetRenderDrawColor(game->renderer, 127, 127, 255, 255);
		SDL_RenderClear(game->renderer);

		int margin_left = 144,
			margin_top  = 144;

		SDL_Rect dst;
		dst.x = margin_left;
		dst.y = margin_top;
		dst.w = text.width;
		dst.h = text.height;
		SDL_SetTextureColorMod(text.texture, 255, 127, 127);
		SDL_RenderCopy(game->renderer, text.texture, NULL, &dst);


		// Dynamic text.
		char buf[8];
		snprintf(buf, sizeof(buf), "fps: %f", 1000.0f / ticks_diff);
		if (!game_draw_text(game, buf, margin_left, margin_top + text.height, white)) {
			success = false;
			break;
		}

		// Again!
		snprintf(buf, sizeof(buf), "%d", ticks_now);
		if (!game_draw_text(game, buf, margin_left, margin_top + 2*text.height, white)) {
			success = false;
			break;
		}

		// 100x.
		for (int i = 0; i < 100; i++) {
			snprintf(buf, sizeof(buf), "you, %i, know the drill", i);
			if (!game_draw_text(game, buf, margin_left + 32*i, margin_top + 3*text.height, white)) {
				success = false;
				break;
			}
		}

		SDL_RenderPresent(game->renderer);

	}

	text_deinit(&text);
	return success;

}
