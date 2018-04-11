// Fragment shader
#version 400

in vec4 fColor;					
in vec3 fVertNormal;			// Also in "world" (no view or projection)
in vec3 fVecWorldPosition;		// 
in vec4 fUV_X2;					// Added: UV 1 and 2 to fragment
								// UV #1 is .xy 
								// UV #2 is .zw
in vec3 fTangent;		// For bump (or normal) mapping
in vec3 fBitangent;		// For bump (or normal) mapping	
						
						
// gl_FragColor is deprecated after version 120
// Now we specify a specific variable.
// If there is only 1, then GL will assume it's the colour 
out vec4 fragOut_colour;
out vec4 fragOut_normal;
out vec4 fragOut_vertexWorldPos;

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
uniform bool useDiscardAlpha;
uniform bool hasReflection;

uniform int renderPassNumber;	//FBO

//uniform sampler2D tex2ndPassSamp2D;		// Offscreen texture for 2nd pass
uniform sampler2D texFBOColour2D;
uniform sampler2D texFBONormal2D;
uniform sampler2D texFBOVertexWorldPos2D;

uniform sampler2D fullRenderedImage2D;
uniform sampler2D fullRenderedImage2D_Overlay;

uniform float screenWidth;
uniform float screenHeight;

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
uniform samplerCube texSampCube04;
uniform float texCubeBlend00;
uniform float texCubeBlend01;
uniform float texCubeBlend02;
uniform float texCubeBlend03;
uniform float texCubeBlend04;

// For env. mapping (reflection and refraction)
uniform bool isReflectRefract;
uniform float reflectBlendRatio;		// How much reflection (0-1)
uniform float refractBlendRatio;		// How much refraction (0-1)
uniform float coefficientRefract; 		// coefficient of refraction 

// For rthe static effect
uniform float staticEffect;
uniform float staticFade;


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
	
	vec4 typeParams2;	// x = lightPower
};

const int NUMBEROFLIGHTS = 20;
uniform sLightDesc myLight[NUMBEROFLIGHTS];

// Calculate the contribution of a light at a vertex
vec3 calcLightColour( in vec3 fVertNormal, 
                      in vec3 fVecWorldPosition, 
                      in int lightID, 
					  in vec3 matDiffuse, 
                      in vec4 matSpecular );

/*****************************************************/

const float CALCULATE_LIGHTING = 1.0f;
const float DONT_CALCULATE_LIGHTING = 0.25f;

const int PASS_0_G_BUFFER_PASS = 0;
const int PASS_1_DEFERRED_RENDER_PASS = 1;
const int PASS_2_FULL_SCREEN_EFFECT_PASS = 2;
const int PASS_3_FULL_SCREEN_EFFECT_PASS_2 = 3;


