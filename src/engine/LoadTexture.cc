#include "LoadTexture.hpp"

#include <iostream>
#include <string>
#include <memory>

#include <engine/Engine.hpp>
#include <GL/glu.h>
#include <game/Game.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

extern std::unique_ptr<Game> game;

GLuint loadTexture(const std::string path) {
    // load and create a texture
    // -------------------------
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D,
                  texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    switch (game->useMipmap) {
        case 0:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            break;

        case 1:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            break;

        case 2:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            break;
    }

    float aniso = 16.0f;
    glGetFloatv(0x84FF, &aniso);
    glTexParameterf(GL_TEXTURE_2D, 0x84FE, 16.0);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    std::string p = "bin/textures/" + path;
    unsigned char *data = stbi_load(p.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        if (game->useMipmap > 0) {
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data); //con mimap
        }

    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
}
