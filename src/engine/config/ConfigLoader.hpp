#ifndef ENGINE_CONFIG_CONFIGLOADER_HPP
#define ENGINE_CONFIG_CONFIGLOADER_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>

class ConfigLoader {
private:
    std::string path;
    std::ifstream file;
    std::vector<std::tuple<std::string, std::string>> data;
public:
    ConfigLoader(std::string path);

    std::tuple<std::string, std::string> loadConfigPair(std::string input);

    std::string getString(std::string name);

    int getInt(std::string name);

    float getFloat(std::string name);

    std::vector<std::string> getVariables();
};

#endif