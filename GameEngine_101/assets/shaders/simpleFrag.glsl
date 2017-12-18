// Fragment shader
#version 420

in vec4 color;					// was vec3
in vec3 vertNormal;
in vec3 vecWorldPosition;		
in vec4 uvX2out;			// Added: UV 1 and 2 to fragment
							// UV #1 is .xy 
							// UV #2 is .zw

//uniform vec4 diffuseColour;		// New variable

// The values our OBJECT material
uniform vec4 materialDiffuse;	
uniform vec4 materialAmbient;   		// 0.2
uniform float ambientToDiffuseRatio; 	// Maybe	// 0.2 or 0.3
uniform vec4 materialSpecular;  // rgb = colour of HIGHLIGHT only
								// w = shininess of the 
uniform vec3 eyePosition;	// Camera position

uniform bool bIsDebugWireFrameObject;

uniform bool hasColour;

// Note: this CAN'T be an array (sorry). See 3D texture array
uniform sampler2D myAmazingTexture00;		// Represents a 2D image
uniform sampler2D myAmazingTexture01;		// Represents a 2D image
// ... and so on...

uniform float textureBlend00;		// Or an array
uniform float textureBlend01;		
// .... and so on... 

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

// This function has now been moved to the bottom of the file...
// Calculate the contribution of a light at a vertex
vec3 calcLightColour( in vec3 vecNormal, 
                      in vec3 vecWorldPosition, 
                      in int lightID );
/*****************************************************/



void main()
{	
	// Is this a 'debug' wireframe object, i.e. no lighting, just use diffuse
	if ( bIsDebugWireFrameObject )
	{
		if (hasColour)
		{
			gl_FragColor.rgb = color.rgb;
			gl_FragColor.a = color.a;		//gl_FragColor.a = 1.0f	
			return;		// Immediate return
		}
		else
		{
			gl_FragColor.rgb = materialDiffuse.rgb;
			gl_FragColor.a = materialDiffuse.a;		//gl_FragColor.a = 1.0f	
			return;		// Immediate return
		}
	}
	
	// Set to black...
	gl_FragColor.rgb = vec3(0.0f, 0.0f, 0.0f);
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{
		gl_FragColor.rgb += calcLightColour( vertNormal, vecWorldPosition, index );
	}

	//
	if (hasColour)
	{
		gl_FragColor.a = color.a;	// set 4th value to 1 if unsure
	}
	else
	{
		
		gl_FragColor.a = materialDiffuse.a;	// set 4th value to 1 if unsure
	}
	
	// Screen is so dim...
	gl_FragColor *= 1.5f;	// 150% brighter
	

	// DEBUG: For now, set RG colour to the UV coordinates
//	gl_FragColor.rg *= 0.00001f;	// Make (essentially) 0.0f	
//	gl_FragColor.rg += uvX2out.xy; 	
	
//uniform sampler2D myAmazingTexture00;
	//gl_FragColor.rgb *= 0.00001f; 	// Make black

	vec2 theUVCoords = uvX2out.xy;		// use UV #1 of vertex
	vec4 texCol00 = texture( myAmazingTexture00, theUVCoords.xy );
	vec4 texCol01 = texture( myAmazingTexture01, theUVCoords.xy );
	//... and so on (to how many textures you are using)
	
	// use the blend value to combine textures
	//gl_FragColor.rgb += (texCol00.rgb * textureBlend00) + 
	//                    (texCol01.rgb * textureBlend01);	// .. and so on
	                    //(texCol02.rgb * textureBlend00);	// .. and so on

	//gl_FragColor.rgb += texColour.rgb;
	
	// Copy object material diffuse to alpha
	gl_FragColor.a = materialDiffuse.a;

	return;
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
	
	if (hasColour)
	{
		outDiffuse.rgb = myLight[lightID].diffuse.rgb 		// Light contribution
			             * color.rgb				// Material contribution
						 * diffFactor;						// Factor based on direction
	}
	else	
	{
		outDiffuse.rgb = myLight[lightID].diffuse.rgb 		// Light contribution
			             * materialDiffuse.rgb				// Material contribution
						 * diffFactor;
	}

// Simple linear attenuation
//	float attenuation = 1.0f / lightDistance; 
	
	// From the "Shader" book article
	// 1.0 dividedBy ( constant 
	//                + linear * distance 
	//                + quadratic * distance * distance)
	// Our light description is: 
	// 	vec4 attenuation;	// x = constant, 
	//                         y = linear, 
	//                         z = quadratic
	float attenuation = 1.0f / 
	   ( myLight[lightID].attenuation.x // 0  
	   + myLight[lightID].attenuation.y * lightDistance			// Linear
	   + myLight[lightID].attenuation.z * lightDistance * lightDistance );	// Quad
	outDiffuse.rgb *= attenuation;

	
	// Vector from vertex to eye 
	// i.e. this makes the vector base effectively at zero.
	vec3 viewVector = normalize( eyePosition - vecWorldPosition );
	vec3 vecLightReflection = reflect( normalize(lightVector), vecNormal );
	
	float specularShininess = materialSpecular.w;	// 64.0f
	vec3 specMatColour = materialSpecular.rgb;		// vec3(1.0f, 1.0f, 1.0f);
	
	outSpecular.rgb = pow( max(0.0f, dot(viewVector,vecLightReflection)), 
	                  specularShininess)
			          * specMatColour
				      * myLight[lightID].specular.rgb;// // myLightSpecular;
				   
	outSpecular *= attenuation;
	
// For now, to simplify, eliminate the specular
//	colour = outDiffuse + outSpecular;
	colour = outDiffuse;
	
	return colour;
}// vec3 calcLightColour(...) 







