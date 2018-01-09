// Fragment shader
#version 400

in vec4 color;			
in vec3 vertNormal;
in vec3 vecWorldPosition;		
in vec4 uvX2out;		
						
						
// gl_FragColor is deprecated after version 120
// Now we specify a specific variable.
// If there is only 1, then GL will assume it's the colour 
out vec4 fragColourOut;

// The values our OBJECT material
uniform vec4 materialDiffuse;	
//uniform vec4 materialAmbient;   		// 0.2
uniform float ambientToDiffuseRatio;
uniform vec4 materialSpecular;  // rgb = colour of HIGHLIGHT only
								// w = shininess of the 
uniform vec3 eyePosition;	// Camera position

uniform bool bIsDebugWireFrameObject;
uniform bool hasColour;
uniform bool hasAlpha;

// Note: this CAN'T be an array (sorry). See 3D texture array
uniform sampler2D texSamp2D00;		// Represents a 2D image
uniform sampler2D texSamp2D01;		// Represents a 2D image
uniform sampler2D texSamp2D02;		// Represents a 2D image
uniform sampler2D texSamp2D03;		// Represents a 2D image
uniform sampler2D texSamp2D04;		// Represents a 2D image
uniform sampler2D texSamp2D05;		// Represents a 2D image
uniform sampler2D texSamp2D06;		// Represents a 2D image
uniform sampler2D texSamp2D07;		// Represents a 2D image
// ... and so on...

uniform float texBlend00;		
uniform float texBlend01;		
uniform float texBlend02;	
uniform float texBlend03;		
uniform float texBlend04;
uniform float texBlend05;		
uniform float texBlend06;
uniform float texBlend07;
// .... and so on... 

uniform bool isASkyBox;				// True samples the skybox texture
uniform samplerCube texSampCube00;
uniform samplerCube texSampCube01;
uniform samplerCube texSampCube02;
uniform samplerCube texSampCube03;
uniform float texCubeBlend00;
uniform float texCubeBlend01;
uniform float texCubeBlend02;
uniform float texCubeBlend03;

// For env. mapping (reflection and refraction)
uniform bool isReflectRefract;
uniform float reflectBlendRatio;		// How much reflection (0-1)
uniform float refractBlendRatio;		// How much refraction (0-1)
uniform float coefficientRefract; 		// coefficient of refraction 


/*****************************************************/
struct sLightDesc {
	vec4 position;
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;		// Colour (xyz), intensity (w)
	vec4 attenuation;	// x = constant, y = linear, z = quadratic
	vec4 direction;
	vec4 typeParams;	// x = type
						// 		0 = point
						// 		1 = directional
						// 		2 = spot
						// y = distance cut-off
	                    // z angle1, w = angle2		- only for spot
};

const int NUMBEROFLIGHTS = 10;
uniform sLightDesc myLight[NUMBEROFLIGHTS];

// Calculate the contribution of a light at a vertex
vec3 calcLightColour( in vec3 vecNormal, 
                      in vec3 vecWorldPosition, 
                      in int lightID, 
					  in vec3 matDiffuse, 
                      in vec4 matSpecular );
/*****************************************************/



void main()
{	

	fragColourOut = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// Is this a 'debug' wireframe object, i.e. no lighting, just use diffuse
	if ( bIsDebugWireFrameObject )
	{
		if (hasColour)
		{
			fragColourOut.rgb = color.rgb;
			fragColourOut.a = color.a;
			return;		// Immediate return
		}
		else
		{
			fragColourOut.rgb = materialDiffuse.rgb;
			fragColourOut.a = materialDiffuse.a;
			return;		// Immediate return
		}
	}
	
	
	if ( isASkyBox )
	{	// Sample from skybox texture and exit
		// I pass texture coords to 'sample' 
		//	returning a colour (at that point in the texture)
		// Note we are using the normals of our skybox object
		//	to determine the point on the inside of the box
		vec4 skyRGBA = texture( texSampCube00, vertNormal.xyz );
		
		fragColourOut = vec4(skyRGBA.rgb, 1.0f);		//gl_FragColor = skyRGBA;
		return;	
	}
	
	if ( isReflectRefract )
	{			
		// Have "eyePosition" (camera eye) in WORLD space
		
		// reFLECTion value 
		vec3 vecReflectEyeToVertex = vecWorldPosition - eyePosition;
		vecReflectEyeToVertex = normalize(vecReflectEyeToVertex);
		vec3 vecReflect = reflect( vecReflectEyeToVertex, vertNormal.xyz );
		// Look up colour for reflection

		// WHY this twice assignment???
		vec4 rgbReflection = texture( texSampCube00, vertNormal.xyz );

		rgbReflection = texture( texSampCube00, vecReflect );
		rgbReflection.rgb * 0.01f;
		rgbReflection.rgb += normalize(vertNormal.xyz);
		
		
		vec3 vecReFRACT_EyeToVertex = eyePosition - vecWorldPosition;
		vecReFRACT_EyeToVertex = normalize(vecReFRACT_EyeToVertex);				
		vec3 vecRefract = refract( vecReFRACT_EyeToVertex, vertNormal.xyz, 
                                   coefficientRefract );
		// Look up colour for reflection
		vec4 rgbRefraction = texture( texSampCube00, vecRefract );
		
		
		// Mix the two, based on how reflective the surface is
		fragColourOut = (rgbReflection * reflectBlendRatio) + 
		                (rgbRefraction * refractBlendRatio);
		
		return;	
	}	
	
	vec3 matDiffuse = vec3(0.0f, 0.0f, 0.0f);
	
	// ****************************************************************/
	vec2 theUVCoords = uvX2out.xy;		// use UV #1 of vertex
		
	vec4 texCol00 = texture( texSamp2D00, theUVCoords.xy );
	vec4 texCol01 = texture( texSamp2D01, theUVCoords.xy );
	vec4 texCol02 = texture( texSamp2D02, theUVCoords.xy );
	vec4 texCol03 = texture( texSamp2D03, theUVCoords.xy );
	vec4 texCol04 = texture( texSamp2D04, theUVCoords.xy );
	vec4 texCol05 = texture( texSamp2D05, theUVCoords.xy );
	vec4 texCol06 = texture( texSamp2D06, theUVCoords.xy );
	vec4 texCol07 = texture( texSamp2D07, theUVCoords.xy );
	//... and so on (to how many textures you are using)

	matDiffuse.rgb += (texCol00.rgb * texBlend00) + 
	                  (texCol01.rgb * texBlend01) + 
					  (texCol02.rgb * texBlend02) + 
					  (texCol03.rgb * texBlend03) +
					  (texCol04.rgb * texBlend04) +
					  (texCol05.rgb * texBlend05) +
					  (texCol06.rgb * texBlend06) +
					  (texCol07.rgb * texBlend07);
	
					  

	//****************************************************************/	
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{
		fragColourOut.rgb += calcLightColour( vertNormal, 					
		                                      vecWorldPosition, 
											  index, 
		                                      matDiffuse, 
											  materialSpecular );
	}


	vec3 ambientContribution = matDiffuse.rgb * ambientToDiffuseRatio;
	fragColourOut.rgb += ambientContribution.rgb;		
	
	// Copy object material diffuse to alpha
	if (hasAlpha)
	{
		if (texCol01.r < 0.5)
			discard;
		fragColourOut.a = texCol01.r;
	}
	else
	{
		fragColourOut.a = materialDiffuse.a;
	}
	
	return;
}

