// Vertex shader
#version 420


uniform mat4 mView;
uniform mat4 mProjection;
uniform mat4 mModel;
uniform mat4 mWorldInvTranspose;

uniform bool isASkyBox;

uniform sampler2D texHeightMap;		// Is Texture Unit 0 no matter WHAT! WHY???
uniform bool bIsHeightMap;

in vec4 vCol;						// Color
in vec3 vPos;						// Position
in vec3 vNorm;						// Vertex normal
in vec4 uvX2;						// UV 1 and 2

out vec4 color;				
out vec3 vertNormal;		
out vec3 vecWorldPosition;	
out vec4 uvX2out;			

void main()
{
    
	vec3 vertPosition = vPos;	
	mat4 matModel = mModel;
	
	// Calculate the model view projection matrix here
	mat4 MVP = mProjection * mView * matModel;
	gl_Position = MVP * vec4(vertPosition, 1.0f);
	
	// Calculate vertex and normal based on ONLY world 
	vecWorldPosition = vec3( matModel * vec4(vertPosition, 1.0f) ).xyz;

	// This normal is in "world space" but only has rotation
	vertNormal = vec3( mWorldInvTranspose * vec4(vNorm, 1.0f) ).xyz;	
	
    color = vCol;
	uvX2out = uvX2;
}

