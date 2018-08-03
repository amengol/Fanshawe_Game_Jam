#version 400

// Ins
in vec4 viewSpace;
in vec4 FragPosLightSpace;	// The space for shadow calculation
in vec4 fColor;					
in vec3 fVertNormal;			
in vec3 fVecWorldPosition;	
in vec4 fUV_X2;				// UV #1 is .xy UV #2 is .zw
in mat3 fTBN;				// Tangent Bitangent Normal matrix	

// Outs
out vec4 fragOut_colour;
out vec4 fragOut_normal;
out vec4 fragOut_vertexWorldPos;

// Fog params
float fogFactor;
uniform vec3 fogColour;
uniform float fogPercent;
uniform bool fogActive;

// For env. mapping (reflection and refraction)
uniform bool isReflectRefract;
uniform float reflectBlendRatio;		// How much reflection (0-1)
uniform float refractBlendRatio;		// How much refraction (0-1)
uniform float coefficientRefract; 		// coefficient of refraction 

// Other Uniforms	
uniform int renderPassNumber;			//FBO				
uniform vec4 materialDiffuse;	
uniform float ambientToDiffuseRatio;
uniform vec4 materialSpecular;  		// rgb = colour of HIGHLIGHT only, w = shininess of the 
uniform vec3 eyePosition;				// Camera position
uniform bool bIsDebugWireFrameObject;	// Render in Wireframe
uniform bool hasColour;
uniform bool hasAlpha;
uniform bool useDiscardAlpha;
uniform bool hasReflection;
uniform bool hasMultiLayerTextures;
uniform bool receiveShadow;
uniform bool selfLight;
uniform bool hasNormalMap;
uniform bool isASkyBox;	
uniform float fade;
uniform float noise;
uniform bool noiseEffectOn;
uniform float sysTime;					// System Time
uniform float gamaCorrection;
uniform float screenWidth;
uniform float screenHeight;

// Texture samplers and blends
uniform sampler2D texSamp2D00;		
uniform sampler2D texSamp2D01;		
uniform sampler2D texSamp2D02;		
uniform sampler2D texSamp2D03;		
uniform sampler2D texSamp2D04;		
uniform sampler2D texSamp2D05;		
uniform sampler2D texSamp2D06;		
uniform sampler2D texSamp2D07;
uniform float texBlend00;		
uniform float texBlend01;		
uniform float texBlend02;	
uniform float texBlend03;		
uniform float texBlend04;
uniform float texBlend05;		
uniform float texBlend06;
uniform float texBlend07;
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
uniform sampler2D texFBOColour2D;
uniform sampler2D texFBONormal2D;
uniform sampler2D texFBOVertexWorldPos2D;
uniform sampler2D fullRenderedImage2D;
uniform sampler2D fullRenderedImage2D_Alpha;
uniform sampler2D fullRenderedImage2D_Overlay;
uniform sampler2D shadowMap;

// Render Passes
const int DEPTH_RENDER_PASS = 0;
const int FULL_SCENE_RENDER_PASS = 1;
const int DEFERRED_RENDER_PASS = 2;
const int FINAL_RENDER_PASS = 99;

// Consts for calculating lighting
const float CALCULATE_LIGHTING = 1.0f;
const float DONT_CALCULATE_LIGHTING = 0.25f;

// Lights
struct sLightDesc {
    vec4 position;
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;		// Colour (xyz), intensity (w)
    vec4 attenuation;	// x = constant, y = linear, z = quadratic
    vec4 direction;
    vec4 typeParams;	// x = type
                        // 		0 = directional
                        // 		2 = point
                        // 		4 = spot
                        // y = distance cut-off
                        // z angle1, w = angle2		- only for spot
    vec4 typeParams2;	// x = lightPower
};
const int NUMBEROFLIGHTS = 20;
uniform sLightDesc myLight[NUMBEROFLIGHTS];

// Shadows calculation
float ShadowCalculation(vec4 fragPosLightSpace, vec3 vecNormal, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
       
    // Bias
    float bias = 0.0005;
    //float bias = max(0.05 * (1.0 - dot(fVertNormal, lightDir)), 0.0005);  

    // PCF (percentage-closer filtering)
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            // check whether current frag pos is in shadow
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;	// Because we did it 9x

    return shadow;
}

