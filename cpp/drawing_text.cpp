#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;


// Camera
float cameraX = -50;
float cameraY = 0;
float cameraZ = 0;
float cameraYaw = 0;
float cameraPitch = 0;
float cameraRoll = 0;
float fov = 3.14/2;
int moveSpeed = 10;
float turnSpeed = 3.14/8;

// Screen/Render
const int screenWidth=100;
const int screenHeight=50;
char buffer[screenWidth*screenHeight];	// stores characters to be displayed in array
int background = 32;	// blank character to fill spaces of buffer

// Render buffer: iterates through the buffer arry and writes the characters within it to the console; prints new lines at screen width

void renderBuffer(){
	
	for(int i=0; i<screenWidth*screenHeight; i++){
		if(i%screenWidth==0){
			putchar('\n'); // print a new line if we are on an edge character
		} else {
			putchar(buffer[i]); // else print char from buffer
		}	
	}
	putchar('\n');
}

// Clear buffer: Resets the buffer to blank characters

void clearBuffer(){
	memset(buffer, background, screenWidth * screenHeight);
}

// Render line: Draws a line on the buffer, starts at x1 y1 follows to y2 along slope (slightly inaccurate but idgaf)

void renderLine(int x1, int y1, int x2, int y2, char h){
	int dy = (y2-y1);
	int dx = (x2-x1);
	
	if(dy==0){	// cant find slope if dy==0 must be a orizontal line at y1
		if(x1<x2){
			for(int i=x1; i<x2;i++){
				if(y1>0&&y1<screenHeight&&i>0&&i<screenWidth){
					buffer[(y1*screenWidth)+i] = h;
				}
			}
			return;
		} else {
			for(int i=x2; i<x1;i++){
				if(y1>0&&y1<screenHeight&&i>0&&i<screenWidth){
					buffer[(y1*screenWidth)+i] = h;
				}
			}
			return;
		}
	}
	
	int run = dx/dy;
	if(dy>0){	// if line is going up
		for(int i=0; i<dy; i++){
			if(y1>(0-i)&&y1<(screenHeight-i)&&x1>(0-(i*run))&&x1<(screenWidth-i*run)){
				buffer[((y1+i)*screenWidth)+x1+(i*run)] = h;	// i*screenWidth gets right position in y, i*run stands for the x position
			}
		}
	} else if (dy<0) {
		for(int i=0; i>dy; i--){
			if(y1>(0-i)&&y1<(screenHeight-i)&&x1>(0-(i*run))&&x1<(screenWidth-i*run)){
				buffer[((y1+i)*screenWidth)+x1+(i*run)] = h;	// i*screenWidth gets right position in y, i*run stands for the x position
			}
		}
	}
	
}

// Render quad: just uses the renderLine function to draw a quad

void renderQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, char h){
	//cout << "Rendering quad with points: (" << x1 << ", " << y1 << "), (" << x2 << ", " << y2 << "), (" << x3 << ", " << y3 << "), (" << x4 << ", " << y4 << ")\n";
	renderLine(x1, y1, x2, y2, h);
	renderLine(x2, y2, x3, y3, h);
	renderLine(x3, y3, x4, y4, h);
	renderLine(x4, y4, x1, y1, h);

}

// Takes 3space coordinates as input, returns a position on screen with respect to camera's position and direction

int renderX(int x, int y, int z){
	int dX = x-cameraX;	// change in x between camera and object
	int dY = y-cameraY;	// change in y between camera and object
	float alpha = atan2(dY, dX);// universal angle between camera and object (atan2 function is weird first input is rise, run)
	float beta = cameraYaw;		// universal angle of camera
	float direction = fmod(alpha-beta,(3.14159f*2.0f));   // direction of object from perspective of camera
					//
	//cout << "returning x: " << int((direction/(fov/2))*(screenWidth/2)) << " for: ("<<x<<","<<y<<","<<z<<")\n"; 
	// return converted angle to screen position in x
	return(int((direction/(fov/2))*(screenWidth/2))+(screenWidth/2));
}

// Similar to function above but to find y position

