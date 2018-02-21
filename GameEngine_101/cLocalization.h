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

class cLocalization
{
public:
    cLocalization();
    ~cLocalization();

    // Loads a localization configuration from a XML
    void loadLanguageFromXml(std::string file);

private:
    std::map<std::string, std::vector<std::string>> mapLanguages;   // Map languages to their IDs
};

