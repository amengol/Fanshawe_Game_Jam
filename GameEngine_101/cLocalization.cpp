#include "cLocalization.h"
#include <pugixml\pugixml.hpp>



cLocalization::cLocalization()
{
}


cLocalization::~cLocalization()
{
}

void cLocalization::loadLanguageFromXml(std::string file)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(file.c_str());

    pugi::xml_node_iterator it = doc.children().begin();
    for (it; it != doc.children().end(); it++)
    {
        pugi::xml_node_iterator it2 = it->children().begin();
        for (it2; it2 != it->children().end(); it2++)
        {
            //fprintf(stdout, "Node name: %s\n", it2->name());
            pugi::xml_node_iterator it3 = it2->children().begin();
            for (it3; it3 != it2->children().end(); it3++)
            {

                this->mapLanguages[it2->first_attribute().value()].push_back(it3->child_value());
            }
        }
    }
}

bool cLocalization::init()
{
    if (init_gl() != GL_TRUE)
        return false;

    if (initfreetype() != GL_TRUE)
        return false;

    return true;
}

void cLocalization::draw()
{
    GLfloat black[4] = {0, 0, 0, 1};
    GLfloat red[4] = {1, 0, 0, 1};
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

    if (FT_New_Face(mft, "assets\\fonts\\FreeSans.ttf", 0, &mface))
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


    return GL_TRUE;
}
