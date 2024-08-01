# Graphics Engine Built From Scratch

## What The Hell Is This?

### I have always been fascinated by 3D graphics and have always wanted to write my own renderer entirely from scratch.  

### This 3D engine is written entirely from scratch only using one external dependancy (SDL2) to simplify creating a window, taking user input, and drawing individual pixels to that said window.

### In its simplest form this program creates a camera that can move about a 3D world and view objects compiled into that world.

### The purpose of this project was to learn the core concepts of 3D graphics rendering and potentially isolate points of optimization for current rendering methods.  

## Limitations:

### Because this graphics engine does not use OpenGL or any other graphics library it does not use the gpu whatsoever.  For this reason all calculations (like any other program) are done entirely on the cpu and all textures are stored in ram.  For this reason there are inherit performance limitations in the engine including: the innablilty to load large object texture files, the innability to load extreme numbers of polygons (forget wii, think gamecube graphics), and the innability to rasterize high resolutions and maintain 60 frames per second.

## Current Features:

1. Custom Rasterizer
2. Custom ZBuffer
3. OBJ + MTL file support
4. TGA texture support + Custom texture mapping
5. No shadows yet ;(  (maybe in the future)

## What Exacatly Does My Code Do?

### The process of this graphics engine can be described in these steps:

1. Perspective Transformation
2. Rasterization
3. ZBuffer Caluclation
4. Texture Mapping

### All of which is done for every frame the engine renders.

### For sake of explanation we will be walking through rendering a simple 3D triangle.  A basic summary of the renderTriPolygon method inside of the cpp/renderWithSDL/includes/world.cpp file.

### We must imagine we are a camera in empty space and all are given is three points in space relative to us that form a triangle.

### Perspective Transformation:  In order to display our triangle correctly on our screen we must calculate where each of these three points should be on our screen. Since our 3D coordinates are relative to us we can simply divide their x and y values by z (the distance they are away from us) to simulate the effect of perspective.

### Rasterization:  Now that we have these three points as 2D coordinates on our screen we must figure out which pixels on the screen are inbetween them. Sounds easy, but hard to execute.  At the time of writing this, my code finds the slope between each of the three lines formed between the verticies and for every y value in the triangle calculates the x value on the left and right side of the triangle.  There are certainly faster rasterization methods, but in my opinion this is the most intuitive.

### ZBuffer Calculation:  Great, we have rendered our 3D triangle onto the screen! Though, what if we have another triangle that is behind our original triangle.  How will our program know which one to render and which one to hide?  For this we will use a ZBuffer.  Our ZBuffer is just an array that contains a number for every pixel in our window.  This number will be the distance every point is from us (more accuratly from the XY plane).  With this ZBuffer we now only render a pixel on the screen if its z value is less than the current value in the z buffer.

### Texture Mapping: Continue later....

### Texture Mapping:  

## How Is This Repo Organized?

### The cpp/renderWithSDL/includes/ folder contains three files which are responsible for the majority of the rendering process:
#### World.cpp is the primary file in this
