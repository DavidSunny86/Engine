#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Terrain.h"
#include "TerrainCube.h"
#include "RandomTextures.h"
#include "Manager/GLSLProgramManager.h"
#include "OpenGL/GLSLProgram.h"

Terrain::Terrain()
{
    textures_ = new RandomTextures();
    for (int i = -32; i < 32; i++)
    {
        for (int j = -8; j < 8; j++)
        {
            for (int k = -32; k < 32; k++)
            {
                cubes_.push_back(new TerrainCube(glm::vec3(i, j - 0.5f, k), textures_));
            }
        }
    }
    renderProgram_ = GLSLProgramManager::Instance()->GetProgram("RenderTerrain");
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
    glBlendFunc(GL_ONE, GL_NONE);
    renderProgram_->Activate();
    glm::mat4 mvp = projection * view;
    glUniformMatrix4fv(renderProgram_->GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    for (auto cube : cubes_)
        cube->Render();
}