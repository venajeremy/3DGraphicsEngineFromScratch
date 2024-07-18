#include "application.hpp"

Application::Application()
{
    int resX = 720;
    int resY = 450;
	m_window = SDL_CreateWindow("SDL2 Window",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			resX, resY,
			0);
	if(!m_window)
	{
		std::cout << "Failed to create window\n";
		std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
		return;
	}

    //Make Fullscreen
    //SDL_SetWindowFullscreen(m_window,SDL_WINDOW_FULLSCREEN);

	m_render = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

	if(!m_render)
	{
		std::cout << "Failed to get window's renderer\n";
		std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
		return;
	}

    // Create new world
	newCamera = new World(m_render, (2*M_PI/6), resX, resY);

    //Object arduino("arduino",0,0,100,0,0,0);
    //newCamera->addObject(arduino);

    //Object grass("grass",0,0,100,0,0,0);
    //newCamera->addObject(grass);

    //Create small minecraft world
    //Create glass plane
    /*
    for(int m = 0 ; m < 10 ; m++){
        for(int n = 0 ; n < 10 ; n++){
            Object grass("grass",0+100*m,0,100+100*n,0,0,0);
            newCamera->addObject(grass);
        }
    }*/
    for(int m = 0 ; m < 1 ; m++){
        Object wood("wood",800,100+100*m,1200,0,0,0);
        newCamera->addObject(wood);
    }
    /*
    for(int m = 0 ; m < 3; m++){
        for(int n = 0 ; n < 3 ; n++){
            for(int o = 0 ; o < 3 ; o++){
                Object leaves("leaves",700+100*m,400+100*o,1000+100*n,0,0,0);
                newCamera->addObject(leaves);
            }
        }
    }*/
    
    //Object phone("phone",0,0,100,0,0,0);
    //newCamera->addObject(phone);
    
    //Object sword("sword",0,0,100,0,0,0);
    //newCamera->addObject(sword);

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
