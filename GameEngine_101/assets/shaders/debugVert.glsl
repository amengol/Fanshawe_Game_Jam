// Debug VERTEX shader
#version 420                                   
												   	                         
uniform mat4 mView;                             
uniform mat4 mProjection;                       
		
// in from the vertex buffer		
in vec3 vPosition;                             
in vec3 vColour;                                
							
out vec4 gVertColour;                           
												
void main()                                    
{                                               
	gl_Position = mProjection * mView * vec4(vPosition, 1.0f);	

	gVertColour = vec4(vColour, 1.0f);                       
}
