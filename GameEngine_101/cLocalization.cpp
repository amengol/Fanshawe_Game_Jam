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
