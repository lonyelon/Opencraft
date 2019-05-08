#include "KeyConverter.hpp"

KeyConverter::KeyConverter(  ) {
    ConfigLoader * cf = new ConfigLoader( "./bin/keyboard.conf" );

    std::vector<std::string> keys = cf->getVariables(  );
    for ( int i = 0; i < keys.size(); i++ ) {
        KeyName kp;
        kp.name = keys[i];
        kp.key = cf->getInt(keys[i]);

        this->kn.push_back( kp );
    }

    printf("%s,%d\n", kn[0].name.c_str(), kn[0].key);
}
    
std::string KeyConverter::getKeyName( int key ) {
    for ( int i = 0; i < this->kn.size(); i++ ) {
        if ( this->kn[i].key == key ) {
            return this->kn[i].name;
        }
    }

    return "";
}

int KeyConverter::getKeyId( std::string name ) {
    for ( int i = 0; i < this->kn.size(); i++ ) {
        if ( name.compare( this->kn[i].name ) == 0 ) {
            return this->kn[i].key;
        }
    }

    return 0;
}

void KeyConverter::addKey( std::string name, int key ) {
    KeyName k;
    k.key = key;
    k.name = name;

    this->kn.push_back( k );
}