// Calcualte the contribution of a light at a vertex
vec3 calcLightColour(vec3 vecNormal, vec3 fVecWorldPosition, int lightID, vec3 matDiffuse, vec4 matSpecular )	
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

    // Light Direction	
    vec3 lightDir = normalize(vecToLight); 

    // Calculate shadow
    float shadow = 0.0f;
    if (myLight[lightID].typeParams.x == 0.0f) // Only for directional lights
    {
        if (receiveShadow)
        {
            shadow = ShadowCalculation(FragPosLightSpace, vecNormal, lightDir);
            
            if (shadow == 1.0f)
            {
                return resultColour;
            }
        }
    }

    // Material diffuse
    resultColour = matDiffuse;
    vec3 norm = normalize(vecNormal);

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
        specular = matSpecular.rgb * spec * myLight[lightID].diffuse.rgb * myLight[lightID].specular.rgb; 
    }
    else
    {
        specular = texture( texSamp2D03, fUV_X2.xy ).rgb * spec * myLight[lightID].diffuse.rgb * myLight[lightID].specular.rgb; 
    }
    resultColour += specular;

    // Attenuation
    float attenuation = 1.0f / 
       ( myLight[lightID].attenuation.x										// Constant  
       + myLight[lightID].attenuation.y * lightDistance						// Linear
       + myLight[lightID].attenuation.z * lightDistance * lightDistance );	// Quad
    resultColour *= attenuation;

    // Spot light
    if ( myLight[lightID].typeParams.x == 4.0f )	// x = type
    {	 
        // 		0 = directional
        // 		2 = point
        // 		4 = spot
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
    return resultColour * (1 - shadow);
}

