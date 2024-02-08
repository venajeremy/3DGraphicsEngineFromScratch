#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

using namespace std;

const int screenWidth=100;
const int screenHeight=100;
char buffer[screenWidth*screenHeight];	// stores characters to be displayed in array
int background = 32;

// new change

void render(){
	
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
			cout << "x: " << x1+(i*run) << " y: " << y1+i << "\n";
			buffer[((y1+i)*screenWidth)+x1+(i*run)] = h;	// i*screenWidth gets right position in y, i*run stands for the x position
		}
	} else {
		for(int i=y1; i>y2; i--){
			buffer[(i*screenWidth)+(i*run)] = h;	// i*screenWidth gets right position in y, i*run stands for the x position
		}
	}
	
}

int main(){
	clearBuffer();
	renderLine(10, 25, 50, 50, '#');
	renderLine(10, 10, 95, 50, '#');
	render();
	return 0;
}
