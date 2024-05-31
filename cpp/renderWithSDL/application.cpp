#include "application.hpp"

Application::Application()
{
	m_window = SDL_CreateWindow("SDL2 Window",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			1440, 960,
			0);
	if(!m_window)
	{
		std::cout << "Failed to create window\n";
		std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
		return;
	}

	m_render = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

	if(!m_render)
	{
		std::cout << "Failed to get window's renderer\n";
		std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
		return;
	}

	newCamera = new World(m_render, (M_PI/2), 1440, 960);
	

	draw();
	zChange = 0;
	
}

Application::~Application()
{
	delete newCamera;
	SDL_DestroyRenderer(m_render);
	SDL_DestroyWindow(m_window);
}

void Application::loop()
{
	bool keep_window_open = true;
	while(keep_window_open)
	{
		while(SDL_PollEvent(&m_window_event) > 0)
		{
			switch(m_window_event.type)
			{
				case SDL_QUIT:
					std::cout << "Quitting\n";
					keep_window_open = false;
					break;
			}
		}
		update(1000.0/240.0);
		draw();
	}
}

void Application::update(double delta_time)
{
	SDL_Delay(delta_time);
	zChange = zChange-(0.01);	
}

void Application::draw()
{
	// Clear Renderer
	SDL_SetRenderDrawColor(m_render, 0, 0, 0, 255);
	SDL_RenderClear(m_render);
	
	// Drawing Process
	SDL_SetRenderDrawColor(m_render, 242, 242, 242, 255);
	newCamera->renderTriPolygon(20, 20, 100+zChange, 25, -30, 95+zChange, -30, 5, 102+zChange);

	//SDL_RenderDrawLine(m_render, 10, 10, 100, 100);

	// Display Updates
	SDL_RenderPresent(m_render);
}
