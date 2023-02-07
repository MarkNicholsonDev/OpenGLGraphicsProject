# OpenGL Graphics Project

A showcase of a graphics engine built from scratch utilising OpenGL and various graphical techniques such as triplanar mapping, multi-texturing with bump mapping and blending and more.

See my portfolio for additional information: https://marknicholsondev.github.io/opengl-graphics

## List of Features
| Name | Description |
| ------------- | ------------- |
| Triplanar Mapping  | Used to correctly texture steep surfaces such as cliff faces  |
| Bump mapping  | Provides more accurate specular lighting effects on surfaces  |
| Scene Graph  | Used to arrange object meshes within the scene and rendering based off object transparency  |
| Blinn-Phong Lighting  | Directional light used to light the scene  |
| Anisotropic Filtering  | 16x Anisotropic filtering applied onto each texture within the scene  |
| Mipmapping & Texture Filtering  | Applied to each texture within the scene to avoid artifacts at textures in the distance  |
| Bump mapping  | Allows for more realistic specular lighting effects onto the textures |

## Controls within the scene
W – Forward
S – Backwards
A – Left	
D – Right
Q - Roll left
E- Roll right
C – Start/Stop camera demo control

## Known Bugs
There is a minor issue with the texturing on the trees with the importing of materials, these are from assets imported from Unity causing
odd discolouration in the trees specifically. This doesn't affect how the project runs or cause crashing, just a visual
issue.
