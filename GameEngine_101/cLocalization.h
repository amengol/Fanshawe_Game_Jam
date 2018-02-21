/**
    cLocalization.h
    Purpose: loads localization files, controls and draws texts

    @author Jorge Amengol
    @date February 21th, 2018
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
    void draw(GLFWwindow* window, unsigned int width, unsigned int height);

private:
    enum eSelectedLanguage
    {
        ENGLIH,
        FRENCH,
        SPANISH,
        INSTRUCTIONS
    };
    
    struct point
    {
        GLfloat x;
        GLfloat y;
        GLfloat s;
        GLfloat t;
    };


    eSelectedLanguage currentLanguage = eSelectedLanguage::INSTRUCTIONS;

    std::map<std::string, std::vector<std::string>> mapLanguages;   // Map languages to their IDs

    GLuint mvertex_shader, mfragment_shader, mprogramm;
    //vertex array object
    GLuint mvao;
    //vertex buffer object
    GLuint mdp_vbo;


    GLint attribute_coord;
    GLint uniform_tex;
    GLint uniform_color;

    const char* mvs_text =
        "#version 400\n"
        "attribute vec4 coord;"
        "varying vec2 texpos;"
        "void main () {"
        "	gl_Position = vec4(coord.xy, 0, 1);"
        "	texpos = coord.zw;"
        "}";

    const char* mfs_text =
        "#version 400\n"
        "varying vec2 texpos;"
        "uniform sampler2D tex;"
        "uniform vec4 color;"
        "void main () {"
        "	gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).r) * color;"
        "}";

    GLboolean init_gl();

    GLboolean initfreetype();

    void renderSelectedMenu(unsigned int width, unsigned int height);
    
    void renderText(const char *text, float x, float y, float sx, float sy);


    FT_Library mft;

    FT_Face mface;

};

