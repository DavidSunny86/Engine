#include <cstdlib>
#include <time.h>

#include "RandomTextures.h"

bool RandomTextures::seeded_ = false;
RandomTextures::RandomTextures()
{
    glGenTextures(NUMBER_OF_TEXTURE, textures_);
    if (!seeded_)
    {
        srand(time(NULL));
        seeded_ = true;
    }
    for (int i = 0; i < NUMBER_OF_TEXTURE; i++)
    {
        CreateTexture(i);
    }
}


RandomTextures::~RandomTextures()
{
    glDeleteTextures(NUMBER_OF_TEXTURE, textures_);
}

GLuint RandomTextures::Texture(int position)
{
    return textures_[position];
}

void RandomTextures::CreateTexture(int position)
{
    const int numberOfVoxels = 16 * 16 * 16;
    GLfloat randomData[numberOfVoxels];
    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 16; ++j)
        {
            for (int k = 0; k < 16; k++)
            {
                randomData[i * 16 * 16 + j * 16 + k] = float(rand() * 2 - RAND_MAX) / RAND_MAX;
            }
        }
    }
    glBindTexture(GL_TEXTURE_3D,textures_[position]);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, 16, 16, 16, 0, GL_RED, GL_FLOAT, randomData);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
}
