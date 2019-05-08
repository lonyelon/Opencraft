#ifndef _KEY_CONVERTER_HPP_
#define _KEY_CONVERTER_HPP_

#include <string>
#include <vector>

#include "../config/ConfigLoader.hpp"

typedef struct kns {
    std::string name;
    int key;
} KeyName;

class KeyConverter {
private:
    std::vector<KeyName> kn;

public:
    KeyConverter(  );
    
    std::string getKeyName( int key );
    int getKeyId( std::string name );
    void addKey( std::string name, int key );
};

#endif