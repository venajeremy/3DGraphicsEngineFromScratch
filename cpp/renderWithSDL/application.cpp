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

    // Create new world
	newCamera = new World(m_render, (2*M_PI/5), 1440, 960);
	

    // Create new object
    Surface s1({20.0f, 20.0f, -20.0f, -20.0f, 20.0f, -20.0f, -20.0f, -20.0f, -20.0f});
    Surface s2({20.0f, 20.0f, -20.0f, 20.0f, -20.0f, -20.0f, 20.0f, -20.0f, 20.0f});
    Surface s3({20.0f, 20.0f, -20.0f, 20.0f, 20.0f, 20.0f, -20.0f, 20.0f, 20.0f});
    Surface s4({-20.0f, 20.0f, -20.0f, -20.0f, 20.0f, 20.0f, -20.0f, -20.0f, 20.0f});
    Surface s5({20.0f, -20.0f, -20.0f, -20.0f, -20.0f, -20.0f, -20.0f, -20.0f, 20.0f});
    Surface s6({-20.0f, -20.0f, 20.0f, 20.0f, -20.0f, 20.0f, 20.0f, 20.0f, 20.0f});

    std::vector<Surface> mesh({s1, s2, s3, s4, s5, s6});

    cube = new Object(mesh, 0, 0, 100, 0, 0, 0);

    newCamera->addObject(*cube);

}

Application::~Application()
{
	delete newCamera;
    delete cube;
	SDL_DestroyRenderer(m_render);
	SDL_DestroyWindow(m_window);
}

void Application::loop()
{
	bool keep_window_open = true;
	while(keep_window_open)
	{
		// Handle Input
		while(SDL_PollEvent(&m_window_event) > 0)
		{
			// Give input to worlds
			newCamera->handleInput(m_window_event);
			switch(m_window_event.type)
			{
				case SDL_QUIT:
					std::cout << "Quitting\n";
					keep_window_open = false;
					break;
			}
		}
		// Should run at 240 fps?
		update(1000.0/240.0);
		draw();
	}
}

void Application::update(double delta_time)
{
	SDL_Delay(delta_time);
	// Take action
		
}

void Application::draw()
{
	// Clear Renderer
	SDL_SetRenderDrawColor(m_render, 0, 0, 0, 255);
	SDL_RenderClear(m_render);
	
	// Drawing Process
	SDL_SetRenderDrawColor(m_render, 242, 242, 242, 255);
    newCamera->renderWorld();


	// Display Updates
	SDL_RenderPresent(m_render);
}
