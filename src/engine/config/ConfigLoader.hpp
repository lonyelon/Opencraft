#ifndef ENGINE_CONFIG_CONFIGLOADER_HPP
#define ENGINE_CONFIG_CONFIGLOADER_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

typedef struct sP {
    std::string variable;
    std::string value;
} StringPair;

class ConfigLoader {
private:
    std::string path;
    std::ifstream file;
    std::vector<StringPair> data;
public:
    void loadConfigPair( std::string input, std::string *name, std::string *value );
    ConfigLoader( std::string path );
    std::string getString( std::string name );
    int getInt( std::string name );
};

#endif