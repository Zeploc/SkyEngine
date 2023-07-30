# Sky Engine 

A custom project for developing an engine and editor for experience purposes.

# Install
- The static library of `assimp-vc143-mtd.lib` will need to be compiled for wind64 and added to `\Engine\Dependencies\assimp`
 
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
- Vector templated: Vector2, Vector3, Vector 4 with types int, float and double
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

# Next Steps
- Complete Saving/Loading entities from save
- Move Hit check to mesh instead of entity and account for rotation
- Fix/Complete translating entities in editor
- Rotate + Scale entities in editor
- Change editor movement/look to state based input + Improve "Spectator" controls structure
- Complete basic Vector/ Rotator and Quat math functions
- Add Transform math functions
- Link up cubemap
- Multi Param Delegates
- Switch input to delegates
- Pyramid Lighting fix
- Play editor level
- Dynamic link engine (and game for editor) when working on project, static link when building the game as an exe
- Link dependencies PDBs
- Update dependencies to recent versions
- Update VS project to latest
- Add release binaries (Fix 32?)
- Fix assimp model loading for DAE (update to fbx?)
- Try finder lighter option for assimp static library
- Confirm soil2 is a good option
- Add back vld

# Further Development

## Editor
- Content Panel
- Open and Save level
- Play level
- Details panel
- Rotate
- Fix up translate gizmo movement
- Scale
- Shader/Material editor
- Editor Settings
## Examples
- Reflection
- Refraction
- Standard Lighting
- Basic Shapes
- Custom Model
- Animated Model
- 3D Collision
- 3D Particles
- Cubemap
- 2D Sprites + View
- 2D Collision
- 2D Particles
## Production Cycle
- Ability to inject (hot reload) game into editor when playing
- Differientate development vs shipping
## Systems
- Custom Pointer Wrapper type
- Log system
- Delegates create multiple parameters with templates
- Create macro to define custom multi param delegates
## UI
- UI Editor - creator for ImGui
- Widget system -ImGui
- Save and load widgets
## Save and Loading
- Save Mesh Type
- Save Specific properties
 - Save Particles
## Rendering
- Setup placeholder for DirectX and Vulkan
- Generic shader system to use across rendering pipeline
- No Depth test render order
- Live shader compiling/editing (hot reload shaders)
## Component system
- Base Component
- Mesh Component
- Skeletal Mesh Component
## Math Libraries
- Transform operations
- Quaternion operations
- Rotator operations
- Vector math Helpers (project point, plane, etc)
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
