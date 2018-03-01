// Debug FRAGMENT shader
#version 420  
											
in vec4 fVertColour;     

// gl_FragColor is depricated
out vec4 outputColour;                    
											
void main()                                
{                                          
	outputColour.rgb = fVertColour.rgb;     
	outputColour.a = fVertColour.a;      

//	gl_FragColor.rgb = vec3(1.0f, 1.0f, 0.0f);
//	gl_FragColor.a = 1.0f;
}
