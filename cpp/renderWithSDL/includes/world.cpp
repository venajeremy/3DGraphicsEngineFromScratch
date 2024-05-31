#include "world.hpp"

World::World(SDL_Renderer *inputRenderer, double inputCameraFov, int displayX, int displayY){
	renderer = inputRenderer;
	cameraFov = inputCameraFov;
	disX = displayX;
	disY = displayY;
}

std::tuple<double, double> World::renderPointRelative(double ix,double iy,double iz)
{
	// Given Coorinates are relative to camera
	// Fix this later (vertex is behind camera it is ignored)
	if(iz>0)
	{
		double xAngle = atan(ix/iz);
		
		double yAngle = atan(iy/iz);

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
	std::tuple<double, double> p1 = renderPointRelative(x1, y1, z1);
	std::tuple<double, double> p2 = renderPointRelative(x2, y2, z2);
	std::tuple<double, double> p3 = renderPointRelative(x3, y3, z3);

	// p1 to p2
	SDL_RenderDrawLine(renderer,std::get<0>(p1),std::get<1>(p1),std::get<0>(p2),std::get<1>(p2));

	// p2 to p3
	SDL_RenderDrawLine(renderer,std::get<0>(p2),std::get<1>(p2),std::get<0>(p3),std::get<1>(p3));

	// p3 to p1
	SDL_RenderDrawLine(renderer,std::get<0>(p3),std::get<1>(p3),std::get<0>(p1),std::get<1>(p1));	
	
}
