/**
    cLocalization.h
    Purpose: loads localization files, controls and draws texts
    Disclaimer: Based on the in-class code from Professor Oscar Lara

    @author Jorge Amengol
    @date February 22th, 2018
*/
#pragma once
#include <string>
#include <map>
#include <vector>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "stb_image.h" 
#include <ft2build.h>
#include FT_FREETYPE_H

class cLocalization
{
public:
    cLocalization();
    ~cLocalization();

    // Loads a localization configuration from a XML
    void loadLanguageFromXml(std::string file);

    // Initiate all parameters
    bool init();

    // Draws localization text
    void draw(unsigned int width, unsigned int height);

private:

    // Creates the shaders and compile the shader program
    GLboolean init_gl();

    // Init FreeType stuff
    GLboolean initfreetype();

    // Renders the selected menu
    void renderSelectedMenu(unsigned int width, unsigned int height);

    // Render the FreeType text
    void renderText(const wchar_t *text, float x, float y, float sx, float sy);

    GLuint mvertex_shader;              // Vertex Shader name
    GLuint mfragment_shader;            // Fragment Shader name
    GLuint mprogramm;                   // The Shader Program    
    GLuint mvao;                        // Vertex array object    
    GLuint mdp_vbo;                     // Vertex buffer object
    GLint attribute_coord;              // Attribute coordinate
    GLint uniform_tex;                  // Uniform texture
    GLint uniform_color;                // Unifor color
    const char* mvs_text;               // Vertex shader text
    const char* mfs_text;               // Fragment shader text
    FT_Library mft;                     // The FreeType font
    FT_Face mface;                      // The FreeType face

    // Languages
    enum eSelectedLanguage
    {
        ENGLIH,
        FRENCH,
        SPANISH,
        INSTRUCTIONS
    };
    
    // Point struct to draw the box on screen
    struct point
    {
        GLfloat x;
        GLfloat y;
        GLfloat s;
        GLfloat t;
    };

    std::map<std::wstring, std::vector<std::wstring>> mapLanguages;   // Map languages to their IDs
    std::vector<std::wstring> m_vecMainMenu;
    eSelectedLanguage currentLanguage;  // Current selected language    
};