// Calcualte the contribution of a light at a vertex
vec3 calcLightColour( in vec3 vecNormal, 
                      in vec3 vecWorldPosition, 
                      in int lightID, 
                      in vec3 matDiffuse, 	
                      in vec4 matSpecular )	
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
			             * color.rgb						// Material contribution
						 * diffFactor;						// Factor based on direction
	}
	else	
	{
		outDiffuse.rgb = myLight[lightID].diffuse.rgb 		// Light contribution
			             * matDiffuse.rgb					// Material contribution
						 * diffFactor;
	}
					 


	float attenuation = 1.0f / 
	   ( myLight[lightID].attenuation.x										// Constant  
	   + myLight[lightID].attenuation.y * lightDistance						// Linear
	   + myLight[lightID].attenuation.z * lightDistance * lightDistance );	// Quad
	outDiffuse.rgb *= attenuation;

	
	// Vector from vertex to eye 
	vec3 viewVector = normalize( eyePosition - vecWorldPosition );
	vec3 vecLightReflection = reflect( normalize(lightVector), vecNormal );
	
	float specularShininess = matSpecular.w;	
	vec3 specMatColour = matSpecular.rgb;		
	
	outSpecular.rgb = pow( max(0.0f, dot(viewVector,vecLightReflection)), 
	                  specularShininess)
			          * specMatColour
				      * myLight[lightID].specular.rgb;
				   
	outSpecular *= attenuation;
	
	//colour = outDiffuse + outSpecular;
	// No specular for now...
	colour = outDiffuse;

	// Now we have the colour if this was a point light 
	// See if it's a spot light...
	if ( myLight[lightID].typeParams.x == 2.0f ) 			// x = type
	{	// Then it's a spot light! 
		// 		0 = point
		// 		1 = directional
		// 		2 = spot
	    // z angle1, w = angle2		- only for spot
		
		// Vector from the vertex to the light... 
		vec3 vecVertexToLight = vecWorldPosition - myLight[lightID].position.xyz;
		// Normalize to unit length vector
		vec3 vecVtoLDirection = normalize(vecVertexToLight);
		
		float vertSpotAngle = max(0.0f, dot( vecVtoLDirection, myLight[lightID].direction.xyz ));
		
		// Is this withing the angle1?
		float angleInsideCutCos = cos(myLight[lightID].typeParams.z);		// z angle1
		float angleOutsideCutCos = cos(myLight[lightID].typeParams.w);		// z angle2
		
		if ( vertSpotAngle <= angleOutsideCutCos )
		{	// NO, it's outside this angle1
			colour = vec3(0.0f, 0.0f, 0.0f );
		}
		else if ( (vertSpotAngle > angleOutsideCutCos ) && 
		          (vertSpotAngle <= angleInsideCutCos) )
		{	// inside the 'penumbra' region
			// Use smoothstep to get the gradual drop off in brightness
			float penRatio = smoothstep(angleOutsideCutCos, 
			                            angleInsideCutCos, 
										vertSpotAngle );          
			
			colour *= penRatio;
		}

	}//if ( typeParams.x

	
	return colour;
}// vec3 calcLightColour(...) 





