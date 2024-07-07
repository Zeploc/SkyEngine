# Sky Engine 

A custom project for developing an engine and editor for experience purposes.

# Install
- Make sure to git sync all modules
- Visual studio 2019 or higher required with v142
- The static library of `assimp-vc142-mtd.lib` will need to be compiled for wind64 and added to `\SkyEngine\Dependencies\lib\Win64\assimp`
- Download from: https://drive.google.com/drive/folders/1LyBMllvO4DkauYG2qWTw8TBa2GciBejx?usp=sharing
- TODO: Include manual assimp compile steps (Uses CMake)

# Roadmap
The current milestones and development plans can be seen in the roadmap link below

https://polite-industry-b61.notion.site/233cc000ccef456ebc2d260418678f60?v=cc8052d72b1b46af95dda8e496a0bbfc
 
# Currently supports (out of date):
## Input
- Key states
- ~~Gamepad [Rudimentary]~~
## Editor
- Editor scene/mode
- Basic movement
- ~~On screen windows to external windows and back~~
- Translation Gizmo [Rudimentary]
- Focus and rotate around selected object [Rudimentary]
- ~~Loading transforms from save [Rudimentary]~~
- Toggle Wireframe mode
## Entity
- Basic Shapes
- ~~Particle system~~
- ~~2D Particle system~~
- ~~Animated Model~~
- ~~3D Button~~
## UI
- Docking windows/widgets
- Multiple tabs on docked window
- Viewport vs UI panel input capture
## System
- Void Delegates
- Log system [Rudimentary]
## Math
- Vector templated: Vector2, Vector3, Vector 4 with types int, float and double
- Vector [Rudimentary]
- Transform [Rudimentary]
- Matrix
- Rotator [Rudimentary]
## Rendering
- Shader programs
- External shader files
- ~~Reflection [Rudimentary]~~
- ~~Cubemap~~
- ~~Fog~~
- ~~Particle Shader~~
- ~~2D Particle Shader~~
- Mesh Renderer
- Basic Shapes: Cube, Sphere, Triangle, Pyramid, Plane, ~~custom geometry shape~~
