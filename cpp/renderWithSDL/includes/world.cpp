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

	movementSpeed = 2.0f;
	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = 0.0f;
	cameraYaw = 0.0f;
	cameraPitch = 0.0f;
    cameraRoll = 0.0f;
	sensitivity = 0.001f;
	// Make Mouse Movement Relative
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void World::handleInput(SDL_Event const &event){
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
            handleRotation(event.motion.xrel,event.motion.yrel,0);
		case SDL_KEYDOWN:
			Uint8 const *keys = SDL_GetKeyboardState(nullptr);
			if(keys[SDL_SCANCODE_W] == 1){
                handleMovement(movementSpeed,0,0);
            }else if(keys[SDL_SCANCODE_S] == 1){
				handleMovement(-movementSpeed,0,0);
            }else if(keys[SDL_SCANCODE_A] == 1){
				handleMovement(0,0,-movementSpeed);
            }else if(keys[SDL_SCANCODE_D] == 1){
				handleMovement(0,0,movementSpeed);
            }

            break;

	}
}

void World::handleMovement(float forwards, float upwards, float sideways){

    // This is standing camera movement similar to most videogame's camera control
    if(forwards!=0){
        cameraX += (forwards*cos(cameraPitch)*sin(cameraYaw));
        cameraY += -(forwards*sin(cameraPitch));
        cameraZ += (forwards*cos(cameraPitch)*cos(cameraYaw));
    }
    if(upwards!=0){
        cameraY += upwards;
    }
    if(sideways!=0){
        cameraX += (sideways*cos(cameraPitch)*sin(cameraYaw+(M_PI/2)));
        cameraZ += (sideways*cos(cameraPitch)*cos(cameraYaw+(M_PI/2)));
    }
}

void World::handleRotation(float dYaw, float dPitch, float dRoll){
    
    cameraYaw += dYaw*sensitivity;
    cameraPitch += dPitch*sensitivity;

}

std::tuple<int, int> World::renderPointRelative(float ix,float iy,float iz)
{
	// Given Coorinates are relative to camera

	// z can never be zero
	if(iz==0) {
        iz = 0.0001f;
    }
	
    float xAngle = (ix)/(iz);
    
    float yAngle = (-iy)/(iz);

    return std::make_tuple(disX*(((cameraFov/2)+xAngle)/cameraFov),disY*(((cameraFov/2)+yAngle)/cameraFov));
	
}

