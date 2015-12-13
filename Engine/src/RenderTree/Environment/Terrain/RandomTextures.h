#pragma once
#define NUMBER_OF_TEXTURE 4
#include <GL/glew.h>

class RandomTextures
{
public:
    RandomTextures();
    ~RandomTextures();

    GLuint Texture(int position);
protected:
    void CreateTexture(int position);
    GLuint textures_[4];
    static bool seeded_;
};

