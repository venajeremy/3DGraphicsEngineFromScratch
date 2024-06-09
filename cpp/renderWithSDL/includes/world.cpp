#include "world.hpp"
#include "object.hpp"

World::World(SDL_Renderer *inputRenderer, float inputCameraFov, int displayX, int displayY){
	renderer = inputRenderer;
	cameraFov = inputCameraFov;
	disX = displayX;
	disY = displayY;

    // Initialize zBuffers
    emptyBuffer.assign(displayX*displayY,-1);
    zBuffer = emptyBuffer;

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

std::tuple<int, int> World::renderPointRelative(float  ix,float  iy,float  iz)
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

void World::renderTriPolygon(float x1, float y1, float z1,
	    float x2, float y2, float z2,
		float x3, float y3, float z3, std::array<float,4> color)
{ 

	// Given Coordinates are relative to camera
	// z axis is horizontal to forward direction (when viewed from above)

    // Make sure object is infront of camera
    if (z1>0 && z2>0 && z3>0) { 

        screenX1 = std::get<0>(renderPointRelative(x1,y1,z1));
        screenX2 = std::get<0>(renderPointRelative(x2,y2,z2));
        screenX3 = std::get<0>(renderPointRelative(x3,y3,z3));
        screenY1 = std::get<1>(renderPointRelative(x1,y1,z1));
        screenY2 = std::get<1>(renderPointRelative(x2,y2,z2));
        screenY3 = std::get<1>(renderPointRelative(x3,y3,z3));

        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        // p1 to p2
        SDL_RenderDrawLine(renderer, screenX1, screenY1, screenX2, screenY2);
        
        // p2 to p3
        SDL_RenderDrawLine(renderer, screenX2, screenY2, screenX3, screenY3);

        // p3 to p1
        SDL_RenderDrawLine(renderer, screenX3, screenY3, screenX1, screenY1);

        // Set color to given
        SDL_SetRenderDrawColor(renderer,color.at(0),color.at(1),color.at(2),color.at(3));
    
        // Find bounding box of triangle(winin the display window)
        smallestX=std::min(screenX1,screenX2);
        smallestX=std::min(smallestX,screenX3);
        smallestX=std::max(0,smallestX);
        smallestY=std::min(screenY1,screenY2);
        smallestY=std::min(smallestY,screenY3);
        smallestY=std::max(0,smallestY);

        greatestX=std::max(screenX1,screenX2);
        greatestX=std::max(greatestX,screenX3);
        greatestX=std::min(disX,greatestX);
        greatestY=std::max(screenY1,screenY2);
        greatestY=std::max(greatestY,screenY3);
        greatestY=std::min(disY,greatestY);

        //Slope 1:
        if (screenY1-screenY2 == 0){
            slope1 = 9999;
        } else {
            slope1 = ((float)(screenX1-screenX2)/(float)(screenY1-screenY2));
        }

        //Slope 2:
        if (screenY2-screenY3 == 0){
            slope2 = 9999;
        } else {
            slope2 = ((float)(screenX2-screenX3)/(float)(screenY2-screenY3));
        }

        //Slope 3:
        if (screenY3-screenY1 == 0){
            slope3 = 9999;
        } else {
            slope3 = ((float)(screenX3-screenX1)/(float)(screenY3-screenY1));
        }

        // Draw every pixel ( zBuffer[((i-1)*disX)+j] is the current pixel on the buffer ) ( bigger z is further away from camera )
        //      1 find every pixel that is inside the traingle
        //          2 if there exists a z point of the triangle that is less than than current buffer 
        //              3 calculate pixels z and see if that is less than current z buffer
        //                  4 if so, draw the pixel and add its z value to the buffer
        
        for(int i = smallestY; i<=greatestY; i++){

            // Calculate the x position on every line at a given y
            
            triangleEdgeLeft=disX+1;
            triangleEdgeRight=-1;
            xPos = 0;
            // Edge 1 to 2
            if(((screenY1 <= i)&&(screenY2 >= i))||((screenY2 <= i)&&(screenY1 >= i))){
                // Edge does cross this y
                xPos = (int)((float)(i-screenY1)*(slope1))+screenX1;
                triangleEdgeLeft = std::min(triangleEdgeLeft,xPos);
                triangleEdgeRight = std::max(triangleEdgeRight,xPos);
            }
            
            // Edge 2 to 3
            if(((screenY2 <= i)&&(screenY3 >= i))||((screenY3 <= i)&&(screenY2 >= i))){
                // Edge does cross this y
                xPos = (int)((float)(i-screenY2)*(slope2))+screenX2;
                triangleEdgeLeft = std::min(triangleEdgeLeft,xPos);
                triangleEdgeRight = std::max(triangleEdgeRight,xPos); 
            }

            // Edge 3 to 1
            if(((screenY3 <= i)&&(screenY1 >= i))||((screenY1 <= i)&&(screenY3 >= i))){
                // Edge does cross this y 
                xPos = (int)((float)(i-screenY3)*(slope3))+screenX3;
                triangleEdgeLeft = std::min(triangleEdgeLeft,xPos);
                triangleEdgeRight = std::max(triangleEdgeRight,xPos);
            }

            /*  This is very slow we should not have to search for the edge we can calculate it 
            
            lastRead=0;
            triangleEdgeLeft=-1;
            triangleEdgeRight=-1;
            for (int k = smallestX; k<=greatestX; k++){
                // See if our current y (i) lays on one of the triangles borders at x = k
                if((((i < ((slope1)*(float)(k+1-screenX1))+screenY1) && (i > ((slope1)*(float)(k-1-screenX1))+screenY1)) || ((i > ((slope1)*(float)(k+1-screenX1))+screenY1) && (i < ((slope1)*(float)(k-1-screenX1))+screenY1))) && lastRead != 1){
                    
                    if(triangleEdgeLeft==-1){
                        triangleEdgeLeft=k;
                        lastRead=1;
                    } else if (triangleEdgeRight==-1){
                        triangleEdgeRight=k;
                        lastRead=1;
                    } else {
                        break;
                    }
                }
                    //i == (int)((slope2)*(float)(k-screenX2))+screenY2
                if((((i < ((slope2)*(float)(k+1-screenX2))+screenY2) && (i > ((slope2)*(float)(k-1-screenX2))+screenY2)) || ((i > ((slope2)*(float)(k+1-screenX2))+screenY2) && (i < ((slope2)*(float)(k-1-screenX2))+screenY2))) && lastRead != 2){
                    
                    if(triangleEdgeLeft==-1){
                        triangleEdgeLeft=k;
                        lastRead=2;
                    } else if (triangleEdgeRight==-1){
                        triangleEdgeRight=k;
                        lastRead=2;
                    } else {
                        break;
                    }
                }

                if((((i < ((slope3)*(float)(k+1-screenX3))+screenY3) && (i > ((slope3)*(float)(k-1-screenX3))+screenY3)) || ((i > ((slope3)*(float)(k+1-screenX3))+screenY3) && (i < ((slope3)*(float)(k-1-screenX3))+screenY3))) && lastRead != 3){
                    
                    if(triangleEdgeLeft==-1){
                        triangleEdgeLeft=k;
                        lastRead=3;
                    } else if (triangleEdgeRight==-1){
                        triangleEdgeRight=k;
                        lastRead=3;
                    } else {
                        break;
                    }
                }
                
            } */
             
            for(int j = triangleEdgeLeft; j<=triangleEdgeRight; j++){
                //currZ = zBuffer[((i-1)*disX)+j];
                // 1 z1<currZ || z2<currZ || z3<currZ ||currZ==-1
                if(true){
                    // 2
                    
                    //delX = (j-screenX1)*z1      because screenX1 = x1/z1 (surface is linear)
                    //delY = (i-screenY1)*z1
                    //a = <x2-x1,y2-y1,z2-z1>
                    //b = <x3-x1,y3-y1,z3-z1>
                    //pixZ = z1 + (-((((y2-y1)*(z3-z1)-(z2-z1)*(y3-y1))*((j-screenX1)*z1))+(((z2-z1)*(x3-x1)-(x2-x1)*(z3-z1))*((i-screenY1))*z1))/((x2-x1)*(y3-y1)-(y2-y1)*(x3-x1)));

                    //zCalcDenominator = (((x2-x1)*(y3-y1))-((y2-y1)*(x3-x1))); 
                    // Check for divide by zero
                    //if(zCalcDenominator==0){
                    //    break;
                    //}
                    // Calculating the Z here does not work
                    //pixZ = z1 + (-((((y2-y1)*(z3-z1)-(z2-z1)*(y3-y1))*((j-screenX1)*z1))+(((z2-z1)*(x3-x1)-(x2-x1)*(z3-z1))*((i-screenY1))*z1))/(zCalcDenominator));

                    // 3 pixZ < currZ
                    if(true){

                    
                        SDL_RenderDrawPoint(renderer, j,i);

                        
                    }
                }
            }
        }

    }
	
}

void World::renderEdgeTriPolygon(float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3, std::array<float,4> color)
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
                            surface->vertices[6], surface->vertices[7], surface->vertices[8], 
                            surface->color);
        }
    }
}

void World::addObject(Object object)
{
    objects.push_back(object);
}

void World::renderWorld()
{
    zBuffer=emptyBuffer;
    for(auto obj = objects.begin(); obj != objects.end(); obj++) {
        renderObject(*obj);
    }
}
