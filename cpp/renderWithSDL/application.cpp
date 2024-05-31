#include "application.hpp"

Application::Application()
{
	m_window = SDL_CreateWindow("SDL2 Window",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			680, 480,
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

	newCamera = new World(m_render, (M_PI/2), 680, 480);
	

	draw();
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
		//update(1.0/60.0);
		//draw();
	}
}

void Application::update(double delta_time)
{
	zChange = zChange+1;	
}

void Application::draw()
{
	// Clear Renderer
	SDL_SetRenderDrawColor(m_render, 0, 0, 0, 255);
	SDL_RenderClear(m_render);
	
	// Drawing Process
	SDL_SetRenderDrawColor(m_render, 242, 242, 242, 255);
	newCamera->renderTriPolygon(20, 20, 100, 25, -30, 95, -30, 5, 102);
	newCamera->renderTriPolygon(20, 20, 90, 25, -30, 85, -30, 5, 92);
	newCamera->renderTriPolygon(20, 20, 80, 25, -30, 75, -30, 5, 82);
	newCamera->renderTriPolygon(20, 20, 70, 25, -30, 65, -30, 5, 72);
	newCamera->renderTriPolygon(20, 20, 60, 25, -30, 55, -30, 5, 62);
	newCamera->renderTriPolygon(20, 20, 50, 25, -30, 45, -30, 5, 52);

	//SDL_RenderDrawLine(m_render, 10, 10, 100, 100);

	// Display Updates
	SDL_RenderPresent(m_render);
}
