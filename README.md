# Graphics Engine Built From Scratch

## What Is This?

### I have always been fascinated by 3D graphics and have always wanted to write my own renderer entirely from scratch.  

### This 3D engine is written entirely from the ground up, using only one external dependency (SDL2) to simplify creating a window, taking user input, and drawing individual pixels to that said window.

### In its simplest form this program creates a camera that can move about a 3D world and view objects compiled into that world.

### The purpose of this project was to learn the core concepts of 3D graphics rendering and potentially isolate points of optimization for current rendering methods.  

## Limitations:

### This graphics engine does not utilize OpenGL or any other graphics libraries and does not use the GPU for its rendering process.  For this reason all calculations are done entirely on the cpu and all textures are stored in ram.  For this reason there are inherent performance limitations in the engine including: the inability to load large object texture files, the inability to load extreme numbers of polygons (forget wii, think gamecube graphics), and the inability to rasterize high resolutions and maintain 60 frames per second.

## Current Features:

1. Custom Rasterizer
2. Custom ZBuffer
3. OBJ + MTL file support
4. TGA texture support + Custom texture mapping
5. No shadows yet ;(  (maybe a future addition)

## Compilation:

### build.txt includes GNU compilation commands for windows and linux to include the SDL2 libraries.  SDL2 will have to be configured beforehand.

### New obj objects can be added by including the obj and mtl files within the objects folder following the same naming scheme as the already present objects.  Same goes for textures.  This renderer so far only supports triangle polygons, so before importing your own object load it into blender and triangulate all faces by pressing ctrl-t while in edit mode.  Once the files are properly included create an Object object with the file name its world position and its rotation into the cpp/renderWithSDL/application.cpp default constructor.  Then add the Object to the World by called the world's addObject Method (see the blocks I have already created to form the default world).  Lastly save the application.cpp file and recompile the engine using the build.txt commands.

### **Since I wrote my own parser for obj, mtl, and tga files there are many many edge cases in the file type's fine print that I did not consider, writing parsers was not the purpose of this project so I did not care to spend time making them perfect.  I would recommend looking at the objects I have already created and their textures and following their same design.  Most of the objects I have included in the repo should load correctly.**

## Design:

### The process of this graphics engine can be described in these steps:

1. Perspective Transformation
2. Rasterization
3. ZBuffer Calculation
4. Texture Mapping

### All of which is done for every frame the engine renders.

### The following is a simplified walkthrough of the rendering process of a 3D triangle.  This provides a basic summary of the renderTriPolygon method inside of the cpp/renderWithSDL/includes/world.cpp file.

1. Perspective Transformation:  In order to display our triangle correctly on our screen we must calculate where each of its three vertices should be on our screen. Since our 3D coordinates are relative to us we can simply divide their x and y values by z (where z is distance in the forward direction) to simulate the effect of perspective. We denote these screen coordinates x' and y' (x' = x/z, y' = y/z).

2. Rasterization:  Now that we have these three points as 2D coordinates on our screen we must figure out which pixels on the screen are inside the triangle they form. Sounds easy, but hard to execute.  At the time of writing this, my code finds the slope between each of the three lines formed between the vertices and for every y value in the triangle calculates the x value on the left and right side of the triangle.  There are certainly faster rasterization methods, but in my opinion this is the most intuitive.

3. ZBuffer Calculation:  We have finished rendering our 3d triangle onto the screen. Though, what if we have another triangle that is behind our original triangle.  How will our program know which one to render and which one to hide?  To solve this issue we will use a ZBuffer.  Our ZBuffer is just an array that contains a value for every pixel in our window.  This value will represent the distance every point is from us (more accuratly from the XY plane).  With this ZBuffer we now only render a pixel on the screen if its z value is less than the current value in the z buffer.

*The calculation of the ZBuffer value of a pixel on a triangle is actually quite difficult.  We must find a formula to find the difference in z in 3D space with respect to change in x' and y' in 2D space (on the screen).  What makes this problem difficult is that the change in z with respect to screen space is not linear.  If you move one pixel across the screen the change in distance to the flat surface you are rendering will not be constant. Take for example this image:*

![image](https://github.com/user-attachments/assets/87fc47b2-7d0e-4b88-9912-a88dcc113217)

*Here you can see that change in distance to the wall is not constant between pixels.  To interpolate the depth linearly with respect to screen space, we use the fact that the screen coordinates x' and y' where found by via their 3D coordinates divided by their depth: x' = x/z, y' = y/z.*
*We know that in 3 space our flat surface satisfies the equation of a plane ax + by + cz = d, but we need some way to find z in this equation only given screen coordinates x' and y'.  Or in other words, if given a pixel on the screen we a way to find the distance to the polygon at that point.  a, b, c, and d can all be calculated via cross product and a single point, so we are left with two unknowns: x and y.  z being our desired output.*

*Luckily we have two equations that we can use.  Since x' = x/z and y' = y/z, x = x'z and y = y'z.  We can plug into our plane equation we know our polygon satisfies to get the equation ax'z + by'z + cz = d.  Now, since all these z's represent the same z value of a single point we can factor them all out getting us: z (ax' + by' + c) = d.  Meaning: z = d / (ax' + by' + c).  Or most notably: (ax' + by' + c) / d = 1/z*

*This may not seem very useful but this solution shows us that the plane equation of x' and y' in screen space is a linear function of the **reciprocal** of z.  With this knowledge we can easily find the z distance of any point on a surface only knowing the screen pixel that is viewing it.  And by compairing these z distances we can easily figure out which surfaces are closer to us than others and only render pixels that are visible to us.*

*To fix this we use the fact that the position of the triangle's vertices on the screen were initially found by dividing x/z and y/z to give the effect of perspective.  Because of this we can find the change in 1/z with respect to the change in pixels on the screen to get a linear equation for distance from the camera (equation of plane).*

4. Texture Mapping:  Now that we have propperly occluded our triangle we need to apply a texture.  To do this we use the uv coordinates (u:v = x:y position on texture image) provided by the obj file to map each verticy to a position on our texture.  Then, much like the z buffer we account for the z distance from the camera and find the functions for u/z with respect to x' and y' and v/z with respect to x' and y' (both of which are plane equations).  With these, for any given pixel on the screen we can find the corresponding pixel on the texture map it should be colored too.  It would be at this position that we could implement some sort of anti-aliasing.

## How Is This Repo Organized?

### The cpp/renderWithSDL/includes/ folder contains three files which are responsible for the majority of the rendering process:

#### World is the primary file in this folder responsible for holding all the objects and drawing them to the screen. World.cpp also manages the world's camera.  The renderTriPolygon method is the focus of this program and contains the majority of the rendering pipeline and optimizations.

#### Object contains the object class and the code for parsing obj and mtl files to create my own object objects from them.  Because this engine the world around the camera, the rotations of the camera are applied to every object in the world for every frame.  This can be seen in the getMesh method in the object.cpp file.

#### Texture contains the tgaImage class that holds the image pixel data.  It also contains the tgaReadPixel method to retrieve the color data from a specific pixel on the image. The decompressTGA file reads the raw data from tga images and converts them to my tgaImage objects which are cached in textureCache.

### The cpp/renderWithSDL/objects/ folder contains all the objects and their mtl files

### The cpp/renderWithSDL/textures/ folder contains all the textures corresponding to the objects

### The cpp/renderWithSDL/application.cpp file defines how the window should be created which objects should be added into the scene.  It also creates the world, renders it in a loop, and destroys the world when the program ends.