void main()
{	

	fragOut_colour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	fragOut_normal = vec4( 0.0f, 0.0f, 0.0f, DONT_CALCULATE_LIGHTING );
	fragOut_vertexWorldPos = vec4( 0.0f, 0.0f, 0.0f, 1.0f );

		switch (renderPassNumber)
	{
	case PASS_0_G_BUFFER_PASS:	 // (0)
	{
			// Is this a 'debug' wireframe object, i.e. no lighting, just use diffuse
		if ( bIsDebugWireFrameObject )
		{
			if (hasColour)
			{
				fragOut_colour.rgb = fColor.rgb;
				fragOut_colour.a = fColor.a;
			}
			else
			{
				fragOut_colour.rgb = materialDiffuse.rgb;
				fragOut_colour.a = materialDiffuse.a;
			}

			fragOut_vertexWorldPos.xyz = fVecWorldPosition.xyz;
			fragOut_normal.a = DONT_CALCULATE_LIGHTING;
			return;		// Immediate return
		}
		
		
		if ( isASkyBox )
		{	// Sample from skybox texture and exit
			// I pass texture coords to 'sample' 
			//	returning a colour (at that point in the texture)
			// Note we are using the normals of our skybox object
			//	to determine the point on the inside of the box
			vec4 skyRGBA = texture(texSampCube00, fVertNormal.xyz ) * texCubeBlend00 +
									texture( texSampCube01, fVertNormal.xyz ) * texCubeBlend01 +
									texture( texSampCube02, fVertNormal.xyz ) * texCubeBlend02 +
									texture( texSampCube03, fVertNormal.xyz ) * texCubeBlend03 +
									texture( texSampCube04, fVertNormal.xyz ) * texCubeBlend04;
			
			fragOut_colour = vec4(skyRGBA.rgb, 1.0f);		//gl_FragColor = skyRGBA;

			fragOut_vertexWorldPos.xyz = fVecWorldPosition.xyz;
			fragOut_normal.a = DONT_CALCULATE_LIGHTING;

			return;	
		}
		
		if ( isReflectRefract )
		{			
			// Have "eyePosition" (camera eye) in WORLD space
			
			// reFLECTion value 
			vec3 vecReflectEyeToVertex = fVecWorldPosition - eyePosition;
			//vec3 vecReflectEyeToVertex = eyePosition - vecWorldPosition;
			vecReflectEyeToVertex = normalize(vecReflectEyeToVertex);
			vec3 vecReflect = reflect( vecReflectEyeToVertex, fVertNormal.xyz );
			
			// Look up colour for reflection
			
			// We probably wont need this enymore, but will be here just in case...
			//vec4 rgbReflection = texture(texSampCube00, fVertNormal.xyz ) * texCubeBlend00 +
			//						texture( texSampCube01, fVertNormal.xyz ) * texCubeBlend01 +
			//						texture( texSampCube02, fVertNormal.xyz ) * texCubeBlend02 +
			//						texture( texSampCube03, fVertNormal.xyz ) * texCubeBlend03);

			vec4 rgbReflection = texture(texSampCube00, fVertNormal.xyz ) * texCubeBlend00 +
									texture( texSampCube01, fVertNormal.xyz ) * texCubeBlend01 +
									texture( texSampCube02, fVertNormal.xyz ) * texCubeBlend02 +
									texture( texSampCube03, fVertNormal.xyz ) * texCubeBlend03 +
									texture( texSampCube04, fVertNormal.xyz ) * texCubeBlend04;

			rgbReflection.rgb * 0.01f;
			rgbReflection.rgb += normalize(fVertNormal.xyz);
			
			
			vec3 vecReFRACT_EyeToVertex = eyePosition - fVecWorldPosition;
			vecReFRACT_EyeToVertex = normalize(vecReFRACT_EyeToVertex);				
			vec3 vecRefract = refract( vecReFRACT_EyeToVertex, fVertNormal.xyz, 
		                               coefficientRefract );
			// Look up colour for reflection
			vec4 rgbRefraction = texture(texSampCube00, fVertNormal.xyz ) * texCubeBlend00 +
									texture( texSampCube01, fVertNormal.xyz ) * texCubeBlend01 +
									texture( texSampCube02, fVertNormal.xyz ) * texCubeBlend02 +
									texture( texSampCube03, fVertNormal.xyz ) * texCubeBlend03 +
									texture( texSampCube04, fVertNormal.xyz ) * texCubeBlend04;
			
			
			// Mix the two, based on how reflective the surface is
			fragOut_colour = (rgbReflection * reflectBlendRatio) + 
			                (rgbRefraction * refractBlendRatio);
			
			//fragOut_colour.r = 1.0f;
			
			fragOut_vertexWorldPos.xyz = fVecWorldPosition.xyz;
			fragOut_normal.a = DONT_CALCULATE_LIGHTING;

			return;	
		}	
		
		vec3 matDiffuse = vec3(0.0f, 0.0f, 0.0f);
		
		// ****************************************************************/
		vec2 theUVCoords = fUV_X2.xy;		// use UV #1 of vertex
			
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
		
						  




		vec3 ambientContribution = matDiffuse.rgb * ambientToDiffuseRatio;
		fragOut_colour.rgb += ambientContribution.rgb;	
		
		// Transparency value (for alpha blending)
		fragOut_colour.a = materialDiffuse.a;

		fragOut_normal.rgb = fVertNormal.xyz;

		fragOut_vertexWorldPos.xyz = fVecWorldPosition.xyz;

		fragOut_normal.a = CALCULATE_LIGHTING;
		
		if ( hasReflection )
		{
			vec3 eyeDir = fVecWorldPosition - eyePosition;		
			vec3 reflectedDirection = normalize(reflect(eyeDir, normalize(fVertNormal)));
			vec4 fragColor = texture(texSampCube00, fVertNormal.xyz ) * texCubeBlend00 +
									texture( texSampCube01, fVertNormal.xyz ) * texCubeBlend01 +
									texture( texSampCube02, fVertNormal.xyz ) * texCubeBlend02 +
									texture( texSampCube03, fVertNormal.xyz ) * texCubeBlend03 +
									texture( texSampCube04, fVertNormal.xyz ) * texCubeBlend04;
			vec4 matReflect = texCol02;
			fragOut_colour += fragColor * matReflect;
			fragOut_colour.rgb += ambientContribution.rgb;	

			fragOut_normal.a = DONT_CALCULATE_LIGHTING;

			fragOut_colour *= 0.1f;
		}	

		// Copy object material diffuse to alpha
		if (hasAlpha)
		{
			if (useDiscardAlpha)
			{
				if (texCol01.r < 0.5)
					discard;
			}
			fragOut_colour.a = texCol01.r;
		}
		else
		{
			fragOut_colour.a = materialDiffuse.a;
		}

	//****************************************************************/	
		for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
		{
			fragOut_colour.rgb += calcLightColour( fVertNormal, 					
			                                      fVecWorldPosition, 
												  index, 
			                                      matDiffuse, 
												  materialSpecular );
		}
	}
		break;	// end of PASS_0_G_BUFFER_PASS (0):
	case PASS_1_DEFERRED_RENDER_PASS:	// (1)
	{
		vec2 textCoords = vec2( gl_FragCoord.x / screenWidth, gl_FragCoord.y / screenHeight );

		vec3 theColourAtThisPixel = texture( texFBOColour2D, textCoords).rgb;
		vec4 theNormalAtThisPixel = texture( texFBONormal2D, textCoords).rgba;
		vec3 theVertLocWorldAtThisPixel = texture( texFBOVertexWorldPos2D, textCoords).rgb;

		if ( theNormalAtThisPixel.a != CALCULATE_LIGHTING )
		{
			// Return the colour as it is on the colour FBO
			fragOut_colour.rgb = theColourAtThisPixel.rgb;
			fragOut_colour.a = 1.0f;
		}
		else
		{
			// ELSE: do the lighting...
			for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
			{
				fragOut_colour.rgb += calcLightColour( theNormalAtThisPixel.xyz, 					
													   theVertLocWorldAtThisPixel, 
													   index, 
													   theColourAtThisPixel, 
													   materialSpecular );
			}
		}// if ( theNormalAtThisPixel.a != CALCULATE_LIGHTING )


		fragOut_colour.rgb *= 7.0f;		// dim projector
		fragOut_colour.a = 1.0f;
	}
		break;	// end of pass PASS_1_DEFERRED_RENDER_PASS (1)
	case PASS_2_FULL_SCREEN_EFFECT_PASS:	// (2)
	{
		vec2 theUVCoords = fUV_X2.xy;
		theUVCoords.x += staticEffect;
		theUVCoords.y += staticEffect;
		fragOut_colour.rgb = texture( fullRenderedImage2D, fUV_X2.xy ).rgb * (1.0f - staticFade);
		fragOut_colour.rgb += texture( fullRenderedImage2D_Overlay, theUVCoords.xy).rgb * staticFade;
		fragOut_colour.a = 1.0f;
	}
		break;	// end of pass PASS_2_FULL_SCREEN_EFFECT_PASS:
	case PASS_3_FULL_SCREEN_EFFECT_PASS_2:	// (2)
	{
		vec2 textCoords = vec2( gl_FragCoord.x / screenWidth, gl_FragCoord.y / screenHeight );


		fragOut_colour.rgb = texture( fullRenderedImage2D, textCoords).rgb;
		fragOut_colour.a = 1.0f;
	}
		break;	// end of pass PASS_3_FULL_SCREEN_EFFECT_PASS_2:
	}
	
	return;
}

