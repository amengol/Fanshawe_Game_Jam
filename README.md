# Physics1 Project 2

Group:

Euclides Araujo Jr.
Jorge Amengol.

### How to build

Uncompress the file and run the GameEngine_101.sln solution file. Prefer to run in x64 Release mode, because the AABBs (faster).

### Description of the scene

This is a large complex scene with circa 600k vertices, lots of lights, textures, transparencies, reflections, specular etc. Also there is an AABB system and a "simpleDebugRender" active, which means it can take a little longer to load the scene up. Running in Debug mode can take up to 6 minutes to calculate the AABBs, however, running in Release mode only takes about 12 seconds. If the debug mode is needed, freeing some meshes from the calculation will speed up the load. The suggestion is to edit the \_models.json that is in the GameEngine_101\assets\models folder, putting a '#' in front of the lines that have "heavy" models. There is a comment there showing them.

### Camera and Game Objects controls

The camera system now has three modes: NORMAL, CONTROL and FOLLOW  

For the NORMAL and CONTROL modes, the GameObjects have the same controls and principals:

W/A/S/D -> moves along its LOCAL XZ axes (forward, backward, to the left, to the right);
Q/E -> moves along the GLOBAL Y axis (up and down);
Z/C -> rotates along the Z axis (bank left/right);
Arrows -> rotates along XY axes (yaw and pitch);

The FOLLOW mode activates the Newtonian forces of the GameObjects and position the camera right behind them, so it can be controlled. The controls are:

W/S -> Increases, decreases the speed of the GameObject' (vel.z);
Q/E -> Increases, decreases the speed of the GameObject' in the Y axis (vel.y);
A/D -> Increases, decreases rotational speed of the GameObjects along the Y axis;
Arrow Up/Down -> Increases, decreases rotational speed of the GameObjects along the X axis;
Arrow Left/Right -> Increases, decreases rotational speed of the GameObjects along the Z axis;

Change camera modes with number 9 **after selecting a GameObject**. In other words, the camera won't move to a `third person view` of the object selected until you **press 9**.

The scene have 3 controllable GameObjects:
1. Helicpter01: press number 1 to control it;
2. Helicpter02: press number 2 to control it;
3. Delorean: press number 3 to control it;

**Important**: Only the Delorean has collision contact points. To be able to see them, select the delorean and manipulate the GameObject to get "inside" a geometry. To better see the effect, use the CONTROL mode that you can change the GameObject's orientation without Newtonian forces. However, to see the `Collision Detection` working, change to FOLLOW mode and "colide" with something. All velocities will be clamped to zero, including rotations. After colliding, you can maneuver to get out of the collision. Until you collide the GameObject again.

To get back to the camera NORMAL mode, press **number 0**;

### Debug AABB mode

When an object is selected, it is possible to enable/disable the Debug AABB mode by pressing 'L'. It will show the AABB boundary in RED, the normals in BLUE and the triangles facing the object in GREEN. It will only show them if the object is close enough to the geometry (inside an AABB).

### Grading Scheme

**Environment is a “complex” 3D model of at least 100K triangles or several million vertices (you get 0% if you have “tiny” models).**  
The environment is a complete set of a city hundreds of building and geometry. The entire scene has more than 500k vertices.

**Entity (i.e. spaceship, plane, etc.) is composed of at least 3K triangles or 30K vertices.**  
The Delorean, the GameObject used for the collision has 23357 triangles. It can be checked at the Delorean_Textured.ply file.

**Collision detection is implemented showing collision between environment and entity**  
The collision geometry is being shown every time the Delorean penetrates a geometry. Almost the entire scene (more than 1km squared) has collision geometry. The exceptions are the clouds, the helicopters and the trees. The trees were turned off because they are only crossed planes and the alpha channel could lead the user to an impression that he is missing the a tree but still colliding (with the plane). But there is hundreds of thousands of triangles in the scene to collide.

**Collisions cause entity to stop (i.e. velocity is clamped) in the direction of the collision. In other words, if the ship is flying down (y-axis) and forward (z-axis) while hitting the “ground” (i.e. y-axis), the ship will continue to move forward (along z-axis).**  
When colliding with some geometry (FOLLOW camera mode active), all speeds are clamped to zero, even the rotational ones.

