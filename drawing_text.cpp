#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;


// Camera
float cameraX = -100;
float cameraY = 0;
float cameraZ = 0;
float cameraYaw = 0;
float cameraPitch = 0;
float cameraRoll = 0;
float fov = 3.14/2;

// Screen/Render
const int screenWidth=100;
const int screenHeight=100;
char buffer[screenWidth*screenHeight];	// stores characters to be displayed in array
int background = 32;	// blank character to fill spaces of buffer


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

void clearBuffer(){
	memset(buffer, background, screenWidth * screenHeight);
}
void renderLine(int x1, int y1, int x2, int y2, char h){
	int dy = (y2-y1);
	int dx = (x2-x1);
	int run = dx/dy;
	if(dy>0){	// if line is going up
		for(int i=0; i<dy; i++){
			buffer[((y1+i)*screenWidth)+x1+(i*run)] = h;	// i*screenWidth gets right position in y, i*run stands for the x position
		}
	} else {
		for(int i=0; i>dy; i--){
			buffer[((y1+i)*screenWidth)+x1+(i*run)] = h;	// i*screenWidth gets right position in y, i*run stands for the x position
		}
	}
	
}

void renderQuad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, char h){

	renderLine(x1, y1, x2, y2, h);
	renderLine(x2, y2, x3, y3, h);
	renderLine(x3, y3, x4, y4, h);
	renderLine(x4, y4, x1, y1, h);

}

int renderX(int x, int y, int z){
	int dX = x-cameraX;	// change in x between camera and object
	int dY = y-cameraY;	// change in y between camera and object
	float alpha = atan2(dY, dX);// universal angle between camera and object (atan2 function is weird first input is rise, run)
	float beta = cameraYaw;		// universal angle of camera
	float direction = alpha-beta;   // direction of object from perspective of camera
					//
	cout << "returning x: " << int((direction/(fov/2))*(screenWidth/2)) << " for: ("<<x<<","<<y<<","<<z<<")\n"; 
	// return converted angle to screen position in x
	return(int((direction/(fov/2))*(screenWidth/2))+(screenWidth/2));
}

int renderY(int x, int y, int z){
	int dX = x-cameraX;	// change in x between camera and object
	int dY = y-cameraY;	// change in y between camera and object
	int dZ = z-cameraZ;	// change in z between camera and object
	float dXY = sqrtf((dX*dX)+(dY*dY));	// distance on xy plane between camera and object
	float alpha = atan2(dZ, dXY);
	float beta = cameraPitch;
	float direction = alpha-beta;
	
	// return converted angle to screen position in y
	cout << "returning y: " << int((direction/(fov/2))*(screenHeight/2)) << " for: ("<<x<<","<<y<<","<<z<<")\n"; 
	return(int((direction/(fov/2))*(screenHeight/2))+(screenHeight/2));
}

void renderQuadThr(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int x4, int y4, int z4, char h){
	
	


/*
	renderQuad();
	renderQuad();
	renderQuad();
	renderQuad();
*/

}

int main(){

	clearBuffer();
	
	renderLine(renderX(0, -20, 15), renderY(0,-20, 15), renderX(0, 80, -20), renderY(0, 80, -20), '#');
	//renderLine(16, -11, -12, 9, 'T');
	renderQuad(10, 15, 80, 25, 75, 85, 10, 80, 'P');
	renderBuffer();
	return 0;
}
