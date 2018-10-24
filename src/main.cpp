#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