void main()
{	
    // Fog range based
    if (fogActive)
    {
        float dist = length(viewSpace);
        // 8 - fog starts; 40 - fog ends
        fogFactor = (40.0 - dist)/(40.0 - 8.0);
        fogFactor = clamp( fogFactor, 0.0, 1.0 );
        fogFactor += (1.0 - fogFactor) * (1.0f - fogPercent);
    }
    else
    {
        fogFactor = 1.0f;
    }

    fragOut_colour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    fragOut_normal = vec4( 0.0f, 0.0f, 0.0f, DONT_CALCULATE_LIGHTING );
    fragOut_vertexWorldPos = vec4( 0.0f, 0.0f, 0.0f, 1.0f );

    switch (renderPassNumber)
    {
    case DEPTH_RENDER_PASS:
    {
        if (hasAlpha)
        {
            vec4 textAlpha = texture( texSamp2D01, fUV_X2.xy );
            if (textAlpha.r < 0.5f)
                discard;
        }
    } // !case DEPTH_RENDER_PASS
        break;
    case FULL_SCENE_RENDER_PASS:
    {
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
        {	
            // Sample from skybox texture and exit
            vec4 skyRGBA = texture(texSampCube00, fVertNormal.xyz ) * texCubeBlend00 +
                                    texture( texSampCube01, fVertNormal.xyz ) * texCubeBlend01 +
                                    texture( texSampCube02, fVertNormal.xyz ) * texCubeBlend02 +
                                    texture( texSampCube03, fVertNormal.xyz ) * texCubeBlend03 +
                                    texture( texSampCube04, fVertNormal.xyz ) * texCubeBlend04;
            fragOut_colour = vec4(skyRGBA.rgb, 1.0f);
            fragOut_vertexWorldPos.xyz = fVecWorldPosition.xyz;
            fragOut_normal.a = DONT_CALCULATE_LIGHTING;

            // Fog calc
            fragOut_colour.rgb = mix(fogColour, skyRGBA.rgb, fogFactor);
            return;	
        }
        
        if ( isReflectRefract )
        {			
            // reFLECTion value 
            vec3 vecReflectEyeToVertex = fVecWorldPosition - eyePosition;
            vecReflectEyeToVertex = normalize(vecReflectEyeToVertex);
            vec3 vecReflect = reflect( vecReflectEyeToVertex, fVertNormal.xyz );
            
            // Look up colour for reflection
            vec4 rgbReflection = texture(texSampCube00, fVertNormal.xyz ) * texCubeBlend00 +
                                    texture( texSampCube01, fVertNormal.xyz ) * texCubeBlend01 +
                                    texture( texSampCube02, fVertNormal.xyz ) * texCubeBlend02 +
                                    texture( texSampCube03, fVertNormal.xyz ) * texCubeBlend03 +
                                    texture( texSampCube04, fVertNormal.xyz ) * texCubeBlend04;
            rgbReflection.rgb * 0.01f;	// Do not make sense. Investigate after the Game Jam
                                        // as we not useing any reflections now
            rgbReflection.rgb += normalize(fVertNormal.xyz);
            
            // reFRACTion value
            vec3 vecReFRACT_EyeToVertex = eyePosition - fVecWorldPosition;
            vecReFRACT_EyeToVertex = normalize(vecReFRACT_EyeToVertex);				
            vec3 vecRefract = refract( vecReFRACT_EyeToVertex, fVertNormal.xyz, 
                                       coefficientRefract );
            // Look up colour for refraction
            vec4 rgbRefraction = texture(texSampCube00, fVertNormal.xyz ) * texCubeBlend00 +
                                    texture( texSampCube01, fVertNormal.xyz ) * texCubeBlend01 +
                                    texture( texSampCube02, fVertNormal.xyz ) * texCubeBlend02 +
                                    texture( texSampCube03, fVertNormal.xyz ) * texCubeBlend03 +
                                    texture( texSampCube04, fVertNormal.xyz ) * texCubeBlend04;
            
            // Mix the two, based on how reflective the surface is
            fragOut_colour = (rgbReflection * reflectBlendRatio) + 
                            (rgbRefraction * refractBlendRatio);

            fragOut_vertexWorldPos.xyz = fVecWorldPosition.xyz;
            fragOut_normal.a = DONT_CALCULATE_LIGHTING;

            return;	
        }	
        
        vec3 matDiffuse = vec3(0.0f, 0.0f, 0.0f);
        vec2 theUVCoords = fUV_X2.xy;		// use UV #1 of vertex
        vec2 theUV2Coords = fUV_X2.zw;		// use UV #2 of vertex
        // Texel colours	
        vec4 texCol00;
        vec4 texCol01;
        vec4 texCol02;
        vec4 texCol03;
        vec4 texCol04;
        vec4 texCol05;
        vec4 texCol06;
        vec4 texCol07;

        if (hasMultiLayerTextures)
        {
            texCol00 = texture( texSamp2D00, theUVCoords );
            texCol01 = texture( texSamp2D01, theUVCoords );
            texCol02 = texture( texSamp2D02, theUVCoords );
            texCol03 = texture( texSamp2D03, theUVCoords );
            texCol04 = texture( texSamp2D04, theUVCoords );
            texCol05 = texture( texSamp2D05, theUVCoords );
            texCol06 = texture( texSamp2D06, theUVCoords );
            texCol07 = texture( texSamp2D07, theUV2Coords );

            // Filter them by the texCol07 RGB
            // textCol00 is filtered by texCol07 GREEN
            // textCol01 is filtered by texCol07 RED
            // textCol02 is filtered by texCol07 BLUE
            matDiffuse.rgb += (texCol00.rgb * texCol07.g) + 
                              (texCol01.rgb * texCol07.r) + 
                              (texCol02.rgb * texCol07.b) + 
                              (texCol03.rgb * texBlend03) +
                              (texCol04.rgb * texBlend04) +
                              (texCol05.rgb * texBlend05) +
                              (texCol06.rgb * texBlend06);
        }
        else
        {
            texCol00 = texture( texSamp2D00, theUVCoords );
            texCol01 = texture( texSamp2D01, theUVCoords );
            texCol02 = texture( texSamp2D02, theUVCoords );
            texCol03 = texture( texSamp2D03, theUVCoords );
            texCol04 = texture( texSamp2D04, theUVCoords );
            texCol05 = texture( texSamp2D05, theUVCoords );
            texCol06 = texture( texSamp2D06, theUVCoords );
            texCol07 = texture( texSamp2D07, theUVCoords );

            matDiffuse.rgb += (texCol00.rgb * texBlend00) + 
                              (texCol01.rgb * texBlend01) + 
                              (texCol02.rgb * texBlend02) + 
                              (texCol03.rgb * texBlend03) +
                              (texCol04.rgb * texBlend04) +
                              (texCol05.rgb * texBlend05) +
                              (texCol06.rgb * texBlend06) +
                              (texCol07.rgb * texBlend07);
        }

        // Transparency value (for alpha blending)
        fragOut_colour.a = materialDiffuse.a;

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

        // Self inlumination
        if (selfLight)
        {
            fragOut_colour.rgb = matDiffuse.rgb * 1.5f;
            fragOut_colour.rgb = mix(fogColour, fragOut_colour.rgb, fogFactor);
            return;
        }

        // Ambient
        vec3 ambientContribution = matDiffuse.rgb * ambientToDiffuseRatio;
        fragOut_colour.rgb += ambientContribution.rgb;	
        
        // Normal
        vec3 normal;
        if (hasNormalMap)
        {	
            if (hasMultiLayerTextures)
            {
                // textCol04 is filtered by texCol07 GREEN
                // textCol05 is filtered by texCol07 RED
                // textCol06 is filtered by texCol07 BLUE
                normal = texCol04.rgb * texCol07.g + texCol05.rgb * texCol07.r +  texCol06.rgb * texCol07.b;
            }
            else
            {
                // obtain normal from normal map in range [0,1]
                normal = texCol04.rgb;
            }
            // transform normal vector to range [-1,1]
            normal = normalize(normal * 2.0 - 1.0);
            // transform to world space
            normal = normalize(fTBN * normal); 
        }
        else
        {
            normal = fVertNormal;
        }
        fragOut_normal.rgb = normal.xyz;
        fragOut_vertexWorldPos.xyz = fVecWorldPosition.xyz;
        fragOut_normal.a = CALCULATE_LIGHTING;
        
        // Own reflection calculation
        // After the Game Jam, remove and refine the Reflection/Refraction thingy
        if ( hasReflection )
        {
            vec3 eyeDir = fVecWorldPosition - eyePosition;		
            vec3 reflectedDirection = normalize(reflect(eyeDir, normalize(normal)));
            vec4 fragColor = texture(texSampCube00, normal.xyz ) * texCubeBlend00 +
                                    texture( texSampCube01, normal.xyz ) * texCubeBlend01 +
                                    texture( texSampCube02, normal.xyz ) * texCubeBlend02 +
                                    texture( texSampCube03, normal.xyz ) * texCubeBlend03 +
                                    texture( texSampCube04, normal.xyz ) * texCubeBlend04;
            vec4 matReflect = texCol02;
            fragOut_colour += fragColor * matReflect;
            fragOut_colour.rgb += ambientContribution.rgb;	
            fragOut_normal.a = DONT_CALCULATE_LIGHTING;

            fragOut_colour *= 0.1f;
        }	

        // Light Contribution
        vec3 lightColor = vec3(0.0f);
        for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
        {
            fragOut_colour.rgb += calcLightColour( normal, 					
                                                  fVecWorldPosition, 
                                                  index, 
                                                  matDiffuse, 
                                                  materialSpecular );	   
        }
        fragOut_colour.rgb = mix(fogColour, fragOut_colour.rgb, fogFactor);
    } // !case FULL_SCENE_RENDER_PASS
        break;
    case DEFERRED_RENDER_PASS:
    {
        // I could blend this pass with the FINAL pass, but I will be
        // using it for lightining calc per frag in the future

        vec2 textCoords = vec2( gl_FragCoord.x / screenWidth, gl_FragCoord.y / screenHeight );

        vec3 theColourAtThisPixel = texture( texFBOColour2D, textCoords).rgb;
        vec4 theNormalAtThisPixel = texture( texFBONormal2D, textCoords).rgba;
        vec3 theVertLocWorldAtThisPixel = texture( texFBOVertexWorldPos2D, textCoords).rgb;

        // Return the colour as it is on the colour FBO
        fragOut_colour.rgb = theColourAtThisPixel;
        fragOut_colour.a = 1.0f;

    } // !case DEFERRED_RENDER_PASS
        break;
    case FINAL_RENDER_PASS:
    {
        vec2 textCoords = vec2( gl_FragCoord.x / screenWidth, gl_FragCoord.y / screenHeight );

        // Main "Game" image
        vec4 mainImage = texture( fullRenderedImage2D, textCoords);
        // Gama correction
        mainImage.rgb = pow(mainImage.rgb, vec3(1.0/gamaCorrection));
        // Wave effect
        if (fade != 0.0f)
        {
            // The wave effect is being increased by the fade
            textCoords.x += (sin(textCoords.y * 10.0 + sysTime) / 10.0f) * fade;
        }
        
        // Overlay image (mainly Menus)
        vec4 overlayImage = texture( fullRenderedImage2D_Overlay, textCoords);
        if (noiseEffectOn)
        {
            vec2 newCoords = textCoords;
            newCoords.x += noise;
            newCoords.y -= noise;
            overlayImage *= texture( fullRenderedImage2D_Alpha, newCoords).r;
            // Nicely change from RGB to mono
            float mono = (0.2125 * overlayImage.r) + (0.7154 * overlayImage.g) + (0.0721 * overlayImage.b);
            overlayImage = vec4(vec3(mono), 1.0f);
            // Fine tune to Sepia
            overlayImage.r *= 1.2f;
            overlayImage.g *= 1.0f;
            overlayImage.b *= 0.8f;
        }
        
        fragOut_colour = mainImage * fade + overlayImage * (1.0f - fade);
        fragOut_colour.a = 1.0f;
    } // !case FINAL_RENDER_PASS
        break;
    } // !switch (renderPassNumber)
    
    return;
}

