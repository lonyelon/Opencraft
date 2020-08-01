#include "ModelLoader.hpp"

#include <cstdio>

ModelLoader::ModelLoader() {
    this->path = "";
}

std::shared_ptr<Model> ModelLoader::loadModel(std::string path) {
    std::shared_ptr<Model> m;
    FILE *f;
    std::vector<float> vertex;
    std::vector<int> textureCoords;
    int size;
    float v0, v1, v2;
    int t0, t1;

    path = "./bin/models/" + path;

    m = std::make_shared<Model>();
    f = fopen(path.c_str(), "r");

    if (f == nullptr) {
        return nullptr;
    }

    fscanf(f, "V %d", &size);

    for (int i = 0; i < size; i++) {
        fscanf(f, "%f %f %f", &v0, &v1, &v2);
        vertex.push_back(v0);
        vertex.push_back(v1);
        vertex.push_back(v2);
    }

    for (int i = 0; i < size; i++) {
        fscanf(f, "%d %d", &t0, &t1);
        textureCoords.push_back(t0);
        textureCoords.push_back(t1);
    }

    m->setVertex(vertex);
    m->setTextureCoords(textureCoords);

    return m;
}