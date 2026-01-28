#include "ModelLoader.hpp"

#include <cstdio>

ModelLoader::ModelLoader() {
    this->path = "";
}

std::shared_ptr<Model> ModelLoader::loadModel(std::string path) {
    path = "./bin/models/" + path;
    FILE *f = fopen(path.c_str(), "r");
    if (f == nullptr)
        return nullptr;

    std::shared_ptr<Model> m = std::make_shared<Model>();

    int size;
    fscanf(f, "V %d", &size);

    std::vector<float> vertex;
    float v0, v1, v2;
    for (int i = 0; i < size; i++) {
        fscanf(f, "%f %f %f", &v0, &v1, &v2);
        vertex.push_back(v0);
        vertex.push_back(v1);
        vertex.push_back(v2);
    }
    m->setVertex(vertex);

    std::vector<int> textureCoords;
    int t0, t1;
    for (int i = 0; i < size; i++) {
        fscanf(f, "%d %d", &t0, &t1);
        textureCoords.push_back(t0);
        textureCoords.push_back(t1);
    }
    m->setTextureCoords(textureCoords);

    return m;
}
