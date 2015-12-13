#include <glm/mat4x4.hpp>

#include "Terrain.h"
#include "TerrainCube.h"
#include "RandomTextures.h"

Terrain::Terrain()
{
    textures_ = new RandomTextures();
    for (int i = -10; i < 10; i++)
    {
        for (int j = -10; j < 10; j++)
        {
            for (int k = -10; k < 10; k++)
            {
                cubes_.push_back(new TerrainCube(glm::vec3(i, j - 0.5f, k), textures_));
            }
        }
    }
    //cubes_.push_back(new TerrainCube(glm::vec3(0, 0.f, 0), textures_));
    //cubes_.push_back(new TerrainCube(glm::vec3(1.0, 0.0, 1.0),textures_));
}


Terrain::~Terrain()
{
    for (size_t i = 0; i < cubes_.size(); ++i)
    {
        delete cubes_[i];
    }
    delete textures_;
}

void Terrain::Render(const glm::mat4& view, const glm::mat4& projection)
{
    glDisable(GL_CULL_FACE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (auto cube : cubes_)
        cube->Render(glm::mat4(1.0),view, projection);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
