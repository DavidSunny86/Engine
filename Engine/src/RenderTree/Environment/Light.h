#pragma once
#include <glm/glm.hpp>

#include "OpenGL/GLSLProgram.h"
class Light
{
public:
    Light();
    ~Light();
    void TransformPosition(const glm::mat4& modelView);
    void Apply(glm::mat4& view, glm::mat4& projection);
    void GenerateDepthTexture();
    void UseFrameBuffer();
    glm::vec4 transformedPosition_;
    glm::vec4 position_;
    glm::vec4 ambientColor_;
    glm::vec4 diffuseColor_;
    glm::vec4 specularColor_;
    glm::mat4 dephtMVP_;
    glm::mat4 view_;
    glm::mat4 projection_;
    glm::mat4 dephtBiasMVP_;
    float power_;
    GLuint shadowMap_;
    GLuint framebuffer_;
};

