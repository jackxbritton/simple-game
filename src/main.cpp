#include <SDL2/SDL.h>
#include <signal.h>
#include <string.h>

#include "pool.h"

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

int main(int argc, char *argv[]) {

	if (init_sighandler() == -1) {
		perror("init_sighandler");
		return 1;
	}

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow(
		"jack",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		0
	);
	if (window == NULL) {
		fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC
	);
	if (renderer == NULL) {
		fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_SetRenderDrawColor(renderer, 127, 127, 255, 255);

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
		printf("%d\n", ticks_now - ticks_last);
		ticks_last = ticks_now;

		// TODO Display the ticks in the window.

		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);

	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
