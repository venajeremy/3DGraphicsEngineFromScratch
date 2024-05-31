#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "window.hpp"
#include "./includes/world.hpp"

class Application
{
public:
	Application();
	~Application();
	
	void loop();
	void update(double delta_time);
	void draw();
private:
	SDL_Window	*m_window;
	SDL_Surface	*m_window_surface;
	SDL_Renderer	*m_render;
	SDL_Event	m_window_event;
	
	World	*newCamera;

};