// Calcualte the contribution of a light at a vertex
vec3 calcLightColour( in vec3 vecNormal, 
                      in vec3 fVecWorldPosition, 
                      in int lightID, 
                      in vec3 matDiffuse, 	
                      in vec4 matSpecular )	
{
	// Result Colour
	vec3 resultColour = vec3( 0.0f, 0.0f, 0.0f );

	// Distance between light and vertex (in world)
	vec3 vecToLight = myLight[lightID].position.xyz - fVecWorldPosition;
	float lightDistance = length(vecToLight);
	
	// Short circuit distance
	if ( myLight[lightID].typeParams.y <  lightDistance )
	{
		return resultColour;
	}

	// Material diffuse
	resultColour = matDiffuse;

	// Ambient
	vec3 norm = normalize(vecNormal);
	vec3 lightDir = normalize(vecToLight); 

	// Diffuse
	float diffusePower = max(dot(norm, lightDir), 0.0);
	vec3 diffuseLight = diffusePower * myLight[lightID].diffuse.rgb;
	resultColour *= diffuseLight;

	// Light Power
	float lightPower = myLight[lightID].typeParams2.x * 2.0f;
	resultColour *= lightPower;

	// Specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(eyePosition - fVecWorldPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specularShininess = matSpecular.w;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularShininess);
	vec3 specular;
	if (hasColour)
	{
		specular = matSpecular.rgb * spec * myLight[lightID].diffuse.rgb; 
	}
	else
	{
		specular = texture( texSamp2D03, fUV_X2.xy ).rgb * spec * myLight[lightID].diffuse.rgb; 
	}
	resultColour += specular;

	// Attenuation
	float attenuation = 1.0f / 
	   ( myLight[lightID].attenuation.x										// Constant  
	   + myLight[lightID].attenuation.y * lightDistance						// Linear
	   + myLight[lightID].attenuation.z * lightDistance * lightDistance );	// Quad
	resultColour *= attenuation;

	// Spot light
	if ( myLight[lightID].typeParams.x == 2.0f )	// x = type
	{	 
		// 		0 = point
		// 		1 = directional
		// 		2 = spot
		//		z angle1, w = angle2
	
		// Vector from the vertex to the light... 
		vec3 vecVertexToLight = fVecWorldPosition - myLight[lightID].position.xyz;
		
		// Normalize to unit length vector
		vec3 vecVtoLDirection = normalize(vecVertexToLight);
		
		float vertSpotAngle = max(0.0f, dot( vecVtoLDirection, myLight[lightID].direction.xyz ));
		
		float angleInsideCutCos = cos(myLight[lightID].typeParams.z);		// z angle1
		float angleOutsideCutCos = cos(myLight[lightID].typeParams.w);		// z angle2
		
		// Is this withing the angle1?
		if ( vertSpotAngle <= angleOutsideCutCos )
		{	
			resultColour = vec3(0.0f, 0.0f, 0.0f );
		}
		else if ( (vertSpotAngle > angleOutsideCutCos ) && 
		          (vertSpotAngle <= angleInsideCutCos) )
		{
			// Use smoothstep to get the gradual drop off in brightness
			float penumbraRatio = smoothstep(angleOutsideCutCos, 
											 angleInsideCutCos, 
											 vertSpotAngle );          
			
			resultColour *= penumbraRatio;
		}
	}

	// Result
	return resultColour;
}