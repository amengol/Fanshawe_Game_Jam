#pragma once

#ifdef _WIN32
#include <windows.h> 
#endif
#include <math.h>
#include <vector>
#include <iostream>
#include "src/globalOpenGL_GLFW.h"
#include "src/globalGameStuff.h"


/* Some physics constants */
#define DAMPING 0.01f // how much to damp the cloth simulation each frame
#define TIME_STEPSIZE2 0.5f*0.5f // how large time step each particle takes each frame
#define CONSTRAINT_ITERATIONS 15 // how many iterations of constraint satisfaction each frame (more is rigid, less is soft)


/* The particle class represents a particle of mass that can move around in 3D space*/
class Particle
{
private:
    bool movable; // can the particle move or not ? used to pin parts of the cloth

    float mass; // the mass of the particle (is always 1 in this example)
    glm::vec3 pos; // the current position of the particle in 3D space
    glm::vec3 old_pos; // the position of the particle in the previous time step, used as part of the verlet numerical integration scheme
    glm::vec3 acceleration; // a vector representing the current acceleration of the particle
    glm::vec3 accumulated_normal; // an accumulated normal (i.e. non normalized), used for OpenGL soft shading

public:
    Particle(glm::vec3 pos) : pos(pos), old_pos(pos), acceleration(glm::vec3(0.0f, 0.0f, 0.0f)), mass(1.0f), movable(true), accumulated_normal(glm::vec3(0.0f, 0.0f, 0.0f)) { }
    Particle() { }

    void addForce(glm::vec3 f)
    {
        acceleration += f / mass;
    }

    /* This is one of the important methods, where the time is progressed a single step size (TIME_STEPSIZE)
    The method is called by Cloth.time_step()
    Given the equation "force = mass * acceleration" the next position is found through verlet integration*/
    void timeStep()
    {
        if (movable)
        {
            glm::vec3 temp = pos;
            pos = pos + (pos - old_pos) * (1.0f - DAMPING) + acceleration * TIME_STEPSIZE2;
            old_pos = temp;
            acceleration = glm::vec3(0.0f, 0.0f, 0.0f); // acceleration is reset since it HAS been translated into a change in position (and implicitely into velocity)	
        }
    }

    glm::vec3& getPos() { return pos; }

    void resetAcceleration() { acceleration = glm::vec3(0.0f, 0.0f, 0.0f); }

    void offsetPos(const glm::vec3 v) { if (movable) pos += v; }

    void makeUnmovable() { movable = false; }

    void addToNormal(glm::vec3 normal)
    {
        accumulated_normal += glm::normalize(normal);
    }

    glm::vec3& getNormal() { return accumulated_normal; } // notice, the normal is not unit length

    void resetNormal() { accumulated_normal = glm::vec3(0.0f, 0.0f, 0.0f); }

};

class Constraint
{
private:
    float rest_distance; // the length between particle p1 and p2 in rest configuration

public:
    Particle *p1, *p2; // the two particles that are connected through this constraint

    Constraint(Particle *p1, Particle *p2) : p1(p1), p2(p2)
    {
        glm::vec3 vec = p1->getPos() - p2->getPos();
        rest_distance = glm::length(vec);
    }

    /* This is one of the important methods, where a single constraint between two particles p1 and p2 is solved
    the method is called by Cloth.time_step() many times per frame*/
    void satisfyConstraint()
    {
        glm::vec3 p1_to_p2 = p2->getPos() - p1->getPos(); // vector from p1 to p2
        float current_distance = glm::length(p1_to_p2); // current distance between p1 and p2
        glm::vec3 correctionVector = p1_to_p2 * (1.0f - rest_distance / current_distance); // The offset vector that could moves p1 into a distance of rest_distance to p2
        glm::vec3 correctionVectorHalf = correctionVector * 0.5f; // Lets make it half that length, so that we can move BOTH p1 and p2.
        p1->offsetPos(correctionVectorHalf); // correctionVectorHalf is pointing from p1 to p2, so the length should move p1 half the length needed to satisfy the constraint.
        p2->offsetPos(-correctionVectorHalf); // we must move p2 the negative direction of correctionVectorHalf since it points from p2 to p1, and not p1 to p2.	
    }

};

