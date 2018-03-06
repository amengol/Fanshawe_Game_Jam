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


class Vec3 // a minimal vector class of 3 floats and overloaded math operators
{	
public:
	float f[3];

	Vec3(float x, float y, float z)
	{
		f[0] =x;
		f[1] =y;
		f[2] =z;
	}

	Vec3() {}

	float length()
	{
		return sqrt(f[0]*f[0]+f[1]*f[1]+f[2]*f[2]);
	}

	Vec3 normalized()
	{
		float l = length();
		return Vec3(f[0]/l,f[1]/l,f[2]/l);
	}

	void operator+= (const Vec3 &v)
	{
		f[0]+=v.f[0];
		f[1]+=v.f[1];
		f[2]+=v.f[2];
	}

	Vec3 operator/ (const float &a)
	{
		return Vec3(f[0]/a,f[1]/a,f[2]/a);
	}

	Vec3 operator- (const Vec3 &v)
	{
		return Vec3(f[0]-v.f[0],f[1]-v.f[1],f[2]-v.f[2]);
	}

	Vec3 operator+ (const Vec3 &v)
	{
		return Vec3(f[0]+v.f[0],f[1]+v.f[1],f[2]+v.f[2]);
	}

	Vec3 operator* (const float &a)
	{
		return Vec3(f[0]*a,f[1]*a,f[2]*a);
	}

	Vec3 operator-()
	{
		return Vec3(-f[0],-f[1],-f[2]);
	}

	Vec3 cross(const Vec3 &v)
	{
		return Vec3(f[1]*v.f[2] - f[2]*v.f[1], f[2]*v.f[0] - f[0]*v.f[2], f[0]*v.f[1] - f[1]*v.f[0]);
	}

	float dot(const Vec3 &v)
	{
		return f[0]*v.f[0] + f[1]*v.f[1] + f[2]*v.f[2];
	}
};

/* The particle class represents a particle of mass that can move around in 3D space*/
class Particle
{
private:
	bool movable; // can the particle move or not ? used to pin parts of the cloth

	float mass; // the mass of the particle (is always 1 in this example)
	Vec3 pos; // the current position of the particle in 3D space
	Vec3 old_pos; // the position of the particle in the previous time step, used as part of the verlet numerical integration scheme
	Vec3 acceleration; // a vector representing the current acceleration of the particle
	Vec3 accumulated_normal; // an accumulated normal (i.e. non normalized), used for OpenGL soft shading

public:
	Particle(Vec3 pos) : pos(pos), old_pos(pos),acceleration(Vec3(0,0,0)), mass(1), movable(true), accumulated_normal(Vec3(0,0,0)){}
	Particle(){}

	void addForce(Vec3 f)
	{
		acceleration += f/mass;
	}

	/* This is one of the important methods, where the time is progressed a single step size (TIME_STEPSIZE)
	   The method is called by Cloth.time_step()
	   Given the equation "force = mass * acceleration" the next position is found through verlet integration*/
	void timeStep()
	{
		if(movable)
		{
			Vec3 temp = pos;
			pos = pos + (pos-old_pos)*(1.0-DAMPING) + acceleration*TIME_STEPSIZE2;
			old_pos = temp;
			acceleration = Vec3(0,0,0); // acceleration is reset since it HAS been translated into a change in position (and implicitely into velocity)	
		}
	}

	Vec3& getPos() {return pos;}

	void resetAcceleration() {acceleration = Vec3(0,0,0);}

	void offsetPos(const Vec3 v) { if(movable) pos += v;}

	void makeUnmovable() {movable = false;}

	void addToNormal(Vec3 normal)
	{
		accumulated_normal += normal.normalized();
	}

	Vec3& getNormal() { return accumulated_normal;} // notice, the normal is not unit length

	void resetNormal() {accumulated_normal = Vec3(0,0,0);}

};

class Constraint
{
private:
	float rest_distance; // the length between particle p1 and p2 in rest configuration

public:
	Particle *p1, *p2; // the two particles that are connected through this constraint

	Constraint(Particle *p1, Particle *p2) :  p1(p1),p2(p2)
	{
		Vec3 vec = p1->getPos()-p2->getPos();
		rest_distance = vec.length();
	}

