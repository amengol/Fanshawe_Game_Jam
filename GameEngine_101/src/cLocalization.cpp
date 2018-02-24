#include "cLocalization.h"
#include <pugixml\pugixml.hpp>
#include <iostream>



cLocalization::cLocalization()
{
    m_currentLanguage = eSelectedLanguage::ENGLISH;
    m_state = MAIN;

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


cLocalization::~cLocalization()
{
}

void cLocalization::loadLanguageFromXml(std::string file)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(file.c_str());

    // Loads the main menu
    pugi::xml_node mainMenu = doc.child(L"Localization").child(L"LanguangeMenu");
    pugi::xml_node_iterator itMainMenu = mainMenu.children().begin();
    for (itMainMenu; itMainMenu != mainMenu.children().end(); itMainMenu++)
    {
        this->m_vecMainMenu.push_back(itMainMenu->child(L"Description").child_value());
        this->m_vecStringsToDraw.push_back(itMainMenu->child(L"Description").child_value());
    }

    // Loads the languages    
    pugi::xml_node_iterator itLanguages = doc.child(L"Localization").children().begin();
    for (; itLanguages != doc.child(L"Localization").children().end(); itLanguages++)
    {
        std::wstring ws = itLanguages->first_attribute().value();
        if (ws == L"IN")
            continue;

        sLanguageStrings theLanguage;
        pugi::xml_node_iterator itLangItems = itLanguages->children().begin();
        for (; itLangItems != itLanguages->children().end(); itLangItems++)
        {
            theLanguage.langID = itLanguages->first_attribute().value();
            std::wstring itemName = itLangItems->name();
            if (itemName == L"L1")
            {
                theLanguage.languageMenu.push_back(itLangItems->child(L"Description").first_child().value());
            }
            
            if (itemName == L"L2")
            {
                theLanguage.languageInstructions.push_back(itLangItems->first_child().value());
            }
        }

        m_vecLanguageStrings.push_back(theLanguage);
    }

    int a = 0;
}

bool cLocalization::init()
{
    if (init_gl() != GL_TRUE)
        return false;

    if (initfreetype() != GL_TRUE)
        return false;

    return true;
}

void cLocalization::draw(unsigned int width, unsigned int height)
{
    GLfloat black[4] = {0, 0, 0, 1};
    GLfloat red[4] = {1, 0, 0, 1};

    glUseProgram(mprogramm);


    glUniform4fv(uniform_color, 1, black);


    glBindVertexArray(mvao);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);

    this->renderSelectedMenu(width, height);

    glBindVertexArray(0);
}

void cLocalization::setMenu(eSelectedLanguage selLang, bool instr)
{
    switch (selLang)
    {
    case ENGLISH:
    {
        for (size_t i = 0; i < m_vecLanguageStrings.size(); i++)
        {
            if (m_vecLanguageStrings[i].langID == L"EN")
            {
                sLanguageStrings lang = m_vecLanguageStrings[i];
                
                if (instr)
                {
                    m_vecStringsToDraw = lang.languageInstructions;
                    m_state = INSTRUCTIONS;
                }
                else
                {
                    m_vecStringsToDraw = lang.languageMenu;
                    m_state = LANGUAGE;
                }
                m_currentLanguage = selLang;
                return;
            }
        }
    }
        break;
    case FRENCH:
    {
        for (size_t i = 0; i < m_vecLanguageStrings.size(); i++)
        {
            if (m_vecLanguageStrings[i].langID == L"FR")
            {
                sLanguageStrings lang = m_vecLanguageStrings[i];

                if (instr)
                {
                    m_vecStringsToDraw = lang.languageInstructions;
                    m_state = INSTRUCTIONS;
                }
                else
                {
                    m_vecStringsToDraw = lang.languageMenu;
                    m_state = LANGUAGE;
                }
                m_currentLanguage = selLang;
                return;
            }
        }
    }
        break;
    case SPANISH:
    {
        for (size_t i = 0; i < m_vecLanguageStrings.size(); i++)
        {
            if (m_vecLanguageStrings[i].langID == L"SP")
            {
                sLanguageStrings lang = m_vecLanguageStrings[i];

                if (instr)
                {
                    m_vecStringsToDraw = lang.languageInstructions;
                    m_state = INSTRUCTIONS;
                }
                else
                {
                    m_vecStringsToDraw = lang.languageMenu;
                    m_state = LANGUAGE;
                }
                m_currentLanguage = selLang;
                return;
            }
        }
    }
        break;
    case PORTUGUESE:
    {
        for (size_t i = 0; i < m_vecLanguageStrings.size(); i++)
        {
            if (m_vecLanguageStrings[i].langID == L"PTBR")
            {
                sLanguageStrings lang = m_vecLanguageStrings[i];

                if (instr)
                {
                    m_vecStringsToDraw = lang.languageInstructions;
                    m_state = INSTRUCTIONS;
                }
                else
                {
                    m_vecStringsToDraw = lang.languageMenu;
                    m_state = LANGUAGE;
                }
                m_currentLanguage = selLang;
                return;
            }
        }
    }
        break;
    case CHINESE:
    {
        for (size_t i = 0; i < m_vecLanguageStrings.size(); i++)
        {
            if (m_vecLanguageStrings[i].langID == L"CH")
            {
                sLanguageStrings lang = m_vecLanguageStrings[i];

                if (instr)
                {
                    m_vecStringsToDraw = lang.languageInstructions;
                    m_state = INSTRUCTIONS;
                }
                else
                {
                    m_vecStringsToDraw = lang.languageMenu;
                    m_state = LANGUAGE;
                }
                m_currentLanguage = selLang;
                return;
            }
        }
    }
        break;
    default:
        break;
    }
}

void cLocalization::setMainMenu()
{
    m_vecStringsToDraw = m_vecMainMenu;
    m_state = MAIN;
}

void cLocalization::dismiss()
{
    m_vecStringsToDraw.clear();
    m_state = DISMISS;
}

GLboolean cLocalization::init_gl()
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

GLboolean cLocalization::initfreetype()
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

void cLocalization::renderSelectedMenu(unsigned int width, unsigned int height)
{
    float sx = 1.0f / width;
    float sy = 1.0f / height;
    GLfloat yoffset = 50.0f;
    GLfloat xoffset = 8 * sx;

    for (std::vector<std::wstring>::iterator it = m_vecStringsToDraw.begin();
         it != m_vecStringsToDraw.end(); it++)
    {
        std::wstring tmp = *it;
        renderText(tmp.c_str(), -1 + xoffset, 1 - yoffset * sy, sx, sy);
        yoffset += 50.0f;
    }
}

void cLocalization::renderText(const wchar_t * text, float x, float y, float sx, float sy)
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
