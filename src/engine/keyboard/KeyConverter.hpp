#ifndef _KEY_CONVERTER_HPP_
#define _KEY_CONVERTER_HPP_

#include "KeyType.hpp"

class KeyConverter {
private:
public:
    KeyConverter ();
    KeyType numToCode(int num);
};

#endif