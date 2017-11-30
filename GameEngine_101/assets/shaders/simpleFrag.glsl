// Fragment shader version
#version 420

in vec3 color;
in vec3 vertNormal;
in vec3 vecWorldPosition;	

// The values our OBJECT material
uniform vec4 materialDiffuse;	
uniform vec4 materialAmbient;   
uniform float ambientToDiffuseRatio;
uniform vec4 materialSpecular; 

// Camera position
uniform vec3 eyePosition;
uniform bool bIsDebugWireFrameObject;	

uniform bool bUseTheVertexColourAsDiffuse;

/*****************************************************/
struct sLightDesc {
	vec4 position;
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;		// Colour (xyz), intensity (w)
	vec4 attenuation;	// x = constant, y = linear, z = quadratic
	vec4 direction;
	vec4 typeParams;	// x = type, y = distance cut-off
	                    // z angle1, w = angle2
};

const int NUMBEROFLIGHTS = 10;
uniform sLightDesc myLight[NUMBEROFLIGHTS];

// Calculate the contribution of a light at a vertex
vec3 calcLightColour( in vec3 vecNormal, 
                      in vec3 vecWorldPosition, 
                      in int lightID );
					  
/*****************************************************/

void main()
{
	// Is this a 'debug' wireframe object
	// i.e. no lighting, just use diffuse
	if ( bIsDebugWireFrameObject )
	{
		// This is used for the debug lines, etc.
 		if ( bUseTheVertexColourAsDiffuse )
 		{
 			gl_FragColor.rgb = color;
 			gl_FragColor.a = 1.0f;
 		}
 		else
 		{
 			gl_FragColor.rgb = materialDiffuse.rgb;
 			gl_FragColor.a = 1.0f;
 		}
		// Immediate return
		return;
	}
	
	// Set to black...
	gl_FragColor.rgb = vec3(0.0f, 0.0f, 0.0f);
	for ( int index = 0; index < 9; index++ )
	{
		gl_FragColor.rgb += calcLightColour( vertNormal, vecWorldPosition, index );
	}

    gl_FragColor.a = 1.0f;	// set 4th value to 1 if unsure
	
	// Screen is so dim...
	gl_FragColor *= 1.5f;	// 150% brighter
}

// Inspired by Mike Bailey's Graphics Shader book
// (when you see "half-vector", that's eye space)
// Technically, a "per-pixel" or a form of Phong shading
// Calcualte the contribution of a light at a vertex
vec3 calcLightColour( in vec3 vecNormal, 
                      in vec3 vecWorldPosition, 
                      in int lightID )
{
	vec3 colour = vec3( 0.0f, 0.0f, 0.0f );	
	vec3 outDiffuse = vec3( 0.0f, 0.0f, 0.0f );
	vec3 outSpecular = vec3( 0.0f, 0.0f, 0.0f );	

	// Distance between light and vertex (in world)
	vec3 vecToLight = myLight[lightID].position.xyz - vecWorldPosition;	
	// How long is this vector?
	float lightDistance = length(vecToLight);
	
	// Short circuit distance
	if ( myLight[lightID].typeParams.y <  lightDistance )
	{
		return colour;
	}
	
	// The "normalized" vector to the light (don't care about distance, just direction)
	vec3 lightVector = normalize( vecToLight );
	
	// The normalized version of the vertex normal
	vecNormal = normalize(vecNormal);
	
	// How much diffuse light is being "reflected" off the surface 
	float diffFactor = max(0.0f, dot( lightVector, vecNormal ));
	
	outDiffuse.rgb = myLight[lightID].diffuse.rgb 		// Light contribution
	                 * materialDiffuse.rgb				// Material contribution
					 * diffFactor;						// Factor based on direction

	float attenuation = 1.0f / 
	   ( myLight[lightID].attenuation.x  
	   + myLight[lightID].attenuation.y * lightDistance
	   + myLight[lightID].attenuation.z * lightDistance * lightDistance );
						
	outDiffuse.rgb *= attenuation;
	
	// Vector from vertex to eye 
	// i.e. this makes the vector base effectively at zero.
	vec3 viewVector = normalize( eyePosition - vecWorldPosition );
	vec3 vecLightReflection = reflect( normalize(lightVector), vecNormal );
	
	float specularShininess = materialSpecular.w;
	vec3 specMatColour = materialSpecular.rgb;
	
	outSpecular.rgb = pow( max(0.0f, dot(viewVector,vecLightReflection)), specularShininess)
			          * specMatColour
				      * myLight[lightID].specular.rgb;
				   
	outSpecular *= attenuation;
	
	colour = outDiffuse;
	
	return colour;
}// vec3 calcLightColour(...) 

