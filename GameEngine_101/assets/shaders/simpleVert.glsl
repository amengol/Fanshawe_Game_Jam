// Vertex shader
#version 420

//uniform mat4 MVP;
uniform mat4 mModel;
//uniform mat4 mModelRotationOnly;
uniform mat4 mView;
uniform mat4 mProjection;

// "Vertex" attribute (now use 'in')
in vec4 vCol;		// attribute
in vec3 vPos;		// was: vec2 vPos
in vec3 vNorm;		// Vertex normal
in vec4 uvX2;		// Added: UV 1 and 2

out vec4 color;				// was: vec4
out vec3 vertNormal;		// Also in "world" (no view or projection)
out vec3 vecWorldPosition;	// 
out vec4 uvX2out;			// Added: UV 1 and 2 to fragment

void main()
{
    //gl_Position = MVP * vec4(vPos, 0.0, 1.0);	
	vec3 position = vPos;
	
	// Calculate the model view projection matrix here
	mat4 MVP = mProjection * mView * mModel;
	gl_Position = MVP * vec4(position, 1.0f);
	
	// Calculate vertex and normal based on ONLY world 
	vecWorldPosition = vec3( mModel * vec4(position, 1.0f) ).xyz;
	
	// Inv Tran - strips translation and scale from model transform
	// Alternative is you pass a "rotation only" model mat4
	mat4 mWorldInTranspose = inverse(transpose(mModel));
	
	// Was: MVP * vNorm;
	// This normal is in "world space" but only has rotation
	vertNormal = vec3( mWorldInTranspose * vec4(vNorm, 1.0f) ).xyz;		
	
	
    color = vCol;
	uvX2out = uvX2;			// Sent to fragment shader
}


//	// Or could do this:
//	mWorldInTranpose = inverse(transpose(MVP));
//	
//	// Calculate vertex and normal based on ONLY world 
//	vecWorldPosition = mWorldInTranpose * vec4(position, 1.0f);
//	
//	// Not correct, but for now, just pass along
//	vertNormal = mWorldInTranpose * vNorm;		// Was: MVP * vNorm;

//-----------------------------------------------------------------------------


// OLD VERSION BEFORE MERGING WITH FEENEY CODE
//// Vertex shader version
//#version 420

//uniform mat4 mModel;
//uniform mat4 mView;
//uniform mat4 mProjection;

//// Vertex attributes
//attribute vec3 vCol;
//attribute vec3 vPos;
//attribute vec3 vNorm;

//out vec3 color;
//out vec3 vertNormal;
//out vec3 vecWorldPosition;	

//void main()
//{	
//	vec3 position = vPos;
	
//	// Calculate the model view projection matrix
//	mat4 MVP = mProjection * mView * mModel;	
//	gl_Position = MVP * vec4(position, 1.0);
	
//	// Calculate vertex and normal based on ONLY world 
//	vecWorldPosition = vec3( mModel * vec4(position, 1.0f) ).xyz;

//	// Inv Tran - strips translation and scale from model transform
//	// Alternative is you pass a "rotation only" model mat4
//	mat4 mWorldInTranspose = inverse(transpose(mModel));
	
//	// This normal is in "world space" but only has rotation
//	vertNormal = vec3( mWorldInTranspose * vec4(vNorm, 1.0f) ).xyz;	
	
//    color = vCol;
//}
