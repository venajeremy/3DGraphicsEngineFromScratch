#include "world.hpp"

World::World(SDL_Renderer *inputRenderer, float inputCameraFov, int displayX, int displayY){
	renderer = inputRenderer;
	cameraFov = inputCameraFov;
	disX = displayX;
	disY = displayY;
	movementSpeed = 5.0f;
	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = 0.0f;
	cameraYaw = 0.0f;
	cameraPitch = 0.0f;
	sensitivity = 0.005f;
	// Make Mouse Movement Relative
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void World::handleInput(SDL_Event const &event){
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
            dY = event.motion.xrel*sensitivity;
            dP = event.motion.yrel*sensitivity;
			cameraYaw = cameraYaw-dY;
			cameraPitch = cameraPitch-dP;
            handleRotation(dY,dP, 0);
			std::cout << cameraYaw << "\n";
		case SDL_KEYDOWN:
			Uint8 const *keys = SDL_GetKeyboardState(nullptr);
			if(keys[SDL_SCANCODE_W] == 1){
                handleMovement(0, 0, movementSpeed);
            }else if(keys[SDL_SCANCODE_S] == 1){
				handleMovement(0, 0, -movementSpeed);
            }else if(keys[SDL_SCANCODE_A] == 1){
				handleMovement(-movementSpeed, 0, 0);
            }else if(keys[SDL_SCANCODE_D] == 1){
				handleMovement(movementSpeed, 0, 0);
            }
            break;

	}
}

void World::handleMovement(float dX, float dY, float dZ){
    //Move every object in world
    for(auto obj = objects.begin(); obj != objects.end(); obj++) {
        obj->worldTranslate(dX,dY,dZ);
    }
}

void World::handleRotation(float dYaw, float dPitch, float dRoll){
    //Rotate every object in world around camera
    for(auto obj = objects.begin(); obj != objects.end(); obj++) {
        obj->worldRotate(dYaw,dPitch,dRoll);
    }

}

std::tuple<float , float > World::renderPointRelative(float  ix,float  iy,float  iz)
{
	// Given Coorinates are relative to camera
	// Fix this later (vertex is behind camera it is ignored)
	if(iz>0)
	{
		float xAngle = (ix)/(iz);
		
		float yAngle = (-iy)/(iz);

		return std::make_tuple(disX*(((cameraFov/2)+xAngle)/cameraFov),disY*(((cameraFov/2)+yAngle)/cameraFov));
	} else {
		return std::make_tuple(-1, -1);
	}
} 

void World::renderTriPolygon(int x1, int y1, int z1,
		int x2, int y2, int z2,
		int x3, int y3, int z3)
{
	
	// Given Coordinates are relative to camera
	// z axis is horizontal to forward direction (when viewed from above)
	std::tuple<float , float > p1 = renderPointRelative(x1, y1, z1);
	std::tuple<float , float > p2 = renderPointRelative(x2, y2, z2);
	std::tuple<float , float > p3 = renderPointRelative(x3, y3, z3);

	// p1 to p2
	SDL_RenderDrawLine(renderer,std::get<0>(p1),std::get<1>(p1),std::get<0>(p2),std::get<1>(p2));

	// p2 to p3
	SDL_RenderDrawLine(renderer,std::get<0>(p2),std::get<1>(p2),std::get<0>(p3),std::get<1>(p3));

	// p3 to p1
	SDL_RenderDrawLine(renderer,std::get<0>(p3),std::get<1>(p3),std::get<0>(p1),std::get<1>(p1));	
	
}

void World::renderObject(Object object)
{
    mesh = object.getMesh();
    for(auto surface = mesh.begin(); surface != mesh.end(); ++surface) {
        // Only render object if it is infront of the camera (these positions are relative)
        if(surface->vertices[2]>0.0f && surface->vertices[5]>0.0f && surface->vertices[8]>0.0f){
            World::renderTriPolygon(surface->vertices[0], surface->vertices[1], surface->vertices[2],
                            surface->vertices[3], surface->vertices[4], surface->vertices[5],
                            surface->vertices[6], surface->vertices[7], surface->vertices[8]);
        }
    }
}

void World::addObject(Object object)
{
    objects.push_back(object);
}

void World::renderWorld()
{
    for(auto obj = objects.begin(); obj != objects.end(); obj++) {
        renderObject(*obj);
    }
}