//-----------------------------------------------------------------------------


// OLD VERSION BEFORE MERGING WITH FEENEY CODE
//// Fragment shader version
//#version 420

//in vec3 color;
//in vec3 vertNormal;
//in vec3 vecWorldPosition;	

//// The values our OBJECT material
//uniform vec4 materialDiffuse;	
//uniform vec4 materialAmbient;   
//uniform float ambientToDiffuseRatio;
//uniform vec4 materialSpecular; 

//// Camera position
//uniform vec3 eyePosition;
//uniform bool bIsDebugWireFrameObject;	

//uniform bool bUseTheVertexColourAsDiffuse;

///*****************************************************/
//struct sLightDesc {
//	vec4 position;
//	vec4 diffuse;
//	vec4 ambient;
//	vec4 specular;		// Colour (xyz), intensity (w)
//	vec4 attenuation;	// x = constant, y = linear, z = quadratic
//	vec4 direction;
//	vec4 typeParams;	// x = type, y = distance cut-off
//	                    // z angle1, w = angle2
//};

//const int NUMBEROFLIGHTS = 10;
//uniform sLightDesc myLight[NUMBEROFLIGHTS];

//// Calculate the contribution of a light at a vertex
//vec3 calcLightColour( in vec3 vecNormal, 
//                      in vec3 vecWorldPosition, 
//                      in int lightID );
					  
///*****************************************************/

//void main()
//{
//	// Is this a 'debug' wireframe object
//	// i.e. no lighting, just use diffuse
//	if ( bIsDebugWireFrameObject )
//	{
//		// This is used for the debug lines, etc.
// 		if ( bUseTheVertexColourAsDiffuse )
// 		{
// 			gl_FragColor.rgb = color;
// 			gl_FragColor.a = 1.0f;
// 		}
// 		else
// 		{
// 			gl_FragColor.rgb = materialDiffuse.rgb;
// 			gl_FragColor.a = 1.0f;
// 		}
//		// Immediate return
//		return;
//	}
	
//	// Set to black...
//	gl_FragColor.rgb = vec3(0.0f, 0.0f, 0.0f);
//	for ( int index = 0; index < 9; index++ )
//	{
//		gl_FragColor.rgb += calcLightColour( vertNormal, vecWorldPosition, index );
//	}

//    gl_FragColor.a = 1.0f;	// set 4th value to 1 if unsure
	
//	// Screen is so dim...
//	gl_FragColor *= 1.5f;	// 150% brighter
//}

//// Inspired by Mike Bailey's Graphics Shader book
//// (when you see "half-vector", that's eye space)
//// Technically, a "per-pixel" or a form of Phong shading
//// Calcualte the contribution of a light at a vertex
//vec3 calcLightColour( in vec3 vecNormal, 
//                      in vec3 vecWorldPosition, 
//                      in int lightID )
//{
//	vec3 colour = vec3( 0.0f, 0.0f, 0.0f );	
//	vec3 outDiffuse = vec3( 0.0f, 0.0f, 0.0f );
//	vec3 outSpecular = vec3( 0.0f, 0.0f, 0.0f );	

//	// Distance between light and vertex (in world)
//	vec3 vecToLight = myLight[lightID].position.xyz - vecWorldPosition;	
//	// How long is this vector?
//	float lightDistance = length(vecToLight);
	
//	// Short circuit distance
//	if ( myLight[lightID].typeParams.y <  lightDistance )
//	{
//		return colour;
//	}
	
//	// The "normalized" vector to the light (don't care about distance, just direction)
//	vec3 lightVector = normalize( vecToLight );
	
//	// The normalized version of the vertex normal
//	vecNormal = normalize(vecNormal);
	
//	// How much diffuse light is being "reflected" off the surface 
//	float diffFactor = max(0.0f, dot( lightVector, vecNormal ));
	
//	outDiffuse.rgb = myLight[lightID].diffuse.rgb 		// Light contribution
//	                 * materialDiffuse.rgb				// Material contribution
//					 * diffFactor;						// Factor based on direction

//	float attenuation = 1.0f / 
//	   ( myLight[lightID].attenuation.x  
//	   + myLight[lightID].attenuation.y * lightDistance
//	   + myLight[lightID].attenuation.z * lightDistance * lightDistance );
						
//	outDiffuse.rgb *= attenuation;
	
//	// Vector from vertex to eye 
//	// i.e. this makes the vector base effectively at zero.
//	vec3 viewVector = normalize( eyePosition - vecWorldPosition );
//	vec3 vecLightReflection = reflect( normalize(lightVector), vecNormal );
	
//	float specularShininess = materialSpecular.w;
//	vec3 specMatColour = materialSpecular.rgb;
	
//	outSpecular.rgb = pow( max(0.0f, dot(viewVector,vecLightReflection)), specularShininess)
//			          * specMatColour
//				      * myLight[lightID].specular.rgb;
				   
//	outSpecular *= attenuation;
	
//	colour = outDiffuse;
	
//	return colour;
//}// vec3 calcLightColour(...) 

