#ifndef ENGINE_MODEL_MODEL_LOADER
#define ENGINE_MODEL_MODEL_LOADER

#include <string>
#include "Model.hpp"

class ModelLoader {
private:
    std::string path;
public:
    ModelLoader();

    Model *loadModel( std::string path );
};

#endif