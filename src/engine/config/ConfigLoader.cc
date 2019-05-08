#include "ConfigLoader.hpp"

#include <string>
#include <exception>

void ConfigLoader::loadConfigPair( std::string input, std::string *name, std::string *value ) {
    int i;
        
    *name = "";
    *value = "";

    for ( i = 0; i < input.size(  ) && input[i] != '='; i++  ) {
        *name += input[i];
    }

    for ( i++ ; i < input.size(  ); i++  ) {
        *value += input[i];
    }
}

ConfigLoader::ConfigLoader( std::string path ) {
    this->path = path;
    this->file.open(path);

    while ( !file.eof(  ) ) {
        std::string input;
        StringPair sp;

        getline( this->file, input );
        this->loadConfigPair( input, &sp.variable, &sp.value );
        this->data.push_back( sp );
    }
}

std::string ConfigLoader::getString( std::string name ) {
    for ( int i = 0; i < this->data.size(  ); i++ ) {
        if ( this->data[i].variable.compare( name ) == 0 ) {
            return this->data[i].value;
        }
    }
    return "";
}

int ConfigLoader::getInt( std::string name ) {
    std::string result = this->getString( name );

    try {
        return std::stof( result.c_str() );
    } catch ( std::exception e ) {
        return 0;
    }
}

float ConfigLoader::getFloat( std::string name ) {
    std::string result = this->getString( name );

    try {
        return atof( result.c_str() );
    } catch ( std::exception e ) {
        return 0.0f;
    }
}

std::vector<std::string> ConfigLoader::getVariables(  ) {
    std::vector<std::string> result;
    for ( int i = 0; i < this->data.size(); i++ ) {
        result.push_back( this->data[i].variable );
    }
    return result;
}