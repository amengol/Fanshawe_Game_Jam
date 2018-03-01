// Debug GEOMETRY shader
#version 400 core

layout(triangles) 			in;
layout(triangle_strip) 		out;
layout(max_vertices = 12) 	out;

// gl_in[] array is built in (don't have to declare)
// gl_in[0] is the 1st vertex
// gl_in[1] is the 2nd vertex
// gl_in[2] is the 3rd vertex

in vec4 gVertColour[];		// in from the vertex shader
out vec4 fVertColour;		// out to the fragment shader

uniform mat4 mModel;    	                         
uniform mat4 mView;                             
uniform mat4 mProjection; 


void main()
{

	// gl_Position is the ORIGINAL vertex value (from VBA)
	vec3 triCentre = ( gl_in[0].gl_Position.xyz + 
                       gl_in[1].gl_Position.xyz + 
					   gl_in[2].gl_Position.xyz ) / 3.0f;
	
	
	vec4 red = 	 vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vec4 green = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	vec4 blue =  vec4(0.0f, 0.0f, 1.0f, 1.0f);
	
	// Perform the final screen space transform here... 
	mat4 MVP = mProjection * mView * mModel;    
	
	vec4 v0 = MVP * gl_in[0].gl_Position;
	vec4 v1 = MVP * gl_in[1].gl_Position;
	vec4 v2 = MVP * gl_in[2].gl_Position;
	
	vec4 vCentre = MVP * vec4(triCentre, 1.0f);
	
	
	// 0, 1, centre
    gl_Position = v0;		fVertColour = red;
    EmitVertex();
    gl_Position = v1;		fVertColour = red;
    EmitVertex();
    gl_Position = vCentre;	fVertColour = red;
    EmitVertex();
    EndPrimitive();

	// centre, 1, 2
   gl_Position = vCentre;	fVertColour = green;
   EmitVertex();
   gl_Position = v1;		fVertColour = green;
   EmitVertex();
   gl_Position = v2;		fVertColour = green;
   EmitVertex();
   EndPrimitive();

	// 0, centre, 2
    gl_Position = v0;		fVertColour = blue;
    EmitVertex();
    gl_Position = vCentre;	fVertColour = blue;
    EmitVertex();
    gl_Position = v2;		fVertColour = blue;
    EmitVertex();
    EndPrimitive();
	
	
	// Cross product of two triangle edges gives normal 
	//	at that vertex (so cross( V0->V1, V0-V2 )
	
	vec3 vec0to1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 vec0to2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	
	// The normal at vertex 0 (i.e. at the origin)
	vec3 vecTriNormal = normalize(cross( vec0to1, vec0to2 ));
	
	// Scale to 50 times length
	vec3 vecTipOfNormal = (triCentre + vecTriNormal * 50.0f);

	vec4 vNormDraw = MVP * vec4(vecTipOfNormal.xyz, 1.0f);
	
	vec4 white = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// Triangle with two "base" vertices at centre
	// End of triangle is the "tip" of the normal
    gl_Position = vNormDraw;			fVertColour = white;
    EmitVertex();
    gl_Position = vCentre;				fVertColour = white;
    EmitVertex();
    gl_Position = vCentre;				fVertColour = white;
    EmitVertex();
    EndPrimitive();

	
}





