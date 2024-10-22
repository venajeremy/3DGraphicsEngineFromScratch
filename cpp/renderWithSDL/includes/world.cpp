#include "world.hpp"
#include "object.hpp"

World::World(SDL_Renderer *inputRenderer, float inputCameraFov, int displayX, int displayY, bool enableDepthCulling){
	renderer = inputRenderer;
	cameraFov = inputCameraFov;
	disX = displayX;
	disY = displayY;

    // Texture Folder
    textureFolderLocation = "/textures/";

    // Initialize zBuffers
    emptyBuffer.assign(displayX*displayY,-1);
    zBuffer = emptyBuffer;

    // Enable Depth Culling
    depthCulling = enableDepthCulling;

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
		float x3, float y3, float z3, 
        float u1, float v1,
        float u2, float v2,
        float u3, float v3,
        std::string textureMap)
{

    // This method takes 3 points in threespace maps them to 3 points in twospace on the window and fills the polygon depending on its visibility to the camera.

    // We only want to render if at least one vertex of the polygon is infront of the camera
    if (z1>0 || z2>0 || z3>0) { 

        // Use helper function to get twospace coordinates of vertexes relative to the camera
        screenX1 = std::get<0>(renderPointRelative(x1,y1,z1));
        screenX2 = std::get<0>(renderPointRelative(x2,y2,z2));
        screenX3 = std::get<0>(renderPointRelative(x3,y3,z3));
        screenY1 = std::get<1>(renderPointRelative(x1,y1,z1));
        screenY2 = std::get<1>(renderPointRelative(x2,y2,z2));
        screenY3 = std::get<1>(renderPointRelative(x3,y3,z3));

        // Depth Culling optimization
        // Find the current zbuffer distances at the 3 verticies of the current triangle and see if our verticies would be closer to the camera
        // If the current verticies are not visible to the camera and behind an object rendered this frame do not draw the rest of the triangle
        // This optimization is more effective if every object in the scene is sorted by its distance from the camera
        if(depthCulling){
            if(screenX1 > 0 && screenX1 < disX && screenY1 > 0 && screenY1 < disY){
                currZ1 = zBuffer[(((screenY1-1)*disX)+screenX1)-1];
            } else {
                currZ1 = -1;
            }
            if(screenX2 > 0 && screenX2 < disX && screenY2 > 0 && screenY2 < disY){
                currZ2 = zBuffer[(((screenY2-1)*disX)+screenX2)-1];
            } else {
                currZ2 = -1;
            }
            if(screenX3 > 0 && screenX3 < disX && screenY3 > 0 && screenY3 < disY){
                currZ3 = zBuffer[(((screenY3-1)*disX)+screenX3)-1];
            } else {
                currZ3 = -1;
            }
            if((z1>currZ1+1 && currZ1 != -1)
            && (z2>currZ2+1 && currZ2 != -1)
            && (z3>currZ3+1 && currZ3 != -1)){
                return;
            }
        }

        // ---------------- BEGIN RASTERIZATION -----------------

        // Find the min and max Y of our polygon within the window (bounding y)
        smallestY=std::min(screenY1,screenY2);
        smallestY=std::min(smallestY,screenY3);
        smallestY=std::max(1,smallestY);

        greatestY=std::max(screenY1,screenY2);
        greatestY=std::max(greatestY,screenY3);
        greatestY=std::min(disY,greatestY);

        // Find the slope of all sides of polygon (run/rise dont get confused!)
        // Used in rasterization 
        // If side is perfectly horizontal just make it the length of the side

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

        
        // Used later in texture mapping
        // Basically equation of plane mapping u and v with respect to screenX and screenY coords
        // Similar to z position calculation in zbuffer calculation below
        // Again this takes z into account due to screenX and Y being divisions of z
        tu_a = (((screenY2-screenY1)*(u3/z3-u1/z1))-((u2/z2-u1/z1)*(screenY3-screenY1)));
        tv_a = (((screenY2-screenY1)*(v3/z3-v1/z1))-((v2/z2-v1/z1)*(screenY3-screenY1)));
        tu_b = (((u2/z2-u1/z1)*(screenX3-screenX1))-((screenX2-screenX1)*(u3/z3-u1/z1)));
        tv_b = (((v2/z2-v1/z1)*(screenX3-screenX1))-((screenX2-screenX1)*(v3/z3-v1/z1)));
        tu_c = (((screenX2-screenX1)*(screenY3-screenY1))-((screenY2-screenY1)*(screenX3-screenX1)));
        tv_c = tu_c;

        // Used in zbuffer calculation (later will build a function of x' and y' outputting 1/z')
        a = (((screenY2-screenY1)*(1.0/z3-1.0/z1))-((1.0/z2-1.0/z1)*(screenY3-screenY1)));

        b = (((1.0/z2-1.0/z1)*(screenX3-screenX1))-((screenX2-screenX1)*(1.0/z3-1.0/z1)));

        c = (((screenX2-screenX1)*(screenY3-screenY1))-((screenY2-screenY1)*(screenX3-screenX1))); 


        // Draw every pixel ( zBuffer[(((i-1)*disX)+j)-1] is the current pixel on the buffer ) ( bigger z is further away from camera )
        //      1 for every point within our triangle
        //          2 if there exists a z point of the triangle that is less than than current buffer
        //              3 calculate pixels z and see if that is less than current z buffer
        //                  4 if so, draw the pixel and add its z value to the buffer
        
        for(int i = smallestY; i<=greatestY; i++){
            
            // 1    NOTE: this portion can be made faster.  We instead could seperate the triangle into two portions a top and a bottom, then find the respective lines for the left and right of the top and bottom portions.  Then over two loops draw the top and bottom portions of the triangle.  I feel like this method is kinda messy though as you dont know which of the 3 lins is going to cary over between the top and bottom portions so I didn't do it
            
            // Calculate the left and right x positions on every line at a given y
            
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
                            
                    // Portions of the above formula are calculated above as they do not need to be calculated differently for each pixel. i and j (our x and y) are the only variables changing per pixel

                    // Uses equation of plane in 2D space on screen with respect to 1/z. We have to use 1/z because the change in z with respect to the change of position on screen is not linear (due to the fact we are looking at a flat surface from an angle).

                    // Ensure z1 is not zero
                    if(z1==0){
                        z1=0.0001;
                    }
                    // Calculate unrecipricated pixZ
                    uRpixZ = 1.0/z1 + (-((a*(j-screenX1))+(b*(i-screenY1)))/c);

                    // Calculate the z of current pixel with  
                    pixZ = 1.0 / uRpixZ;
                    
                    // Calculate the texture position of pix
                    tU = u1/z1 + (-((tu_a*(j-screenX1))+(tu_b*(i-screenY1)))/tu_c);
                    tV = v1/z1 + (-((tv_a*(j-screenX1))+(tv_b*(i-screenY1)))/tv_c);

                    // Final percentage locations on texture
                    
                    tU = tU*pixZ;
                    tV = tV*pixZ;

                    // Get pixel on texture these percentages relate to
                    pixTex = tgaReadPixel(textureMap, tU, tV);
                    
                    // Set the draw color of current pixel to the color at that point on texture
                    SDL_SetRenderDrawColor(renderer, (int)pixTex.r, (int)pixTex.g, (int)pixTex.b, (int)pixTex.a);

                    if((pixZ < currZ)||(currZ==-1)){
                        // 4
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

    SDL_SetRenderDrawColor(renderer,255, 255, 255, 255);

	// p1 to p2
	SDL_RenderDrawLine(renderer,std::get<0>(p1),std::get<1>(p1),std::get<0>(p2),std::get<1>(p2));

	// p2 to p3
	SDL_RenderDrawLine(renderer,std::get<0>(p2),std::get<1>(p2),std::get<0>(p3),std::get<1>(p3));

	// p3 to p1
	SDL_RenderDrawLine(renderer,std::get<0>(p3),std::get<1>(p3),std::get<0>(p1),std::get<1>(p1));	
	
}

void World::renderObject(Object *object)
{
    mesh = object->getMesh(cameraX,cameraY,cameraZ,cameraPitch,cameraYaw,cameraRoll);
    for(auto surface = mesh.begin(); surface != mesh.end(); ++surface) {
        // Only render object if it is infront of the camera (these positions are relative)
        if(surface->vertices[2]>0.0f && surface->vertices[5]>0.0f && surface->vertices[8]>0.0f){
            /*World::renderEdgeTriPolygon(surface->vertices[0], surface->vertices[1], surface->vertices[2],
                            surface->vertices[3], surface->vertices[4], surface->vertices[5],
                            surface->vertices[6], surface->vertices[7], surface->vertices[8], 
                            surface->color);*/
            World::renderTriPolygon(surface->vertices[0], surface->vertices[1], surface->vertices[2],
                            surface->vertices[3], surface->vertices[4], surface->vertices[5],
                            surface->vertices[6], surface->vertices[7], surface->vertices[8],
                            surface->textureCoords[0],surface->textureCoords[1],
                            surface->textureCoords[2],surface->textureCoords[3],
                            surface->textureCoords[4],surface->textureCoords[5],
                            surface->textureMap);
        }
    }
}

void World::addObject(Object object)
{
    objects.push_back(object);
}

bool World::compairObjectDistance(Object *object1, Object *object2){
    return (object1->getDistance(cameraX,cameraY,cameraZ)<object2->getDistance(cameraX,cameraY,cameraZ));
}

void World::renderWorld()
{
    // Clear the Zbuffer
    zBuffer=emptyBuffer;
    if(depthCulling){
        // Sort the objects by their distance from the screen for Depth Culling
        std::sort(objects.begin(), objects.end(),  [this](Object& obj1, Object& obj2) {
            return compairObjectDistance(&obj1, &obj2);
        });
    }
    // Render all objects in scene
    for(std::vector<Object>::iterator obj = objects.begin(); obj != objects.end(); obj++) {
        renderObject(&(*obj));
    }
}
