#ifndef _KEY_CONVERTER_HPP_
#define _KEY_CONVERTER_HPP_

#include <string>
#include <vector>

typedef struct kns {
    std::string name;
    int key;
} KeyName;

class KeyConverter {
private:
    std::vector<KeyName> kn;

public:
    KeyConverter(  ) {
        
    }
    
    std::string getKeyName( int key ) {
        for ( int i = 0; i < this->kn.size(); i++ ) {
            if ( this->kn[i].key == key ) {
                return this->kn[i].name;
            }
        }

        return "";
    }

    int getKeyId( std::string name ) {
        for ( int i = 0; i < this->kn.size(); i++ ) {
            if ( name.compare( this->kn[i].name ) == 0 ) {
                return this->kn[i].key;
            }
        }

        return 0;
    }

    void addKey( std::string name, int key ) {
        KeyName k;
        k.key = key;
        k.name = name;

        this->kn.push_back( k );
    }
};

#endif