int renderY(int x, int y, int z){
	int dX = x-cameraX;	// change in x between camera and object
	int dY = y-cameraY;	// change in y between camera and object
	int dZ = z-cameraZ;	// change in z between camera and object
	float dXY = sqrtf(fabs((dX*dX)+(dY*dY)));	// distance on xy plane between camera and object
	float alpha = atan2(dZ, dXY);
	float beta = cameraPitch;
	float direction = alpha-beta;
	
	// return converted angle to screen position in y
	//cout << "returning y: " << int((direction/(fov/2))*(screenHeight/2)) << " for: ("<<x<<","<<y<<","<<z<<")\n"; 
	return(int((direction/(fov/2))*(screenHeight/2))+(screenHeight/2));
}

// Takes position and sizes as input, creates 8 points and renders quads between them

void renderCuboid(int centerX, int centerY, int centerZ, int width, int height, int length, char h){
	

//      1---------2              z+
//	|  5---6  |		 |
//	|  |   |  |	  x+ ----|
//	|  |   |  |		 \
//	|  8---7  |		  \
//	4---------3		   y-

	int cX = width/2;
	int cY = length/2;
	int cZ = height/2;
	
	int x1=renderX(centerX+cX, centerY-cY, centerZ+cZ);
	int y1=renderY(centerX+cX, centerY-cY, centerZ+cZ);

	int x2=renderX(centerX-cX, centerY-cY, centerZ+cZ);
	int y2=renderY(centerX-cX, centerY-cY, centerZ+cZ);

	int x3=renderX(centerX-cX, centerY-cY, centerZ-cZ);
	int y3=renderY(centerX-cX, centerY-cY, centerZ-cZ);

	int x4=renderX(centerX+cX, centerY-cY, centerZ-cZ);
	int y4=renderY(centerX+cX, centerY-cY, centerZ-cZ);

	int x5=renderX(centerX+cX, centerY+cY, centerZ+cZ);
	int y5=renderY(centerX+cX, centerY+cY, centerZ+cZ);

	int x6=renderX(centerX-cX, centerY+cY, centerZ+cZ);
	int y6=renderY(centerX-cX, centerY+cY, centerZ+cZ);

	int x7=renderX(centerX-cX, centerY+cY, centerZ-cZ);
	int y7=renderY(centerX-cX, centerY+cY, centerZ-cZ);

	int x8=renderX(centerX+cX, centerY+cY, centerZ-cZ);
	int y8=renderY(centerX+cX, centerY+cY, centerZ-cZ);
	
	//cout <<"successfully rendered quardinates of cube's corners!\n";

	// ignores top and bottom
	renderQuad(x1, y1, x2, y2, x3, y3, x4, y4, h);//1 2 3 4
	renderQuad(x2, y2, x3, y3, x7, y7, x6, y6, h);//2 3 7 6
	renderQuad(x6, y6, x7, y7, x8, y8, x5, y5, h);//6 7 8 5
	renderQuad(x1, y1, x4, y4, x8, y8, x5, y5, h);//1 4 8 5

}

//------------------------------------Main Function------------------------------------//

int main(){
	char action;
	while(1){
		clearBuffer();
	
		//renderLine(renderX(0, -20, 15), renderY(0,-20, 15), renderX(0, 80, -20), renderY(0, 80, -20), '#');
		//renderLine(80, 80, 10, 80, 'T');
	
		renderCuboid(30, 0, 0, 50, 50, 50, '@');

		//renderQuad(10, 10, 80, 10, 80, 80, 10, 80, 'P');
		renderBuffer();
		
		cin >> action;
		
		if(action=='w'){
			cameraX+=(cos(cameraYaw)*moveSpeed);
			cameraY+=(sin(cameraYaw)*moveSpeed);

		} else if(action=='s'){
			cameraX-=(cos(cameraYaw)*moveSpeed);
			cameraY-=(sin(cameraYaw)*moveSpeed);

		} else if(action=='a'){
			cameraY-=(cos(cameraYaw)*moveSpeed);
			cameraX+=(sin(cameraYaw)*moveSpeed);
		} else if(action=='d'){
			cameraY+=(cos(cameraYaw)*moveSpeed);
			cameraX-=(sin(cameraYaw)*moveSpeed);
		} else if(action=='l'){
			cameraYaw+=(turnSpeed);
		} else if(action=='j'){
			cameraYaw-=(turnSpeed);
		}

	}
	return 0;
}
