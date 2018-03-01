// Debug VERTEX shader
#version 420                                   
												
uniform mat4 mModel;    	                         
uniform mat4 mView;                             
uniform mat4 mProjection;                       
		
// in from the vertex buffer		
in vec4 vPosition;                             
in vec4 vColour;                                
				
// Going out to??? 				
out vec4 gVertColour;                           
												
void main()                                    
{                                               
//	mat4 MVP = mProjection * mView * mModel;    
//	gl_Position = MVP * vPosition;   

	// Am NOT transforming in the vertex shader
	gl_Position = vPosition;
									
	gVertColour = vColour;                       
}