void World::renderTriPolygon(float x1, float y1, float z1,
	    float x2, float y2, float z2,
		float x3, float y3, float z3, std::array<float,4> color)
{

    // This method takes 3 points in threespace maps them to 3 points in twospace on the window and fills the polygon depending on its visibility to the camera. A few things to take note of:

	// - Given Threespace Coordinates are relative to camera
	// - Z axis is horizontal to forward direction (when viewed from above). We do not calculate the actual distance to the point.  This speeds rendering, but forces us to rotate our world around the camera.

    // We only want to render if at least one vertex of the polygon is infront of the camera
    if (z1>0 || z2>0 || z3>0) { 

        // Use helper function to translate get twospace coordinates of vertexes relative to the camera
        screenX1 = std::get<0>(renderPointRelative(x1,y1,z1));
        screenX2 = std::get<0>(renderPointRelative(x2,y2,z2));
        screenX3 = std::get<0>(renderPointRelative(x3,y3,z3));
        screenY1 = std::get<1>(renderPointRelative(x1,y1,z1));
        screenY2 = std::get<1>(renderPointRelative(x2,y2,z2));
        screenY3 = std::get<1>(renderPointRelative(x3,y3,z3));

        // Set color to given ( in the future this will be replaced by some texture system )
        SDL_SetRenderDrawColor(renderer,color.at(0),color.at(1),color.at(2),color.at(3));
    
        // Find the min and max Y of our polygon within the window (bounding y)
        smallestY=std::min(screenY1,screenY2);
        smallestY=std::min(smallestY,screenY3);
        smallestY=std::max(1,smallestY);

        greatestY=std::max(screenY1,screenY2);
        greatestY=std::max(greatestY,screenY3);
        greatestY=std::min(disY,greatestY);

        smallestX=std::min(screenX1,screenX2);
        smallestX=std::min(smallestX,screenX3);
        smallestX=std::max(1,smallestX);

        greatestX=std::max(screenX1,screenX2);
        greatestX=std::max(greatestX,screenX3);
        greatestX=std::min(disX,greatestX);

        // Find the slope of all sides of polygon (run/rise dont get confused!)
        
        // No such thing as perfectly horizontal ;)  (wont make a difference this is just for pixels on monitor, we would need a monitor with more than 9999 pixels horizontal to see a 1 pixel error)

       
        //Slope 1:
        if (screenY1-screenY2 == 0){
            slope1 = (float)(screenX1-screenX2);
        } else {
            slope1 = ((float)(screenX1-screenX2)/(float)(screenY1-screenY2));
        }

        //Slope 2:
        if (screenY2-screenY3 == 0){
            slope2 = (float)(screenX2-screenX3);
        } else {
            slope2 = ((float)(screenX2-screenX3)/(float)(screenY2-screenY3));
        }

        //Slope 3:
        if (screenY3-screenY1 == 0){
            slope3 = (float)(screenX3-screenX1);
        } else {
            slope3 = ((float)(screenX3-screenX1)/(float)(screenY3-screenY1));
        }


        // Used in zbuffer calculation
        c = (((screenX2-screenX1)*(screenY3-screenY1))-((screenY2-screenY1)*(screenX3-screenX1))); 
        // Check for divide by zero
        if(c==0){
            c=0.001;
        }

        // Used in zbuffer calculation
        a = (((screenY2-screenY1)*(z3-z1))-((z2-z1)*(screenY3-screenY1)));

        b = (((z2-z1)*(screenX3-screenX1))-((screenX2-screenX1)*(z3-z1)));


        // Draw every pixel ( zBuffer[(((i-1)*disX)+j)-1] is the current pixel on the buffer ) ( bigger z is further away from camera )
        //      1 for every point within our triangle
        //          2 if there exists a z point of the triangle that is less than than current buffer
        //              3 calculate pixels z and see if that is less than current z buffer
        //                  4 if so, draw the pixel and add its z value to the buffer
        
        for(int i = smallestY; i<=greatestY; i++){
            
            // 1
            // Calculate the x position on every line at a given y
            
            xPos = 0;
            
            
            // Initilize the edges on left and right at y point
            triangleEdgeLeft=disX+1; 
            triangleEdgeRight=-1;
            // 1, 2, and 3 are the points of the triangle
            // We need to make sure our y point (i) is inside our edge:
            //      Remember: our edges are interpreted lines and technically go on forever, we dont want to account for part of our line that is outside the bounds of our edge

            // We then calculate the X position of this edge at our given Y(i) 
            // We make it the new left or right edge accordingly

            // Edge 1 to 2
            if(((screenY1 <= i)&&(screenY2 >= i))||((screenY2 <= i)&&(screenY1 >= i))){
                // Edge does cross this y
                xPos = (int)((i-screenY1)*(slope1))+screenX1;
                triangleEdgeLeft = std::min(triangleEdgeLeft,xPos);
                triangleEdgeRight = std::max(triangleEdgeRight,xPos);
            }
            
            // Edge 2 to 3
            if(((screenY2 <= i)&&(screenY3 >= i))||((screenY3 <= i)&&(screenY2 >= i))){
                // Edge does cross this y
                xPos = (int)((i-screenY2)*(slope2))+screenX2;
                triangleEdgeLeft = std::min(triangleEdgeLeft,xPos);
                triangleEdgeRight = std::max(triangleEdgeRight,xPos); 
            }

            // Edge 3 to 1
            if(((screenY3 <= i)&&(screenY1 >= i))||((screenY1 <= i)&&(screenY3 >= i))){
                // Edge does cross this y 
                xPos = (int)((i-screenY3)*(slope3))+screenX3;
                triangleEdgeLeft = std::min(triangleEdgeLeft,xPos);
                triangleEdgeRight = std::max(triangleEdgeRight,xPos);
            }

            // Ensure left and right are inside of display
            triangleEdgeLeft = std::max(1,triangleEdgeLeft);
            triangleEdgeLeft = std::min(disX,triangleEdgeLeft);
            triangleEdgeRight = std::max(1,triangleEdgeRight);
            triangleEdgeRight = std::min(disX,triangleEdgeRight);
            

            for(int j = triangleEdgeLeft; j<=triangleEdgeRight; j++){
                // 2
                
                currZ = zBuffer[(((i-1)*disX)+j)-1];
                if(z1<currZ || z2<currZ || z3<currZ ||currZ==-1){
                    // 3
                    
                    // Uses formula for plane between three points.  Uses 2d x and y with 3d z pos of points.  ( this might be a problem, though i hope not )

                    //pixZ = z1 + (-((((screenY2-screenY1)*(z3-z1)-(z2-z1)*(screenY3-screenY1))*(j-screenX1))+(((z2-z1)*(screenX3-screenX1)-(screenX2-screenX1)*(z3-z1))*(i-screenY1)))/((screenX2-screenX1)*(screenY3-screenY1)-(screenY2-screenY1)*(screenX3-screenX1)));
            
                    // Portions of the above formula are calculated above as they do not need to be calculated differently for each pixel. i and j (our x and y) are the only variables changing per pixel

                                   
                    // Calculate the z of current pixel
                    pixZ = 1/(z1 + (-((a*(j-screenX1))+(b*(i-screenY1)))/c));
                    //pixZ = (std::sqrt((x1*x1)+(z1*z1))+std::sqrt((x2*x2)+(z2*z2))+std::sqrt((x3*x3)+(z3*z3)))/3;

                    if(pixZ>largestZ){
                        largestZ=pixZ;
                    }
                    if(pixZ<smallestZ){
                        smallestZ=pixZ;
                    }
                    //SDL_SetRenderDrawColor(renderer,(((pixZ-smallestZ)/(largestZ-smallestZ))*color.at(0)),color.at(1),color.at(2),255);
                    // 4 pixZ < currZ
                    if((pixZ > currZ)||(currZ==-1)){
                        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        zBuffer[(((i-1)*disX)+j)-1] = pixZ;
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

    // This method just draws simple borders of polygon
	
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
    mesh = object.getMesh(cameraX,cameraY,cameraZ,cameraPitch,cameraYaw,cameraRoll);
    for(auto surface = mesh.begin(); surface != mesh.end(); ++surface) {
        // Only render object if it is infront of the camera (these positions are relative)
        if(surface->vertices[2]>0.0f && surface->vertices[5]>0.0f && surface->vertices[8]>0.0f){
            /*World::renderTriPolygon(surface->vertices[0], surface->vertices[1], surface->vertices[2],
                            surface->vertices[3], surface->vertices[4], surface->vertices[5],
                            surface->vertices[6], surface->vertices[7], surface->vertices[8], 
                            surface->color);*/
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
    largestZ = -1;
    smallestZ = 999999;
    for(auto obj = objects.begin(); obj != objects.end(); obj++) {
        renderObject(*obj);
    }
}
