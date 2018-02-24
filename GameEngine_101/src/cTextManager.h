/**
    cTextManager.h
    Purpose: Controls and draws texts

    @author Jorge Amengol
    @date February 24th, 2018
*/
#pragma once
#include <string>
#include <vector>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "stb_image.h" 
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm\vec3.hpp>


class cTextManager
{
public:
    cTextManager();
    ~cTextManager();

    // Initiate all parameters
    bool init();

    // Draws text
    void draw(unsigned int width, unsigned int height);

    // Sets the text to be drawn
    void setText(std::vector<std::wstring> vecText, glm::vec3 color);

private:

    // Creates the shaders and compile the shader program
    GLboolean init_gl();

    // Init FreeType stuff
    GLboolean initfreetype();

    // Renders the vecStrings
    void renderVecStrings(std::vector<std::wstring> vecStrings, 
                          unsigned int width, unsigned int height);

    // Render the FreeType text
    void renderText(const wchar_t *text, float x, float y, float sx, float sy);

    GLuint mvertex_shader;                              // Vertex Shader name
    GLuint mfragment_shader;                            // Fragment Shader name
    GLuint mprogramm;                                   // The Shader Program    
    GLuint mvao;                                        // Vertex array object    
    GLuint mdp_vbo;                                     // Vertex buffer object
    GLint attribute_coord;                              // Attribute coordinate
    GLint uniform_tex;                                  // Uniform texture
    GLint uniform_color;                                // Unifor color
    const char* mvs_text;                               // Vertex shader text
    const char* mfs_text;                               // Fragment shader text
    FT_Library mft;                                     // The FreeType font
    FT_Face mface;                                      // The FreeType face
    std::vector<std::wstring> m_vecStringsToDraw;       // The strings to be drawn  
    GLfloat* m_glColor;                               // The color of the text
    
    // Point struct to draw the box on screen
    struct point
    {
        GLfloat x;
        GLfloat y;
        GLfloat s;
        GLfloat t;
    };
};

