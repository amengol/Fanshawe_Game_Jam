// Debug FRAGMENT shader
#version 420  
											
in vec4 fVertColour;     

// gl_FragColor is depricated
out vec4 outputColour;                    
											
void main()                                
{                                          
	outputColour.rgb = fVertColour.rgb;     
	outputColour.a = fVertColour.a;      
}
