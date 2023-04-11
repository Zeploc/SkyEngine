# Graphics-Combination

A custom project for developing an engine and editor for experience purposes.

# Currently supports:
## Input
- Key states
- Gamepad [Rudimentary]
## Editor
- Editor scene/mode
- Basic movement
- On screen windows to external windows and back
- Translation Gizmo [Rudimentary]
- Focus and rotate around selected object [Rudimentary]
- Loading transforms from save [Rudimentary]
- Toggle Wireframe mode
## Entity
- Render system
- Basic Shapes
- Particle system
- 2D Particle system
- Animated Model
- 3D Button
## UI
- Basic UI to screen
- Slider
- Buttons
- Text Field
- Image
## System
- Basic Utils
- Void Delegates
- Log system [Rudimentary]
## Math
- Vector [Rudimentary]
- Transform [Rudimentary]
- Matrix
- Rotator [Rudimentary]
## Rendering
- Shader programs
- External shader files
- Reflection [Rudimentary]
- Lit Shader
- Cubemap
- Fog
- Particle Shader
- 2D Particle Shader
- Mesh Renderer
- Basic Shapes: Cube, Sphere, Triangle, Pyramid, Plane, custom geometry shape
## Physics
- 2D Collision
- Basic 3D Collision
- Collision Bounds

# Further Development

- Rename > SkyEngine?

## Editor
- Content Panel
- Open and Save level
- Play level
- Details panel
- Rotate
- Fix up translate gizmo movement
- Gizmo always draw infront
- Scale
- Shader/Material editor
## Production Cycle
- Seperate editor and project (Seperate dlls)
- Ability to inject game into editor when playing
- Differientate development vs shipping
## Systems
- Log system
- Delegates create multiple parameters with templates
- Create macro to define custom multi param delegates
## UI
- UI Editor
- Widget system
- Save and load widgets
## Save and Loading
- Save Mesh Type
- Save Specific properties
 - Save Particles
## Rendering
- Abstract rendering to back end
- Split open gl to one rendering pipeline
- Setup placeholder for DirectX and Vulkan
- Generic shader system to use across rendering pipeline
## Component system
## Math Libraries
- Transform operations
- Quaternion operations
- Rotator operations
- Vector math Helpers (project point, plane, etc)
- Switch vector to templated system for type with base functions
## Memory Management
- Custom memory allocation
- Memory types
- Memory record
- Development vs shipping memory handled
## Input
- Switch/Expose to delegates
- Player index system for local
## Physics
- Link physics to component/mesh
- Abstract physics
- Link PhysX?
- Option to switch to Box 2D
