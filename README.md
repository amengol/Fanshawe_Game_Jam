# Game Jam  

By Jorge Amengol  
Apr 23th, 2018  

### Description

This is the the assignment for the "long-waiting" Game Jam. The Game Jam is the final assignment where students present their knowledge acquired during the GDP post-graduate course and have the chance to showcase the "toolkit" to the public, especially the Game Industry. The submission can be either a full game or a technical demo. However, some restrictions apply:  
- Must Incorporate in the project six technical aspects;  
- Using a third-party Game Engine such Unity, Unreal etc is not allowed;  

### Elevator Pitch  

**From the Graveyard to the Forest**  
Revisit one of the most iconic games of the 80's brought to you in a electrifying depict. Fight the undead and beasts as you never seen before.  

### Description of the Visual Studio Solution  

The Visual Studio solution for this assignment has two projects:  
- **GameEngine_101**: The main "Game". This is the Game Engine that has being developed through the GDP course.  
- **Physics**: The implementation of the Physics using the Bullet physics engine. It builds a `Physics.lib` file to be used in the GameEngine_101 project.  
- **PhysicsInterfaces**: Physics interface used in the GameEngine_101.  

### How to build and run  

1. Open the `GameEngine_101.sln` solution  
2. Choose one **`32 bits`** configuration (Release preferable). The 64 bits version has issues with the alpha mapping.  
4. Build the `Physics` project  
5. Build the `GameEngine_101` project  
6. Run the `GameEngine_101` project  

The GameEngine_101 project has an include path defined to $(OutDir), meaning that, in case of a rebuild of the Physics project, it will be built in the right place.  

### Controls

**Joystick**  
The Game starts with a `Third-person Camera`. To be able to control the character a controller is needed. The Only controller tested with the Game so far was the Xbox X controller. However, any Xbox controller should work fine. The joystick controls are:  

- Left Stick - Rotate and move the character  
- Right Stick - Rotate the camera  
- A - Jump  
- X - Slash Attack 1  
- Y - Slash Attack 2  
- Back/Start - Start (When on the Main Menu)  

**Free Camera**  
If `Key 1` is pressed the camera changes to `Free Camera` mode. The controls for this mode are:  

- W/S - Move camera back and forth  
- A/D - Move camera left and right  
- Q/E - Move camera up and down  
- Mouse - Changes the camera *LookAt*  

**Other keys**  

- 0 - Change to Free Camera  
- 1 - Change to Third-person Camera  
- C - Set Day/Night Cycle to STEP/CONTINUOS  
- B - Set Physics Debug Renderer mode on/off  
- F - Fog mod On/Off  
- G with Key Minus (-) and Key Equal (=) - Gama Correction +/-  
- N - Next Screen  
- M - Mute On/Off  
- P - Drop/Get Props  

### Technical Aspects 

**Day/Night Cycle and Environment control**  
Implemented a fully Day/Night cycle with:  
- Time of the day control  
- Multiple sky maps seamless blending the scene  
- Control for the Sun and Moon Light at the right position of the "Dummy Sun/Moon Texture"  
- Moon and Sun lights change colours during the cycle to match the colour of the sky  
- Shadows that also follow the Sun/Moon position (see next item)  

**Shadows**  
Implemented Shadows with transparency (foliage):  
- Using Ortho projectiom, render to a Colour Frame Buffer using the Alpha textures (foliage) of the scene  
- Using Ortho projectiom, render to a Depth Frame Buffer. Discard fragments of the Depth that are in a the alpha threshold  
- Implement PCF (percentage-closer filtering) to soft the shadows  
- Change the Shadow's camera position for rendering according to the environment control  

**Multi-layer texture**  
Implement a multi-layer texture for the ground  
- The ground is a blend of 3 textures (with normal maps too)  
- A forth texture with different texture coordinates commands the blend between them according its RGB channel. Taka a look at the `GameEngine_101\assets\textures\Ground_Overlay.bmp` to see how it looks like  

**Deffered Old-film effect and Menus**  
- Implement and "old-film" effect using a texture displacenment combined with a Sepia tone base for the main menu (that is originally coloured). This is a Deferred render   

**Wave and Fade Menu Effect**  
- Implement a fade/wave effect at the end of the last screen  

**Gama Correction**  
Gama correction implemented to get the colours right  

**Decals**  
Some decals with "dirt" textures are places throughout the scene  

**Normal, Specular and Alpha Maps**  
The normal + specular + alpha maps were havely used is the scene and are working fine  

**Physics**  
Physics is being driven by Bullet indirectly by a Physics interface  
- Implement a debug drawing using a different shader  
- Debug drawing is being called from own Bullet's DebugDrawWorld()  
- Scene with various objects as convex hulls  
- Various objects reacts to the character  
- Character implemented as a regular Bullet capsule  

**Animation**  
	Various character animations such as walk, run, jump, attacks etc.  


