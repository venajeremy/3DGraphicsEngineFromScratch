#include <iostream>
#include <vector>

using namespace std;

const int screenWidth=100;
const int screenHeight=100;
char display[screenWidth][screenHeight];


// new change

void render(){
	for(int j = screenHeight ; j > 0 ; j=j-1){
		//cout << "screen y: " << j << "\n";
		for(int i = 0 ; i < screenWidth ; i=i+1) {
			//cout << "screen x: " << i << "\n";
			cout << display[j][i];
			
		}	
		cout << "\n";

	}
}
void renderLine(int x1, int y1, int x2, int y2, char h){
	int m = (y2-y1)/(x2-x1);
	for(int j = screenHeight ; j > 0 ; j=j-1){
		//cout << "screen y: " << j << "\n";
		for(int i = 0 ; i < screenWidth ; i=i+1) {
			//cout << "screen x: " << i << "\n";
			if(j==(m*(i-x1))+y1){
				display[j][i] = h;
			}
		}

	}
	return;
}

int main(){
	printf("\x1b[2J");
	renderLine(10, 25, 50, 50, '#');
	renderLine(10, 10, 50, 50, '#');
	return 0;
}