class Cloth
{
private:

    int num_particles_width; // number of particles in "width" direction
    int num_particles_height; // number of particles in "height" direction
                              // total number of particles is num_particles_width*num_particles_height

    std::vector<Particle> particles; // all particles that are part of this cloth
    std::vector<Constraint> constraints; // alle constraints between particles as part of this cloth

    Particle* getParticle(int x, int y) { return &particles[y*num_particles_width + x]; }
    void makeConstraint(Particle *p1, Particle *p2) { constraints.push_back(Constraint(p1, p2)); }


    /* A private method used by drawShaded() and addWindForcesForTriangle() to retrieve the
    normal vector of the triangle defined by the position of the particles p1, p2, and p3.
    The magnitude of the normal vector is equal to the area of the parallelogram defined by p1, p2 and p3
    */
    glm::vec3 calcTriangleNormal(Particle *p1, Particle *p2, Particle *p3)
    {
        glm::vec3 pos1 = p1->getPos();
        glm::vec3 pos2 = p2->getPos();
        glm::vec3 pos3 = p3->getPos();

        glm::vec3 v1 = pos2 - pos1;
        glm::vec3 v2 = pos3 - pos1;

        return glm::cross(v1,v2);
    }

    /* A private method used by windForce() to calcualte the wind force for a single triangle
    defined by p1,p2,p3*/
    void addWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const glm::vec3 direction)
    {
        glm::vec3 normal = calcTriangleNormal(p1, p2, p3);
        glm::vec3 d = glm::normalize(normal);
        glm::vec3 force = normal * (glm::dot(d,direction));
        p1->addForce(force);
        p2->addForce(force);
        p3->addForce(force);
    }

    /* A private method used by drawShaded(), that draws a single triangle p1,p2,p3 with a color*/
    void drawTriangle(Particle *p1, Particle *p2, Particle *p3, const glm::vec3 color)
    {
        glColor3fv((GLfloat*)&color);

        glNormal3fv((GLfloat *) &(glm::normalize(p1->getNormal())));
        glVertex3fv((GLfloat *) &(p1->getPos()));

        glNormal3fv((GLfloat *) &(glm::normalize(p2->getNormal())));
        glVertex3fv((GLfloat *) &(p2->getPos()));

        glNormal3fv((GLfloat *) &(glm::normalize(p3->getNormal())));
        glVertex3fv((GLfloat *) &(p3->getPos()));
    }

