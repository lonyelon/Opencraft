#ifndef ENGINE_MODEL_MODEL_LOADER
#define ENGINE_MODEL_MODEL_LOADER

#include <string>
#include <memory>
#include "Model.hpp"

class ModelLoader {
private:
    std::string path;
public:
    ModelLoader();

    std::shared_ptr<Model> loadModel( std::string path );
};

#endif