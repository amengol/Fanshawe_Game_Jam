# Graphics1 Project 2

Group:

Euclides Araujo Jr.
Jorge Amengol.

### How to build

Uncompress the file and run the GameEngine_101.sln solution file. Prefer to run in x64 Release mode, because the AABBs (faster).

### Description of the scene

This is a large complex scene with circa 600k vertices, lots of lights, textures, transparences, reflections, specular etc. Also there is an AABB system and a "simpleDebugRender" active, which means it can take a little longer to load the scene up. Running in Debug mode can take up to 3 minutes to calculate the AABBs, however, running in Release mode only takes about 12 seconds. If the debug mode is needed, freeing some meshes from the calculation will speed up the load. The suggestion is to edit the \_models.json that is in the GameEngine_101\assets\models folder, putting a '#' in front of the lines that have "heavy" models. There is a comment there showing them.

### Camera and Game Objects controls

The camera system and the GameObjects have the same controls and principals:

W/A/S/D -> moves along its LOCAL XZ axes (forard, backward, to the left, to the right);
Q/E -> moves along the GLOBAL Y axis (up and down);
Z/C -> rotates along the Z axis (bank left/right);
Arrows -> rotates along XY axes (yaw and pitch);

The scene have 3 controllebles GameObjects:
1. Helicpter01: press number 1 to control it;
2. Helicpter02: press number 2 to control it;
1. Delorean: press number 3 to control it;

To get back to the camera mode, press number 0;

### Debug AABB mode

When an object is selected, it is possible to enable/disable the Debug AABB mode by pressing 'L'. It will show the AABB boundary in RED, the normals in BLUE and the triangles facing the object in GREEN. It will only show them if the object is close enough to the geometry.

### Grading Scheme

# Transparency: Multiple amounts of blend transparency (different variations between transparent and opaque)

The transparency is working with the "clouds" and the textures have a gradient from pure black to pure white (variations between transparent and opaque). The trees and the helicopters' rotors are using the same techinique.

# Transparency: Multiple objects (so they must be drawn last, and sorted from back to front relative to the camera)

The clouds and the helicopters are being sorted by the `cTransparencyManager` Class. They are being drawn accordingly.

# "discard" transparency present (this is with the "discard" GLSL instruction)

As there are a huge number of trees, this techic was used to discard transparency values less than 0.5.

Here is the code in the frag shader:
```
	// Copy object material diffuse to alpha
	if (hasAlpha)
	{
		if (useDiscardAlpha)
		{
			if (texCol01.r < 0.5)
				discard;
		}
		fragColourOut.a = texCol01.r;
	}
	else
	{
		fragColourOut.a = materialDiffuse.a;
	}
```

# At least three models must have a combination of two (or more) textures, but:
* They must be different textures
* An example of "trivial combining" of the different textures (like "modulating" them, adding them, or an equivalent).
* A more involved "combining" of the textures must be different (using one to change the colour of the other, using one to change the transparency of the other, implementing discard transparency, changing the normal, specular value, etc.).

The helicopters have a combination of three textures (diffuse, alpha and reflection).
The trees have a combination of two textures (diffuse and alpha). They are using `discard` transparency as shown before.

# Lots and lots of lights (more than you've used before), as well as at least types (at least a “point light” type and a “spot light” type, or equivalents of those).

The scene has 16 lights active:
5 Per each helicopter (1 spot and 4 omni per each)
6 for the delorean (6 omini)
1 Sun (1 omini)

All lights can be have their parameters changed in the \_Scene.json file. A maximum of 20 is allowed.

# Must have things
* A "decent" number of models
There is a significant large number of models.

* A "decent" number of variations of models (depending on the scene)
There is a good variation in the scene.

* Interactive camera (of some kind, or a camera that follows the user, or whatever)
The camera is controlleble in all axis and can move around the scene.

* External configuration file
There are two JSON configuration files:
\assets\models\_models.json - loads the meshes of the scene.
\_Scene.json - laods GameObjects, Camera parameters and lights parameters.
Almost all elements of the scene are being loaded by those files.

* The scene has to “make sense” (more details later).
It is a large city (paris), with two helicopters (patrolling?) and a unexpected time travaller (the delorean). Makes sense? :)

* Must be at least four (4) different “shaders” present. This can be separately loaded shaders, or as a single “uber shader” (where there are very different “techniques” being used, selected by a switch or if statement(s). “Different” means things like: lighting, no-lighting, textures only, skybox, textures + lighting, etc.
One "uber" fragment shader was used. It can handle:
- Objects without textures with the colours in the ply file (vertex colours);
- Objects without textures with the colour indicated as the diffuse material;
- Textured objects without alpha
- Textured objects with alpha
- Textured objects with alpha and reflection
- SkyBox objects

### Known issues

Somehow the textures are not being loaded by some Nvidia cards. This is being adressed.