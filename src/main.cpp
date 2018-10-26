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

	Input input;
	input.up    = false;
	input.down  = false;
	input.left  = false;
	input.right = false;

	int ticks_last = SDL_GetTicks();

	while (!stop) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				stop = 1;
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_k:
					input.up = event.type == SDL_KEYDOWN;
					break;
				case SDLK_j:
					input.down = event.type == SDL_KEYDOWN;
					break;
				case SDLK_h:
					input.left = event.type == SDL_KEYDOWN;
					break;
				case SDLK_l:
					input.right = event.type == SDL_KEYDOWN;
					break;
				}
				break;
			}
		}

		int ticks_now = SDL_GetTicks();
		int dt = ticks_now - ticks_last;
		ticks_last = ticks_now;

		if (!game_update(game, input, dt)) return false;
		if (!game_render(game)) return false;

	}

	return success;

}
