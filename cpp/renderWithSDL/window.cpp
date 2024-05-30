#include "window.hpp"

SDL_Surface *load_surface(char const *path)
{
	SDL_Surface *image_surface = SDL_LoadBMP(path);

	if(!image_surface)
		return 0;

	return image_surface;
}
