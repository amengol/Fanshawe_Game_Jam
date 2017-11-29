// Vertex shader version
#version 420

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;

// Vertex attributes
attribute vec3 vCol;
attribute vec3 vPos;
attribute vec3 vNorm;

out vec3 color;
out vec3 vertNormal;
out vec3 vecWorldPosition;	

void main()
{	
	vec3 position = vPos;
	
	// Calculate the model view projection matrix
	mat4 MVP = mProjection * mView * mModel;	
	gl_Position = MVP * vec4(position, 1.0);
	
	// Calculate vertex and normal based on ONLY world 
	vecWorldPosition = vec3( mModel * vec4(position, 1.0f) ).xyz;

	// Inv Tran - strips translation and scale from model transform
	// Alternative is you pass a "rotation only" model mat4
	mat4 mWorldInTranspose = inverse(transpose(mModel));
	
	// This normal is in "world space" but only has rotation
	vertNormal = vec3( mWorldInTranspose * vec4(vNorm, 1.0f) ).xyz;	
	
    color = vCol;
}
