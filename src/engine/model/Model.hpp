#ifndef ENGINE_MODEL_MODEL
#define ENGINE_MODEL_MODEL

#include <vector>

class Model {
private:
    std::vector<float> vertex;
    std::vector<int> textureCoords;
    unsigned int vao = 0;

public:
    Model() = default;

    void setVertex(std::vector<float> vertex);

    void setTextureCoords(std::vector<int> textureCoords);

    std::vector<float> getVertex();

    std::vector<int> getTextureCoords();

    int genVao();

    int getVao();

    void draw();

    ~Model();
};

#endif