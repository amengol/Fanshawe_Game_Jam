// Debug FRAGMENT shader
#version 420  
											
in vec4 vertColour;     

// gl_FragColor is depricated
out vec4 outputColour;                    
											
void main()                                
{                                          
	outputColour.rgb = vertColour.rgb;     
	outputColour.a = vertColour.a;      
}
