#include "application.hpp"

Application::Application()
{
    int windowX = 1440;
    int windowY = 810;
    int renderX = 288;
    int renderY = 162;

	m_window = SDL_CreateWindow("SDL2 Window",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			windowX, windowY,
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

    // Make Fullscreen
    //SDL_SetWindowFullscreen(m_window,SDL_WINDOW_FULLSCREEN);

    // Stretch render resolution to window resolution
    SDL_RenderSetScale(m_render,((float)windowX/(float)renderX),((float)windowY/(float)renderY));


    // Create new world
	newCamera = new World(m_render, (2*M_PI/6), renderX, renderY);

    //Object arduino("arduino",0,0,100,0,0,0);
    //newCamera->addObject(arduino);

    //Object grass("grass",0,0,100,0,0,0);
    //newCamera->addObject(grass);
    
    //Object phone("phone",0,0,100,0,0,0);
    //newCamera->addObject(phone);
    
    //Object sword("sword",0,0,100,0,0,0);
    //newCamera->addObject(sword);

    
    
    
    //Create small minecraft world
    //Create glass plane
    for(int m = -2 ; m < 10 ; m++){
        for(int n = -2 ; n < 10 ; n++){
            Object grass("grass",0+100*m,0,100+100*n,0,0,0);
            newCamera->addObject(grass);
        }
    }
    //Create tree trunk
    for(int m = 0 ; m < 4 ; m++){
        Object wood("wood",800,100+100*m,700,0,0,0);
        newCamera->addObject(wood);
    }
    //Leaves bottom 
    for(int m = 0 ; m < 5; m++){
        for(int n = 0 ; n < 5 ; n++){
            for(int o = 0 ; o < 3 ; o++){
                Object leaves("leaves",600+100*m,400+100*o,500+100*n,0,0,0);
                newCamera->addObject(leaves);
            }
        }
    }
    //Leaves top
    for(int m = 0 ; m < 3; m++){
        for(int n = 0 ; n < 3 ; n++){
            for(int o = 0 ; o < 2 ; o++){
                Object leaves("leaves",700+100*m,700+100*o,600+100*n,0,0,0);
                newCamera->addObject(leaves);
            }
        }
    }
    //Create House
    //Create Pillars
    for(int m = 0 ; m < 4 ; m++){
        Object wood("wood",300,100+100*m,300,0,0,0);
        newCamera->addObject(wood);
    }
    for(int m = 0 ; m < 4 ; m++){
        Object wood("wood",-200,100+100*m,800,0,0,0);
        newCamera->addObject(wood);
    }
    for(int m = 0 ; m < 4 ; m++){
        Object wood("wood",300,100+100*m,800,0,0,0);
        newCamera->addObject(wood);
    }
    for(int m = 0 ; m < 4 ; m++){
        Object wood("wood",-200,100+100*m,300,0,0,0);
        newCamera->addObject(wood);
    }
    //Create Walls
    for(int m = 0 ; m < 4 ; m++){
        for(int n = 0 ; n < 4 ; n++){
            Object cobble("cobble",-200,100+100*m,400+100*n,0,0,0);
            newCamera->addObject(cobble);
        }
    }
    for(int m = 0 ; m < 4 ; m++){
        for(int n = 0 ; n < 4 ; n++){
            Object cobble("cobble",300,100+100*m,400+100*n,0,0,0);
            newCamera->addObject(cobble);
        }
    }
    for(int m = 0 ; m < 4 ; m++){
        for(int n = 0 ; n < 4 ; n++){
            Object cobble("cobble",-100+100*n,100+100*m,800,0,0,0);
            newCamera->addObject(cobble);
        }
    }
    //Create Roof
    for(int m = 0 ; m < 6 ; m++){
        for(int n = 0 ; n < 6 ; n++){
            Object planks("planks",-200+100*n,500,300+100*m,0,0,0);
            newCamera->addObject(planks);
        }
    }
    

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
