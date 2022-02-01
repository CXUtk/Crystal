# Crystal Engine

## Introduction
Crystal Engine is a rendering engine that provides graphics infrastructures to complete
advanced computer graphics projects.

- Support OpenGL and DirectX 11
- Cross-Platform Design
- Asset Management
- Rendering Algorithms

## Plans
- An intermediate shader language that can be converted to glsl and hlsl
- An GUI system build with the engine
- Compute Shader and real-time ray tracing

## Architecture
```
©À©¤©¤ Crystal Engine          
©¦   ©À©¤©¤ Core			   Platform indenpendent contents, rendering systems  
|   |   ©À©¤©¤ Assets		   Resource managment
©¦   ©¦   ©À©¤©¤ Platform	   Platform interfaces, graphics API interfaces  
©¦   |   ©À©¤©¤ Input          Input controls
©¦   |   ©À©¤©¤ Render         Rendering algorithms
|   |   ©¸©¤©¤ Utils		   Utility functions, math, geometry, etc.
|   |
|   ©¸©¤©¤ Platforms          Platform dependent contents and implementations
|       ©À©¤©¤ DX11           DX11 graphics interface related implementations
|       ©À©¤©¤ GLFW		   GLFW subsystem
|		©À©¤©¤ OpenGL		   OpenGL4.5 graphics interface related implementations
|       ©¸©¤©¤ Win32		   Win32 operating system related contents
|
©¸©¤©¤ resources			   Resources
```