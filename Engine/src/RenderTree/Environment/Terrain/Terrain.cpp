#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Terrain.h"
#include "TerrainCube.h"
#include "RandomTextures.h"
#include "RenderTree/Environment/Light.h"
#include "Manager/GLSLProgramManager.h"
#include "OpenGL/GLSLProgram.h"

Terrain::Terrain()
{
    textures_ = new RandomTextures();
    for (int i = -5; i < 5; i++)
    {
        for (int j = -5; j < 5; j++)
        {
            for (int k = -5; k < 51; k++)
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

void Terrain::Render(const glm::mat4& view, const glm::mat4& projection, Light* light)
{
    glCullFace(GL_FRONT);
    glBlendFunc(GL_ONE, GL_NONE);
    if(renderWireframe_)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    renderProgram_->Activate();
    glm::mat4 mvp = projection * view;
    glUniformMatrix4fv(renderProgram_->GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(renderProgram_->GetUniformLocation("MV"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix3fv(renderProgram_->GetUniformLocation("normalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverse(glm::transpose(view)))));
    glUniform4fv(renderProgram_->GetUniformLocation("lightPosition"), 1, glm::value_ptr(light->transformedPosition_));
    glUniform4fv(renderProgram_->GetUniformLocation("lightAmbientColor"), 1, glm::value_ptr(light->ambientColor_));
    glUniform4fv(renderProgram_->GetUniformLocation("lightDiffuseColor"), 1, glm::value_ptr(light->diffuseColor_));
    glUniform4fv(renderProgram_->GetUniformLocation("lightSpecularColor"), 1, glm::value_ptr(light->specularColor_));


    glUniform4fv(renderProgram_->GetUniformLocation("materialAmbient"), 1, glm::value_ptr(glm::vec4(0.2f)));
    glUniform4fv(renderProgram_->GetUniformLocation("materialDiffuse"), 1, glm::value_ptr(glm::vec4(0.6f)));
    glUniform4fv(renderProgram_->GetUniformLocation("materialSpecular"), 1, glm::value_ptr(glm::vec4(0.6f)));
    glUniform1f(renderProgram_->GetUniformLocation("shininess"), 1.0f);
    for (auto cube : cubes_)
        cube->Render();
    if(renderWireframe_)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}