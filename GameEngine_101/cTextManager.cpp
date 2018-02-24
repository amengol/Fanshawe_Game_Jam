#include "cTextManager.h"
#include <pugixml\pugixml.hpp>
#include <iostream>

cTextManager::cTextManager()
{
    this->m_glColor = new GLfloat[4]{0.0f, 0.0f, 0.0f, 1.0f};
    
    // Vertex shader text
    mvs_text =
        "#version 400\n"
        "attribute vec4 coord;"
        "varying vec2 texpos;"
        "void main () {"
        "	gl_Position = vec4(coord.xy, 0, 1);"
        "	texpos = coord.zw;"
        "}";

    // Fragmente shader text
    mfs_text =
        "#version 400\n"
        "varying vec2 texpos;"
        "uniform sampler2D tex;"
        "uniform vec4 color;"
        "void main () {"
        "	gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).r) * color;"
        "}";
}

cTextManager::~cTextManager()
{
    delete[] this->m_glColor;
}

bool cTextManager::init()
{
    if (init_gl() != GL_TRUE)
        return false;

    if (initfreetype() != GL_TRUE)
        return false;

    return true;
}

void cTextManager::draw(unsigned int width, unsigned int height)
{
    

    glUseProgram(mprogramm);
    glUniform4fv(uniform_color, 1, this->m_glColor);
    glBindVertexArray(mvao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);

    this->renderVecStrings(this->m_vecStringsToDraw, width, height);

    glBindVertexArray(0);
}

void cTextManager::setText(std::vector<std::wstring> vecText, glm::vec3 color)
{
    this->m_vecStringsToDraw = vecText;
    this->m_glColor[0] = color.x;
    this->m_glColor[1] = color.y;
    this->m_glColor[2] = color.z;
}

GLboolean cTextManager::init_gl()
{
    //create shaders
    mvertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(mvertex_shader, 1, &mvs_text, NULL);
    glCompileShader(mvertex_shader);

    mfragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(mfragment_shader, 1, &mfs_text, NULL);
    glCompileShader(mfragment_shader);

    mprogramm = glCreateProgram();
    glAttachShader(mprogramm, mvertex_shader);
    glAttachShader(mprogramm, mfragment_shader);

    glLinkProgram(mprogramm);

    //get vertex attribute/s id/s
    attribute_coord = glGetAttribLocation(mprogramm, "coord");
    uniform_tex = glGetUniformLocation(mprogramm, "tex");
    uniform_color = glGetUniformLocation(mprogramm, "color");

    if (attribute_coord == -1 || uniform_tex == -1 || uniform_color == -1)
    {
        fprintf(stderr, "unable to get attribute or uniform from shader\n");
        return GL_FALSE;
    }

    //generate and bind vbo 
    glGenBuffers(1, &mdp_vbo);

    //generate and bind vao
    glGenVertexArrays(1, &mvao);

    return GL_TRUE;
}

GLboolean cTextManager::initfreetype()
{
    if (FT_Init_FreeType(&mft))
    {
        fprintf(stderr, "unable to init free type\n");
        return GL_FALSE;
    }

    if (FT_New_Face(mft, "assets\\fonts\\ARIALUNI.TTF", 0, &mface))
    {
        fprintf(stderr, "unable to open font\n");
        return GL_FALSE;
    }

    

    //set font size
    FT_Set_Pixel_Sizes(mface, 0, 48);


    if (FT_Load_Char(mface, 'X', FT_LOAD_RENDER))
    {
        fprintf(stderr, "unable to load character\n");
        return GL_FALSE;
    }

    //FT_Select_Charmap(mface, ft_encoding_unicode);

    return GL_TRUE;
}

void cTextManager::renderVecStrings(std::vector<std::wstring> vecStrings, 
                                    unsigned int width, unsigned int height)
{
    float fontSize = 1.0f;
    float sx = fontSize / width;
    float sy = fontSize / height;
    GLfloat yoffset = 50.0f;
    //GLfloat xoffset = 8 * sx; // LEFT
    GLfloat xoffset = ((2 * width - 560) * sx) / fontSize; // Right

    for (std::vector<std::wstring>::iterator it = vecStrings.begin();
         it != vecStrings.end(); it++)
    {
        std::wstring tmp = *it;
        renderText(tmp.c_str(), -1 + xoffset, 1 - yoffset * sy, sx, sy);
        yoffset += 50.0f;
    }
}

void cTextManager::renderText(const wchar_t * text, float x, float y, float sx, float sy)
{
    const wchar_t *p;
    FT_GlyphSlot g = mface->glyph;

    GLuint tex;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(uniform_tex, 0);

    //this implementation requires 1 byte alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //clamp edges
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //setup vertex buffer object
    glEnableVertexAttribArray(attribute_coord);
    glBindBuffer(GL_ARRAY_BUFFER, mdp_vbo);
    glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

    //Loop all characters
    for (p = text; *p; p++)
    {
        //try to render the character
        if (FT_Load_Char(mface, *p, FT_LOAD_RENDER))
            continue;

        //upload the bitmap
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                     g->bitmap.buffer);

        //calculate vertex and texture coordinates
        float x2 = x + g->bitmap_left * sx;
        float y2 = -y - g->bitmap_top * sy;
        float w = g->bitmap.width * sx;
        float h = g->bitmap.rows * sy;

        point box[4] = {
        {x2, -y2, 0, 0},
        {x2 + w, -y2, 1, 0},
        {x2, -y2 - h, 0, 1},
        {x2 + w, -y2 - h, 1, 1}
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        //advance the cursor to the start of the next character
        x += (g->advance.x >> 6) * sx;
        y += (g->advance.y >> 6) * sy;

    }

    glDisableVertexAttribArray(attribute_coord);
    glDeleteTextures(1, &tex);
}
