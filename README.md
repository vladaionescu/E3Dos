# E3Dos

A 3D Engine I wrote as a kid in C++ with some inline assembly.

Some interesting parts:

* [The logic for projecting and drawing triangles onto view port](https://github.com/vlad-alexandru-ionescu/E3Dos/blob/master/Sources/3DTri.cpp)
* [Rendering text on the screen (all done manually, letter by letter)](https://github.com/vlad-alexandru-ionescu/E3Dos/blob/master/Sources/Font.cpp)
* [Lighting calculations](https://github.com/vlad-alexandru-ionescu/E3Dos/blob/master/Sources/Lighting.cpp#L42)
* [The drawing surface](https://github.com/vlad-alexandru-ionescu/E3Dos/blob/master/Sources/Surface.cpp). Surface could be used as the screen or as textures. You could also render onto textures to create things like eg mirrors. This also includes the mip-mapping engine that I wrote from scratch.
* [This is how pixel coordinates were transformed to memory location](https://github.com/vlad-alexandru-ionescu/E3Dos/blob/master/Sources/Surface.h#L13)
* [This is how each pixel was plotted](https://github.com/vlad-alexandru-ionescu/E3Dos/blob/master/Sources/Surface.h#L26)
* [This is where the projection math happens](https://github.com/vlad-alexandru-ionescu/E3Dos/blob/master/Sources/ViewPort.h). Back then I only had internet once per week, so I made these calculations myself, from first principles. Later found out that people use matrix math for this stuff, which is more elegant.

Below is the introductory text & screenshots from that time.


### E3Dos Graphics Engine

E3Dos is a software rendering engine that I started programming in the beginning of 2004. Even though the engine has only about 11000 lines of code, I have written and rewritten (reorganized) each part of it at least twice. The sources are tidy (except for “3D.cpp”, the main file, which only applies the features of the engine), but most of them are uncommented.

The program has no immediate practical use (maybe only didactical). I have written it in order to understand the technology in the back of the high-end engines on the game market.

The programming language used is C++ (with some inline assembly procedures). The sources have been compiled with OpenWatcom 1.3 (uses Dos/4GW Rational Systems, protected mode, as target).

Please read pipeline.txt for an idea of the graphics processing stages.

You can look at some screen shots or clips in the “Screenshots and Clips” folder.

The older version “.\Exe\Ride (From old version).exe” should work on any card (it uses only 8-bit colors). Triangles weren’t implemented at that time so it uses only lines.

### Features

```
• Geometry:
    ◦ 2D Vectors
    ◦ 3D Vectors
• Mathematics
    ◦ Fixed Point Calculation (Optimization) (Not integrated with the graphics engine, yet)
• Graphics
    ◦ 2D Graphics
        ▪ Antialiasing (5 pixel mean)
        ▪ Vertical Retrace Waiting
        ▪ Double Buffering
        ▪ Graphics Modes
            • 10fh (320x200, 24-bit)
            • 112h (640x480, 24-bit)
            • 115h (800x600, 24-bit)
            • 118h (1024x768, 24-bit)
        ▪ Simple Motion Blur (No Pixel Speed Calculation)
        ▪ Two Types of Alpha Blending
        ▪ Transition Effects
        ▪ Fade In/Out
        ▪ Line Tracing
        ▪ Fast Pixel Plotting
        ▪ 24-bit Bitmap Loading and Saving Possibility (only with height and width divisible by 4)
        ▪ Font (loading from a bitmap)
        ▪ Progress Bar
    ◦ 3D Graphics
        ▪ Z Buffering
        ▪ Alpha Buffering
        ▪ Linear Fog
        ▪ Stereogram Support
        ▪ 3D Line With Specific Mesh
        ▪ 3D Triangle With Specific Meshes
            • No Jittering Effects (Good Filling Code)
            • Gouraud-Shading with Ambient, Diffuse and Specular Lighting (Supports HDR (High Dynamic Range) Lighting – Over Bright etc.) 
            • Texture Mapping
            • Rendering on Textures
            • Texture Tiling
            • Perspective Correction
            • 3D and 2D Clipping (3D clipping is not 100% proof)
            • Culling (Not 100% proof; might generate jittering, but this happens rarely)
            • Mip Mapping
            • Alpha Blended Triangles
            • Selects Rendering Mode depending on the distance of the object from the camera, resulting in a good Quality / Performance balance
            • Texture Filtering
                ◦ Bilinear Filtering
                ◦ Trilinear Filtering
            • Random Height Terrain Generator With Normals
            • Displacement Mapping (Based on Bump Bitmaps; only from flat surfaces)
        ▪ Mesh Binary I/O
• Input
    ◦ Keyboard
    ◦ Mouse (2 Buttons, No Pointer)
• Memory
    ◦ Upper Memory Allocation
        ▪ Simple Allocation Methods
        ▪ Allocation With No Page Boundary Crossing (Optimization Trick)
    ◦ Lower Memory Allocation
• Random Generator
    ◦ long Values Generator
    ◦ float Values Generator (based on fixed point)
    ◦ Generator based on Distribution Curve
• SoundBlaster
    ◦ WAV Files Compatibility
    ◦ SND Files Compatibility
    ◦ Up To 44100 Samples/Second (~ 44.1 KB/s)
    ◦ Doesn’t Affect Rendering Performance (doesn’t do mixing)
    ◦ Plays Sounds That May Occupy the Entire RAM
    ◦ Auto Detects IRQ, Port and DMA
• Time Calculation
    ◦ Simulates Higher Clock Resolution (the system clock only has 18.2 Hz resolution)
    ◦ Calculates FPS
• Logging (in a log file)
```

### Gallery

![Alpha Buffering](Gallery/Alpha%20Buffering.png)
![AntiAliased Gouraud-Shaded Textured Height Field](Gallery/AntiAliased%20Gouraud-Shaded%20Textured%20Height%20Field.png)
![Displacement Mapping, Bilinear Filtering](Gallery/Displacement%20Mapping%2C%20Bilinear%20Filtering.png)
![Linear Fog](Gallery/Linear%20Fog.png)
![The Moon Was High.png](Gallery/The%20Moon%20Was%20High.png)
![Triangle Mesh.png](Gallery/Triangle%20Mesh.png)
![Trilinear Filtering.png](Gallery/Trilinear%20Filtering.png)

![Gallery/High%20Resolution%202.png](Gallery/High%20Resolution%202.png)

![Gallery/High%20Resolution%206.png](Gallery/High%20Resolution%206.png)

![Gallery/High%20Resolution%207.png](Gallery/High%20Resolution%207.png)

![Logo](Gallery/Logo%202.png)

### Videos

[![Bilinear Filtering, Linear Fog, Displacement Mapping](https://img.youtube.com/vi/15lonNlcoho/0.jpg)](https://www.youtube.com/watch?v=15lonNlcoho)

[![Height Field and Mesh Demo](https://img.youtube.com/vi/4w3oeQvUnC0/0.jpg)](https://www.youtube.com/watch?v=4w3oeQvUnC0)
