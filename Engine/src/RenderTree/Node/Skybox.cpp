#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GL/glew.h>
#include <IL\il.h>

#include "Skybox.h"
#include "Manager\GLSLProgramManager.h"
#include "OpenGL\GLSLProgram.h"

GLfloat points[] = {
    // Positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

Skybox::Skybox(AbstractNode* parent) : AbstractNode(parent)
{
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    LoadTextures();
}


Skybox::~Skybox()
{
}

void Skybox::Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane)
{
    GLSLProgram* skyboxProgram = GLSLProgramManager::Instance()->GetProgram("Skybox");
    glUseProgram(skyboxProgram->ID());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);
    glm::mat4 viewTemp = glm::mat4(glm::mat3(view));
    glm::mat4 MVP = projection * viewTemp * glm::mat4(1);
    glUniform1i(skyboxProgram->GetUniformLocation("Texture"), 0);
    glUniformMatrix4fv(skyboxProgram->GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Skybox::RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection)
{

}

void Skybox::RenderShadowMap(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection)
{

}

void Skybox::RenderReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane, glm::mat4 shadowModel)
{
    GLSLProgram* skyboxProgram = GLSLProgramManager::Instance()->GetProgram("Skybox");
    glUseProgram(skyboxProgram->ID());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);
    glm::mat4 viewTemp = glm::mat4(glm::mat3(view));
    glm::mat4 MVP = projection * viewTemp * glm::scale(glm::mat4(1), glm::vec3(-1, -1, 1));
    glUniform1i(skyboxProgram->GetUniformLocation("Texture"), 0);
    glUniformMatrix4fv(skyboxProgram->GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Skybox::LoadTextures()
{
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);
    std::string names[] = { "./Textures/Skybox/x_pos.png",
                            "./Textures/Skybox/y_pos.png",
                            "./Textures/Skybox/z_pos.png",
                            "./Textures/Skybox/x_neg.png",
                            "./Textures/Skybox/y_neg.png",
                            "./Textures/Skybox/z_neg.png" };
    GLenum target[] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };
    for (int i = 0; i < 6; i++)
    {
        ILuint imageId = ilGenImage();

        ilBindImage(imageId);
        std::wstring temp = std::wstring(names[i].begin(), names[i].end());
        ILboolean success = ilLoadImage(temp.c_str());
        if (!success)
        {
            std::cerr << "Error while loading the texture file: " << names[i] << std::endl;
        }

        int width, height, bytePerPixel;
        width = ilGetInteger(IL_IMAGE_WIDTH);
        height = ilGetInteger(IL_IMAGE_HEIGHT);
        bytePerPixel = ilGetInteger(IL_IMAGE_BPP);
        GLuint format;
        switch (bytePerPixel)
        {
        case 1:
            format = GL_R;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            format = GL_RGBA;
            break;
        }
        glTexImage2D(target[i], 0, format, width, height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
        ilDeleteImage(imageId);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