	/* This is one of the important methods, where a single constraint between two particles p1 and p2 is solved
	the method is called by Cloth.time_step() many times per frame*/
	void satisfyConstraint()
	{
		Vec3 p1_to_p2 = p2->getPos()-p1->getPos(); // vector from p1 to p2
		float current_distance = p1_to_p2.length(); // current distance between p1 and p2
		Vec3 correctionVector = p1_to_p2*(1 - rest_distance/current_distance); // The offset vector that could moves p1 into a distance of rest_distance to p2
		Vec3 correctionVectorHalf = correctionVector*0.5; // Lets make it half that length, so that we can move BOTH p1 and p2.
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

	Particle* getParticle(int x, int y) {return &particles[y*num_particles_width + x];}
	void makeConstraint(Particle *p1, Particle *p2) {constraints.push_back(Constraint(p1,p2));}


	/* A private method used by drawShaded() and addWindForcesForTriangle() to retrieve the  
	normal vector of the triangle defined by the position of the particles p1, p2, and p3.
	The magnitude of the normal vector is equal to the area of the parallelogram defined by p1, p2 and p3
	*/
	Vec3 calcTriangleNormal(Particle *p1,Particle *p2,Particle *p3)
	{
		Vec3 pos1 = p1->getPos();
		Vec3 pos2 = p2->getPos();
		Vec3 pos3 = p3->getPos();

		Vec3 v1 = pos2-pos1;
		Vec3 v2 = pos3-pos1;

		return v1.cross(v2);
	}

	/* A private method used by windForce() to calcualte the wind force for a single triangle 
	defined by p1,p2,p3*/
	void addWindForcesForTriangle(Particle *p1,Particle *p2,Particle *p3, const Vec3 direction)
	{
		Vec3 normal = calcTriangleNormal(p1,p2,p3);
		Vec3 d = normal.normalized();
		Vec3 force = normal*(d.dot(direction));
		p1->addForce(force);
		p2->addForce(force);
		p3->addForce(force);
	}

	/* A private method used by drawShaded(), that draws a single triangle p1,p2,p3 with a color*/
	void drawTriangle(Particle *p1, Particle *p2, Particle *p3, const Vec3 color)
	{
		glColor3fv( (GLfloat*) &color );

		glNormal3fv((GLfloat *) &(p1->getNormal().normalized() ));
		glVertex3fv((GLfloat *) &(p1->getPos() ));

		glNormal3fv((GLfloat *) &(p2->getNormal().normalized() ));
		glVertex3fv((GLfloat *) &(p2->getPos() ));

		glNormal3fv((GLfloat *) &(p3->getNormal().normalized() ));
		glVertex3fv((GLfloat *) &(p3->getPos() ));
	}

public:

	/* This is a important constructor for the entire system of particles and constraints*/
	Cloth(float width, float height, int num_particles_width, int num_particles_height) : num_particles_width(num_particles_width), num_particles_height(num_particles_height)
	{
		particles.resize(num_particles_width*num_particles_height); //I am essentially using this vector as an array with room for num_particles_width*num_particles_height particles

		// creating particles in a grid of particles from (0,0,0) to (width,-height,0)
		for(int x=0; x<num_particles_width; x++)
		{
			for(int y=0; y<num_particles_height; y++)
			{
				Vec3 pos = Vec3(width * (x/(float)num_particles_width),
								-height * (y/(float)num_particles_height),
								0);
				particles[y*num_particles_width+x]= Particle(pos); // insert particle in column x at y'th row
			}
		}

		// Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
		for(int x=0; x<num_particles_width; x++)
		{
			for(int y=0; y<num_particles_height; y++)
			{
				if (x<num_particles_width-1) makeConstraint(getParticle(x,y),getParticle(x+1,y));
				if (y<num_particles_height-1) makeConstraint(getParticle(x,y),getParticle(x,y+1));
				if (x<num_particles_width-1 && y<num_particles_height-1) makeConstraint(getParticle(x,y),getParticle(x+1,y+1));
				if (x<num_particles_width-1 && y<num_particles_height-1) makeConstraint(getParticle(x+1,y),getParticle(x,y+1));
			}
		}


		// Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
		for(int x=0; x<num_particles_width; x++)
		{
			for(int y=0; y<num_particles_height; y++)
			{
				if (x<num_particles_width-2) makeConstraint(getParticle(x,y),getParticle(x+2,y));
				if (y<num_particles_height-2) makeConstraint(getParticle(x,y),getParticle(x,y+2));
				if (x<num_particles_width-2 && y<num_particles_height-2) makeConstraint(getParticle(x,y),getParticle(x+2,y+2));
				if (x<num_particles_width-2 && y<num_particles_height-2) makeConstraint(getParticle(x+2,y),getParticle(x,y+2));			}
		}


		// making the upper left most three and right most three particles unmovable
		for(int i=0;i<3; i++)
		{
			getParticle(0+i ,0)->offsetPos(Vec3(0.5,0.0,0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
			getParticle(0+i ,0)->makeUnmovable(); 

			getParticle(0+i ,0)->offsetPos(Vec3(-0.5,0.0,0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
			getParticle(num_particles_width-1-i ,0)->makeUnmovable();
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
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).resetNormal();
		}

		//create smooth per particle normals by adding up all the (hard) triangle normals that each particle is part of
		for(int x = 0; x<num_particles_width-1; x++)
		{
			for(int y=0; y<num_particles_height-1; y++)
			{
				Vec3 normal = calcTriangleNormal(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1));
				getParticle(x+1,y)->addToNormal(normal);
				getParticle(x,y)->addToNormal(normal);
				getParticle(x,y+1)->addToNormal(normal);

				normal = calcTriangleNormal(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1));
				getParticle(x+1,y+1)->addToNormal(normal);
				getParticle(x+1,y)->addToNormal(normal);
				getParticle(x,y+1)->addToNormal(normal);
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

		for(int x = 0; x < num_particles_width - 1; x++)
		{
			for(int y = 0; y < num_particles_height - 1; y++)
			{
				Vec3 color(0,0,0);
				if (x%2) // red and white color is interleaved according to which column number
					color = Vec3(0.6f,0.2f,0.2f);
				else
					color = Vec3(1.0f,1.0f,1.0f);

                numOfIterations++;
				// The first triangle
                //-------------------------------------------------------------
                // First vertice                
                Vec3 partPos = getParticle(x + 1, y)->getPos();
                pVertices[vertexIndex + 0].x = partPos.f[0];
                pVertices[vertexIndex + 0].y = partPos.f[1];
                pVertices[vertexIndex + 0].z = partPos.f[2];
                
                Vec3 partNormal = getParticle(x + 1, y)->getNormal().normalized();
                pVertices[vertexIndex + 0].nx = partNormal.f[0];
                pVertices[vertexIndex + 0].ny = partNormal.f[1];
                pVertices[vertexIndex + 0].nz = partNormal.f[2];

                pVertices[vertexIndex + 0].r = color.f[0];
                pVertices[vertexIndex + 0].g = color.f[1];
                pVertices[vertexIndex + 0].b = color.f[2];
                pVertices[vertexIndex + 0].a = 1.0f;

                pTriangles[triangleIndex + 0].vertex_ID_0 = vertexIndex + 0;

                // Second vertice                
                partPos = getParticle(x, y)->getPos();
                pVertices[vertexIndex + 1].x = partPos.f[0];
                pVertices[vertexIndex + 1].y = partPos.f[1];
                pVertices[vertexIndex + 1].z = partPos.f[2];

                partNormal = getParticle(x, y)->getNormal().normalized();
                pVertices[vertexIndex + 1].nx = partNormal.f[0];
                pVertices[vertexIndex + 1].ny = partNormal.f[1];
                pVertices[vertexIndex + 1].nz = partNormal.f[2];

                pVertices[vertexIndex + 1].r = color.f[0];
                pVertices[vertexIndex + 1].g = color.f[1];
                pVertices[vertexIndex + 1].b = color.f[2];
                pVertices[vertexIndex + 1].a = 1.0f;

                pTriangles[triangleIndex + 0].vertex_ID_1 = vertexIndex + 1;

                // Third vertice                
                partPos = getParticle(x, y + 1)->getPos();
                pVertices[vertexIndex + 2].x = partPos.f[0];
                pVertices[vertexIndex + 2].y = partPos.f[1];
                pVertices[vertexIndex + 2].z = partPos.f[2];

                partNormal = getParticle(x, y + 1)->getNormal().normalized();
                pVertices[vertexIndex + 2].nx = partNormal.f[0];
                pVertices[vertexIndex + 2].ny = partNormal.f[1];
                pVertices[vertexIndex + 2].nz = partNormal.f[2];

                pVertices[vertexIndex + 2].r = color.f[0];
                pVertices[vertexIndex + 2].g = color.f[1];
                pVertices[vertexIndex + 2].b = color.f[2];
                pVertices[vertexIndex + 2].a = 1.0f;

                pTriangles[triangleIndex + 0].vertex_ID_2 = vertexIndex + 2;

                // The Second triangle
                //-------------------------------------------------------------
                // First vertice                
                partPos = getParticle(x + 1, y + 1)->getPos();
                pVertices[vertexIndex + 3].x = partPos.f[0];
                pVertices[vertexIndex + 3].y = partPos.f[1];
                pVertices[vertexIndex + 3].z = partPos.f[2];

                partNormal = getParticle(x + 1, y + 1)->getNormal().normalized();
                pVertices[vertexIndex + 3].nx = partNormal.f[0];
                pVertices[vertexIndex + 3].ny = partNormal.f[1];
                pVertices[vertexIndex + 3].nz = partNormal.f[2];

                pVertices[vertexIndex + 3].r = color.f[0];
                pVertices[vertexIndex + 3].g = color.f[1];
                pVertices[vertexIndex + 3].b = color.f[2];
                pVertices[vertexIndex + 3].a = 1.0f;

                pTriangles[triangleIndex + 1].vertex_ID_0 = vertexIndex + 3;

                // Second vertice                
                partPos = getParticle(x + 1, y)->getPos();
                pVertices[vertexIndex + 4].x = partPos.f[0];
                pVertices[vertexIndex + 4].y = partPos.f[1];
                pVertices[vertexIndex + 4].z = partPos.f[2];

                partNormal = getParticle(x + 1, y)->getNormal().normalized();
                pVertices[vertexIndex + 4].nx = partNormal.f[0];
                pVertices[vertexIndex + 4].ny = partNormal.f[1];
                pVertices[vertexIndex + 4].nz = partNormal.f[2];

                pVertices[vertexIndex + 4].r = color.f[0];
                pVertices[vertexIndex + 4].g = color.f[1];
                pVertices[vertexIndex + 4].b = color.f[2];
                pVertices[vertexIndex + 4].a = 1.0f;

                pTriangles[triangleIndex + 1].vertex_ID_1 = vertexIndex + 4;

                // Third vertice                
                partPos = getParticle(x, y + 1)->getPos();
                pVertices[vertexIndex + 5].x = partPos.f[0];
                pVertices[vertexIndex + 5].y = partPos.f[1];
                pVertices[vertexIndex + 5].z = partPos.f[2];

                partNormal = getParticle(x, y + 1)->getNormal().normalized();
                pVertices[vertexIndex + 5].nx = partNormal.f[0];
                pVertices[vertexIndex + 5].ny = partNormal.f[1];
                pVertices[vertexIndex + 5].nz = partNormal.f[2];

                pVertices[vertexIndex + 5].r = color.f[0];
                pVertices[vertexIndex + 5].g = color.f[1];
                pVertices[vertexIndex + 5].b = color.f[2];
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
		for(int i=0; i<CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
		{
			for(constraint = constraints.begin(); constraint != constraints.end(); constraint++ )
			{
				(*constraint).satisfyConstraint(); // satisfy constraint.
			}
		}

		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).timeStep(); // calculate the position of each particle at the next time step.
		}
	}

	/* used to add gravity (or any other arbitrary vector) to all particles*/
	void addForce(const Vec3 direction)
	{
		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).addForce(direction); // add the forces to each particle
		}

	}

	/* used to add wind forces to all particles, is added for each triangle since the final force is proportional to the triangle area as seen from the wind direction*/
	void windForce(const Vec3 direction)
	{
		for(int x = 0; x<num_particles_width-1; x++)
		{
			for(int y=0; y<num_particles_height-1; y++)
			{
				addWindForcesForTriangle(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1),direction);
				addWindForcesForTriangle(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1),direction);
			}
		}
	}

	/* used to detect and resolve the collision of the cloth with the ball.
	This is based on a very simples scheme where the position of each particle is simply compared to the sphere and corrected.
	This also means that the sphere can "slip through" if the ball is small enough compared to the distance in the grid bewteen particles
	*/
	void ballCollision(const Vec3 center,const float radius )
	{
		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			Vec3 v = (*particle).getPos()-center;
			float l = v.length();
			if ( v.length() < radius) // if the particle is inside the ball
			{
				(*particle).offsetPos(v.normalized()*(radius-l)); // project the particle to the surface of the ball
			}
		}
	}

	void doFrame()
	{

	}
};

/***** Above are definition of classes; Vec3, Particle, Constraint, and Cloth *****/