# Graphics Engine Built From Scratch

## What Is This?

### I have always been fascinated by 3D graphics and have always wanted to write my own renderer entirely from scratch.  

### This 3D engine is written entirely from the ground up, using only one external dependancy (SDL2) to simplify creating a window, taking user input, and drawing individual pixels to that said window.

### In its simplest form this program creates a camera that can move about a 3D world and view objects compiled into that world.

### The purpose of this project was to learn the core concepts of 3D graphics rendering and potentially isolate points of optimization for current rendering methods.  

## Limitations:

### Because this graphics engine does not use OpenGL or any other graphics library, it does not use the gpu whatsoever.  For this reason all calculations (like any other program) are done entirely on the cpu and all textures are stored in ram.  For this reason there are inherit performance limitations in the engine including: the innablilty to load large object texture files, the innability to load extreme numbers of polygons (forget wii, think gamecube graphics), and the innability to rasterize high resolutions and maintain 60 frames per second.

## Current Features:

1. Custom Rasterizer
2. Custom ZBuffer
3. OBJ + MTL file support
4. TGA texture support + Custom texture mapping
5. No shadows yet ;(  (maybe in the future)

## Design:

### The process of this graphics engine can be described in these steps:

1. Perspective Transformation
2. Rasterization
3. ZBuffer Caluclation
4. Texture Mapping

### All of which is done for every frame the engine renders.

### The following is a simplified walkthrough of the rendering process of a 3D triangle.  This provides a basic summary of the renderTriPolygon method inside of the cpp/renderWithSDL/includes/world.cpp file.

1. Perspective Transformation:  In order to display our triangle correctly on our screen we must calculate where each of its three verticies should be on our screen. Since our 3D coordinates are relative to us we can simply divide their x and y values by z (where z is distance in the forward direction) to simulate the effect of perspective. We denote these screen coordinates x' and y' (x' = x/z, y' = y/z).

2. Rasterization:  Now that we have these three points as 2D coordinates on our screen we must figure out which pixels on the screen are inside the triangle they form. Sounds easy, but hard to execute.  At the time of writing this, my code finds the slope between each of the three lines formed between the verticies and for every y value in the triangle calculates the x value on the left and right side of the triangle.  There are certainly faster rasterization methods, but in my opinion this is the most intuitive.

3. ZBuffer Calculation:  We have finished rendering our 3d triangle onto the screen. Though, what if we have another triangle that is behind our original triangle.  How will our program know which one to render and which one to hide?  To solve this issue we will use a ZBuffer.  Our ZBuffer is just an array that contains a value for every pixel in our window.  This value will represent the distance every point is from us (more accuratly from the XY plane).  With this ZBuffer we now only render a pixel on the screen if its z value is less than the current value in the z buffer.

*The calculation of the ZBuffer value of a pixel on a triangle is actually quite difficult.  We must find a formula to find the difference in z in 3D space with respect to change in x' and y' in 2D space (on the screen).  What makes this problem difficult is that the change in z with respect to screen space is not linear.  If you move one pixel across the screen the distance to a flat surface will change exponentially. Take for example this image: *
![image](https://github.com/user-attachments/assets/87fc47b2-7d0e-4b88-9912-a88dcc113217)
*Here you can see that change in distance to the wall is not constant between pixels.  To fix this we use the fact that the position of the triangle's verticies on the screen were initially found by dividing x/z and y/z to give the effect of perspective.  Because of this we can find the change in 1/z with respect to the change in pixels on the screen to get a linear equation for distance from the camera (equation of plane).*

4. Texture Mapping:  Now that we have propperly occluded our triangle we need to apply a texture.  To do this we use the uv coordinates (u:v = x:y position on texture image) provided by the obj file to map each verticy to a position on our texture.  Then, much like the z buffer we account for the z distance from the camera and find the functions for u/z with respect to x' and y' and v/z with respect to x' and y' (both of which are plane equations).  With these, for any given pixel on the screen we can find the corresponsding pixel on the texture map it should be colored too.  It would be at this position that we could implement some sort of antialiasing, but everyone knows antialiasing is lame.

## How Is This Repo Organized?

### The cpp/renderWithSDL/includes/ folder contains three files which are responsible for the majority of the rendering process:

#### World is the primary file in this folder responsible for holding all the objects and drawing them to the screen. World.cpp also manages the world's camera.  The renderTriPolygon method is the focus of this program and contains the majority of the rendering pipeline and optimizations.

#### Object contains the object class and the code for parsing obj and mtl files to create my own object objects from them.  Because this engine the world around the camera, the rotations of the camera are applied to every object in the world for every frame.  This can be seen in the getMesh method in the object.cpp file.

#### Texture contains the tgaImage class that holds the image pixel data.  It also contains the tgaReadPixel method to retrieve the color data from a specific pixel on the image. The decompressTGA file reads the raw data from tga images and converts them to my tgaImage objects which are cached in textureCache.

### The cpp/renderWithSDL/objects/ folder contains all the objects and their mtl files

### The cpp/renderWithSDL/textures/ folder contains all the textures corresponding to the objects

### The cpp/renderWithSDL/application.cpp file defines how the window should be created which objects should be added into the scene.  It also creates the world, renders it in a loop, and destroys the world when the program ends.
