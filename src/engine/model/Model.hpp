#ifndef ENGINE_MODEL_MODEL
#define ENGINE_MODEL_MODEL

#include <vector>

class Model {
private:
    std::vector<double> vertex;
    std::vector<int> textureCoords;
public:
    Model();

    void setVertex( std::vector<double> vertex );
    void setTextureCoords( std::vector<int> textureCoords );
    std::vector<double> getVertex();
    std::vector<int> getTextureCoords();
};

#endif