public:

    /* This is a important constructor for the entire system of particles and constraints*/
    Cloth(glm::vec3 upLeftPostion, float width, float height, int num_particles_width, int num_particles_height) : num_particles_width(num_particles_width), num_particles_height(num_particles_height)
    {
        particles.resize(num_particles_width*num_particles_height); //I am essentially using this vector as an array with room for num_particles_width*num_particles_height particles

                                                                    // creating particles in a grid of particles from (0,0,0) to (width,-height,0)
        for (int x = 0; x<num_particles_width; x++)
        {
            for (int y = 0; y<num_particles_height; y++)
            {
                glm::vec3 pos = glm::vec3(width * (x / (float)num_particles_width),
                                          -height * (y / (float)num_particles_height),
                                          0) + upLeftPostion;
                particles[y*num_particles_width + x] = Particle(pos); // insert particle in column x at y'th row
            }
        }

        // Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
        for (int x = 0; x<num_particles_width; x++)
        {
            for (int y = 0; y<num_particles_height; y++)
            {
                if (x<num_particles_width - 1) makeConstraint(getParticle(x, y), getParticle(x + 1, y));
                if (y<num_particles_height - 1) makeConstraint(getParticle(x, y), getParticle(x, y + 1));
                if (x<num_particles_width - 1 && y<num_particles_height - 1) makeConstraint(getParticle(x, y), getParticle(x + 1, y + 1));
                if (x<num_particles_width - 1 && y<num_particles_height - 1) makeConstraint(getParticle(x + 1, y), getParticle(x, y + 1));
            }
        }


        // Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
        for (int x = 0; x<num_particles_width; x++)
        {
            for (int y = 0; y<num_particles_height; y++)
            {
                if (x<num_particles_width - 2) makeConstraint(getParticle(x, y), getParticle(x + 2, y));
                if (y<num_particles_height - 2) makeConstraint(getParticle(x, y), getParticle(x, y + 2));
                if (x<num_particles_width - 2 && y<num_particles_height - 2) makeConstraint(getParticle(x, y), getParticle(x + 2, y + 2));
                if (x<num_particles_width - 2 && y<num_particles_height - 2) makeConstraint(getParticle(x + 2, y), getParticle(x, y + 2));
            }
        }


        // making the upper left most three and right most three particles unmovable
        for (int i = 0; i<3; i++)
        {
            getParticle(0 + i, 0)->offsetPos(glm::vec3(0.5f, 0.0f, 0.0f)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
            getParticle(0 + i, 0)->makeUnmovable();

            getParticle(0 + i, 0)->offsetPos(glm::vec3(-0.5f, 0.0f, 0.0f)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
            getParticle(num_particles_width - 1 - i, 0)->makeUnmovable();
        }
    }

    /* drawing the cloth as a smooth shaded (and colored according to column) OpenGL triangular mesh
    Called from the display() method
    The cloth is seen as consisting of triangles for four particles in the grid as follows:

    (x,y)   *--* (x+1,y)
    | /|
    |/ |
    (x,y+1) *--* (x+1,y+1)

    */
    void drawShaded()
    {
        // reset normals (which where written to last frame)
        std::vector<Particle>::iterator particle;
        for (particle = particles.begin(); particle != particles.end(); particle++)
        {
            (*particle).resetNormal();
        }

        //create smooth per particle normals by adding up all the (hard) triangle normals that each particle is part of
        for (int x = 0; x<num_particles_width - 1; x++)
        {
            for (int y = 0; y<num_particles_height - 1; y++)
            {
                glm::vec3 normal = calcTriangleNormal(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1));
                getParticle(x + 1, y)->addToNormal(normal);
                getParticle(x, y)->addToNormal(normal);
                getParticle(x, y + 1)->addToNormal(normal);

                normal = calcTriangleNormal(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1));
                getParticle(x + 1, y + 1)->addToNormal(normal);
                getParticle(x + 1, y)->addToNormal(normal);
                getParticle(x, y + 1)->addToNormal(normal);
            }
        }

        //glBegin(GL_TRIANGLES);
        cMesh theMesh;
        theMesh.numberOfVertices = (num_particles_width - 1) * (num_particles_height - 1) * 6;
        sVertex* pVertices = new sVertex[theMesh.numberOfVertices];
        theMesh.numberOfTriangles = (num_particles_width - 1) * (num_particles_height - 1) * 2;
        cTriangle* pTriangles = new cTriangle[theMesh.numberOfTriangles];
        int vertexIndex = 0;
        int triangleIndex = 0;
        int numOfIterations = 0;

        for (int x = 0; x < num_particles_width - 1; x++)
        {
            for (int y = 0; y < num_particles_height - 1; y++)
            {
                glm::vec3 color(0, 0, 0);
                if (x % 2) // red and white color is interleaved according to which column number
                    color = glm::vec3(0.6f, 0.2f, 0.2f);
                else
                    color = glm::vec3(1.0f, 1.0f, 1.0f);

                numOfIterations++;
                // The first triangle
                //-------------------------------------------------------------
                // First vertice                
                glm::vec3 partPos = getParticle(x + 1, y)->getPos();
                pVertices[vertexIndex + 0].x = partPos.x;
                pVertices[vertexIndex + 0].y = partPos.y;
                pVertices[vertexIndex + 0].z = partPos.z;

                glm::vec3 partNormal = glm::normalize(getParticle(x + 1, y)->getNormal());
                pVertices[vertexIndex + 0].nx = partNormal.x;
                pVertices[vertexIndex + 0].ny = partNormal.y;
                pVertices[vertexIndex + 0].nz = partNormal.z;

                pVertices[vertexIndex + 0].r = color.x;
                pVertices[vertexIndex + 0].g = color.y;
                pVertices[vertexIndex + 0].b = color.z;
                pVertices[vertexIndex + 0].a = 1.0f;

                pTriangles[triangleIndex + 0].vertex_ID_0 = vertexIndex + 0;

                // Second vertice                
                partPos = getParticle(x, y)->getPos();
                pVertices[vertexIndex + 1].x = partPos.x;
                pVertices[vertexIndex + 1].y = partPos.y;
                pVertices[vertexIndex + 1].z = partPos.z;

                partNormal = glm::normalize(getParticle(x, y)->getNormal());
                pVertices[vertexIndex + 1].nx = partNormal.x;
                pVertices[vertexIndex + 1].ny = partNormal.y;
                pVertices[vertexIndex + 1].nz = partNormal.z;

                pVertices[vertexIndex + 1].r = color.x;
                pVertices[vertexIndex + 1].g = color.y;
                pVertices[vertexIndex + 1].b = color.z;
                pVertices[vertexIndex + 1].a = 1.0f;

                pTriangles[triangleIndex + 0].vertex_ID_1 = vertexIndex + 1;

                // Third vertice                
                partPos = getParticle(x, y + 1)->getPos();
                pVertices[vertexIndex + 2].x = partPos.x;
                pVertices[vertexIndex + 2].y = partPos.y;
                pVertices[vertexIndex + 2].z = partPos.z;

                partNormal = glm::normalize(getParticle(x, y + 1)->getNormal());
                pVertices[vertexIndex + 2].nx = partNormal.x;
                pVertices[vertexIndex + 2].ny = partNormal.y;
                pVertices[vertexIndex + 2].nz = partNormal.z;

                pVertices[vertexIndex + 2].r = color.x;
                pVertices[vertexIndex + 2].g = color.y;
                pVertices[vertexIndex + 2].b = color.z;
                pVertices[vertexIndex + 2].a = 1.0f;

                pTriangles[triangleIndex + 0].vertex_ID_2 = vertexIndex + 2;

                // The Second triangle
                //-------------------------------------------------------------
                // First vertice                
                partPos = getParticle(x + 1, y + 1)->getPos();
                pVertices[vertexIndex + 3].x = partPos.x;
                pVertices[vertexIndex + 3].y = partPos.y;
                pVertices[vertexIndex + 3].z = partPos.z;

                partNormal = glm::normalize(getParticle(x + 1, y + 1)->getNormal());
                pVertices[vertexIndex + 3].nx = partNormal.x;
                pVertices[vertexIndex + 3].ny = partNormal.y;
                pVertices[vertexIndex + 3].nz = partNormal.z;

                pVertices[vertexIndex + 3].r = color.x;
                pVertices[vertexIndex + 3].g = color.y;
                pVertices[vertexIndex + 3].b = color.z;
                pVertices[vertexIndex + 3].a = 1.0f;

                pTriangles[triangleIndex + 1].vertex_ID_0 = vertexIndex + 3;

                // Second vertice                
                partPos = getParticle(x + 1, y)->getPos();
                pVertices[vertexIndex + 4].x = partPos.x;
                pVertices[vertexIndex + 4].y = partPos.y;
                pVertices[vertexIndex + 4].z = partPos.z;

                partNormal = glm::normalize(getParticle(x + 1, y)->getNormal());
                pVertices[vertexIndex + 4].nx = partNormal.x;
                pVertices[vertexIndex + 4].ny = partNormal.y;
                pVertices[vertexIndex + 4].nz = partNormal.z;

                pVertices[vertexIndex + 4].r = color.x;
                pVertices[vertexIndex + 4].g = color.y;
                pVertices[vertexIndex + 4].b = color.z;
                pVertices[vertexIndex + 4].a = 1.0f;

                pTriangles[triangleIndex + 1].vertex_ID_1 = vertexIndex + 4;

                // Third vertice                
                partPos = getParticle(x, y + 1)->getPos();
                pVertices[vertexIndex + 5].x = partPos.x;
                pVertices[vertexIndex + 5].y = partPos.y;
                pVertices[vertexIndex + 5].z = partPos.z;

                partNormal = glm::normalize(getParticle(x, y + 1)->getNormal());
                pVertices[vertexIndex + 5].nx = partNormal.x;
                pVertices[vertexIndex + 5].ny = partNormal.y;
                pVertices[vertexIndex + 5].nz = partNormal.z;

                pVertices[vertexIndex + 5].r = color.x;
                pVertices[vertexIndex + 5].g = color.y;
                pVertices[vertexIndex + 5].b = color.z;
                pVertices[vertexIndex + 5].a = 1.0f;

                pTriangles[triangleIndex + 1].vertex_ID_2 = vertexIndex + 5;

                vertexIndex += 6;
                triangleIndex += 2;

                //glColor3fv((GLfloat*)&color);

                //glNormal3fv((GLfloat *) &(p1->getNormal().normalized()));
                //glVertex3fv((GLfloat *) &(p1->getPos()));

                //glNormal3fv((GLfloat *) &(p2->getNormal().normalized()));
                //glVertex3fv((GLfloat *) &(p2->getPos()));

                //glNormal3fv((GLfloat *) &(p3->getNormal().normalized()));
                //glVertex3fv((GLfloat *) &(p3->getPos()));

                //drawTriangle(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1),color);
                //drawTriangle(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1),color);
            }
        }
        theMesh.pVertices = pVertices;
        theMesh.pTriangles = pTriangles;
        ::g_pVAOManager->loadMeshIntoDynamicVAO(theMesh, "Cloth");
        delete[] pVertices;
        delete[] pTriangles;
        //glEnd();
    }

    /* this is an important methods where the time is progressed one time step for the entire cloth.
    This includes calling satisfyConstraint() for every constraint, and calling timeStep() for all particles
    */
    void timeStep()
    {
        std::vector<Constraint>::iterator constraint;
        for (int i = 0; i<CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
        {
            for (constraint = constraints.begin(); constraint != constraints.end(); constraint++)
            {
                (*constraint).satisfyConstraint(); // satisfy constraint.
            }
        }

        std::vector<Particle>::iterator particle;
        for (particle = particles.begin(); particle != particles.end(); particle++)
        {
            (*particle).timeStep(); // calculate the position of each particle at the next time step.
        }
    }

    /* used to add gravity (or any other arbitrary vector) to all particles*/
    void addForce(const glm::vec3 direction)
    {
        std::vector<Particle>::iterator particle;
        for (particle = particles.begin(); particle != particles.end(); particle++)
        {
            (*particle).addForce(direction); // add the forces to each particle
        }

    }

    /* used to add wind forces to all particles, is added for each triangle since the final force is proportional to the triangle area as seen from the wind direction*/
    void windForce(const glm::vec3 direction)
    {
        for (int x = 0; x<num_particles_width - 1; x++)
        {
            for (int y = 0; y<num_particles_height - 1; y++)
            {
                addWindForcesForTriangle(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1), direction);
                addWindForcesForTriangle(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1), direction);
            }
        }
    }

    /* used to detect and resolve the collision of the cloth with the ball.
    This is based on a very simples scheme where the position of each particle is simply compared to the sphere and corrected.
    This also means that the sphere can "slip through" if the ball is small enough compared to the distance in the grid bewteen particles
    */
    void ballCollision(const glm::vec3 center, const float radius)
    {
        std::vector<Particle>::iterator particle;
        for (particle = particles.begin(); particle != particles.end(); particle++)
        {
            glm::vec3 v = (*particle).getPos() - center;
            float vecLength = glm::length(v);
            if (vecLength < radius) // if the particle is inside the ball
            {
                (*particle).offsetPos(glm::normalize(v)*(radius - vecLength)); // project the particle to the surface of the ball
            }
        }
    }

};

/***** Above are definition of classes; glm::vec3, Particle, Constraint, and Cloth *****/