**A hierarchical AABB (or equivalent) is implemented in at least one of the models (you can do this for both, or you can use another technique for your “moving”/player object – see point below about the “10% of your model”). This can be done at run-time (i.e. when the models are loaded) or as a “pre-process” (i.e. done off-line with the additional data saved). Note: If this is done as a pre-process, you must submit the code that does the generation.**
An `cAABBsManager` were created to handle the generation of AABBs for all the meshes that are supposed to have collision. All the AABBs are generated during the load processes of the scene and stored at that entity. There is no AABB where there is no mesh and each AABB has a unique ID.

The system works like this:  
At runtime the max extend of a GameObject are calculated and its geometry is virtually put inside a box. This process can be seen inside `Physics.cpp` around line 199 on.
```c++
/ Take the mesh extents
                cMesh theMesh;
                g_pVAOManager->lookupMeshFromName(pCurGO->meshName, theMesh);
                float diameter = theMesh.maxExtent;

                // Vertices                
                glm::vec3 vertices[8];
                vertices[0] = glm::vec3(pCurGO->position - pCurGO->radius);
                vertices[1] = glm::vec3(vertices[0].x + diameter, vertices[0].y, vertices[0].z);
                vertices[2] = glm::vec3(vertices[0].x, vertices[0].y + diameter, vertices[0].z);
                vertices[3] = glm::vec3(vertices[0].x + diameter, vertices[0].y + diameter, vertices[0].z);
                vertices[4] = glm::vec3(vertices[0].x, vertices[0].y, vertices[0].z + diameter);
                vertices[5] = glm::vec3(vertices[0].x + diameter, vertices[0].y, vertices[0].z + diameter);
                vertices[6] = glm::vec3(vertices[0].x, vertices[0].y + diameter, vertices[0].z + diameter);
                vertices[7] = glm::vec3(vertices[0].x + diameter, vertices[0].y + diameter, vertices[0].z + diameter);
```
After that, for each vertex of this "bounding box", an AABB ID is calculated. Then a check to see if there is an AABB with that ID is made and in case it has all the collision detection process continues. This is a kind of a `Broad Phase` of the Physics step.  
If there is an AABB, the contact points of the GameObject are tested against each triangle of that AABB. If it is intercepting any triangle, all speeds are clamped to zero, even the rotations.   
The collision geometry were added as meshes to the scene as can be seen at the `_models.json` and `_Scene.json`. The physics step puts them in the right place related to the object. They can been seen when a collision occurs.

**Collision details are shown on screen at an appropriate detail (i.e. if your spaceship hits a wall, only the part hitting the wall is highlighted (at most, this is 10% of your model).**
The contact points lit in red when they are intercepting a collision geometry. It is also possible to vizualize the AABBs (in red), the triangles (in green) and the normal (in blue) by **pressing L** in only **if inside and AABB**;


**Entity is controllable by the user in three translational degrees of freedom (x,y,z)**  
See the description of the controls before in this document. In the CONTROL mode it has all the axis active.

**BONUS: Entity has three rotational degrees of freedom (controllable by the user)**  
See the description of the controls before in this document. In the CONROL and FOLLOW mode it has all rotational degrees of freedom.


**Motion is controlled by “forces” rather than direct updates. Think “thrusters” in a space ship or the engines of a plane that “pushes” the object forward (Note: We will be looking into this in more detail soon, but the effect of “force” can be mimicked by controlling the acceleration of the entity; i.e. if you go “forward” ).**
The FOLLOW camera mode triggers Newtonian updates of the GameObject (by varing it various velocities). Change the velocity is literally the same thing of acceleration (change velocity over time). The other approach is to translate or rotate the object directly. This can be done by the CONTROL camera mode.

**Movement is simulated in discrete, real-time step (i.e. independent of frame rate).**  
This is done my the `deltatime` variable. All physics reactions are based on that.

**Detects a reasonably small (approx 10% size of ship or smaller) collision detection point – it is clear that a portion of the character has collided with world**  
The contact points are being drawn in red when colliding with a geometry (exception of the clouds, helicopters and trees as already mentioned).

**Collision point identified on screen somehow (colour change, explosion, etc.)**  
Similar to the last one. They are being drawn accordingly.

**Bonus: Some additional item to assist in locating the characters location in the world (lighting, shadows, point in the environment, etc.)**  
The Delorean is right in front the camera. In case the user wants to go back to it (say after exploring  the scene), just **press 3** to select it and **9** to change to the camera FOLLOW mode.

