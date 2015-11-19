#include "Water.h"
#include <iostream>
#include "IL/il.h"
#include "Constant.h"
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Water::Water(AbstractNode* parent) : AbstractNode(parent)
{
    textureWidth_ = Constant::TextureWidth;
    textureHeight_ = Constant::TextureHeight;
    renderingWater_ = false;
    waterProgram_ = GLSLProgramManager::Instance()->GetProgram("Water");
    firstPassProgram_ = GLSLProgramManager::Instance()->GetProgram("FirstPass");
    CreateBuffers();
    LoadModel();
}


Water::~Water()
{
    glDeleteBuffers(1, &reflectionFbo_);
    glDeleteRenderbuffers(1, &reflectionDepthBuffer_);
    glDeleteTextures(1, &reflectionTexture_);
    glDeleteBuffers(1, &refractionFbo_);
    glDeleteRenderbuffers(1, &refractionDepthBuffer_);
    glDeleteTextures(1, &refractionTexture_);
}

void Water::Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane)
{
    if (parent_ != nullptr && !renderingWater_)
    {
        renderingWater_ = true;
        glBindFramebuffer(GL_FRAMEBUFFER, reflectionFbo_);
        glViewport(0, 0, textureWidth_, textureHeight_);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glEnable(GL_CLIP_DISTANCE0);
        glm::mat4 modelReflection = glm::mat4(1);
        ApplyReflectionTransformation(modelReflection);
        glm::vec4 clipPlane = glm::vec4(0, -1, 0, 0);
        parent_->RenderReflection(modelReflection, view, projection, environnement, clipPlane, glm::mat4(1));
        
        glBindFramebuffer(GL_FRAMEBUFFER, refractionFbo_);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        modelReflection = glm::mat4(1);
        clipPlane = glm::vec4(0, 1, 0, 0);
        parent_->RenderFirstPass(modelReflection, view, projection);
        parent_->Render(modelReflection, view, projection, environnement, clipPlane);
        glDisable(GL_CLIP_DISTANCE0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, Constant::ViewportWidth, Constant::ViewPortHeight);

        ApplyTransformation(model);
        RenderModel(model, view, projection, environnement);
        renderingWater_ = false;
    }
}

void Water::RenderReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane, glm::mat4 shadowModel)
{

}

void Water::RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection)
{
    if (!renderingWater_)
    {
        ApplyTransformation(model);
        RenderModelFirstPass(model, view, projection);
    }
}

void Water::Update(double deltaT)
{
}

void Water::ApplyReflectionTransformation(glm::mat4& modelReflection)
{
    modelReflection = glm::translate(modelReflection, position_);
    modelReflection = glm::scale(modelReflection, glm::vec3(1, -1, 1));
}


void Water::RenderModel(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, Environment* e, const glm::vec4& clipPlane /*= glm::vec4(0)*/)
{
    glUseProgram(waterProgram_->ID());
    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture_);
    glActiveTexture(GL_TEXTURE2);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, refractionTexture_);
    glUniform1i(waterProgram_->GetUniformLocation("reflectionTexture"), 1);
    glUniform1i(waterProgram_->GetUniformLocation("refractionTexture"), 2);
    glm::vec2 viewPort = glm::vec2(Constant::ViewportWidth, Constant::ViewPortHeight);
    glUniform2i(waterProgram_->GetUniformLocation("viewPort"), Constant::ViewportWidth, Constant::ViewPortHeight);
    glm::mat4 mv = v * m;
    glm::mat4 mvp = p * mv;
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(mv)));
    glUniformMatrix4fv(waterProgram_->GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(waterProgram_->GetUniformLocation("MV"), 1, GL_FALSE, glm::value_ptr(mv));
    glUniformMatrix4fv(waterProgram_->GetUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(m));
    glUniformMatrix3fv(waterProgram_->GetUniformLocation("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    for (auto light : e->lights_)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, light->shadowMap_);
        glUniform1i(waterProgram_->GetUniformLocation("shadowMap"), 0);
        glm::mat4 depthMVP = light->projection_ * light->view_ * m;
        depthMVP = light->dephtBiasMVP_ * depthMVP;
        glUniformMatrix4fv(waterProgram_->GetUniformLocation("depthMVP"), 1, GL_FALSE, glm::value_ptr(depthMVP));
        glUniform4fv(waterProgram_->GetUniformLocation("lightAmbientColor"), 1, glm::value_ptr(light->ambientColor_));
        glUniform4fv(waterProgram_->GetUniformLocation("lightDiffuseColor"), 1, glm::value_ptr(light->diffuseColor_));
        glUniform4fv(waterProgram_->GetUniformLocation("lightSpecularColor"), 1, glm::value_ptr(light->specularColor_));
        glUniform4fv(waterProgram_->GetUniformLocation("lightPosition"), 1, glm::value_ptr(light->transformedPosition_));
        glUniform1f(waterProgram_->GetUniformLocation("lightIntensity"), light->power_);

        material_->ApplyMaterial(waterProgram_);
        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, numberOfElement_, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    glDisable(GL_BLEND);
}

void Water::RenderModelFirstPass(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, const glm::vec4& clipPlane /*= glm::vec4(0)*/)
{
    glUseProgram(firstPassProgram_->ID());
    glm::mat4 mv = v * m;
    glm::mat4 mvp = p * mv;
    glUniformMatrix4fv(firstPassProgram_->GetUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(firstPassProgram_->GetUniformLocation("M"), 1, GL_FALSE, glm::value_ptr(m));
    glUniform4fv(firstPassProgram_->GetUniformLocation("clipPlane"), 1, glm::value_ptr(clipPlane));
    glDisable(GL_BLEND);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, numberOfElement_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Water::LoadModel()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("./Models/Water.dae",
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    if (!scene)
    {
        std::cerr << "Error loading model: Water" << std::endl;
        return;
    }
    material_ = new Material(scene->mMaterials[0]);
    auto mesh = scene->mMeshes[0];
    vbo_[VBOType::Position] = NULL;
    vbo_[VBOType::TextureCoord] = NULL;
    vbo_[VBOType::Normal] = NULL;
    vbo_[VBOType::Index] = NULL;
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    numberOfElement_ = mesh->mNumFaces * 3;

    if (mesh->HasPositions())
    {
        float* vertices = new float[mesh->mNumVertices * 3];
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            vertices[i * 3 + 0] = mesh->mVertices[i].x;
            vertices[i * 3 + 1] = mesh->mVertices[i].y;
            vertices[i * 3 + 2] = mesh->mVertices[i].z;
        }
        glGenBuffers(1, &vbo_[VBOType::Position]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_[VBOType::Position]);
        glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);
        delete[] vertices;
    }
    if (mesh->HasNormals())
    {
        float* normals = new float[mesh->mNumVertices * 3];
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            normals[3 * i + 0] = mesh->mNormals[i].x;
            normals[3 * i + 1] = mesh->mNormals[i].y;
            normals[3 * i + 2] = mesh->mNormals[i].z;
        }
        glGenBuffers(1, &vbo_[Normal]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_[Normal]);
        glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);
        delete[] normals;
    }
    unsigned int j = 0;
    while (mesh->HasTextureCoords(j))
    {
        float* coord = new float[mesh->mNumVertices * 2];
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            coord[i * 2 + 0] = mesh->mTextureCoords[j][i].x;
            coord[i * 2 + 1] = mesh->mTextureCoords[j][i].y;
        }

        glGenBuffers(1, &vbo_[TextureCoord]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_[TextureCoord]);
        glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), coord, GL_STATIC_DRAW);

        glVertexAttribPointer(j + 2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(j + 2);
        delete[] coord;
        ++j;
    }
    if (mesh->HasFaces())
    {
        GLuint* indices = new GLuint[mesh->mNumFaces * 3];
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
        {
            indices[3 * i + 0] = mesh->mFaces[i].mIndices[0];
            indices[3 * i + 1] = mesh->mFaces[i].mIndices[1];
            indices[3 * i + 2] = mesh->mFaces[i].mIndices[2];
        }
        glGenBuffers(1, &vbo_[Index]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[Index]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfElement_ * sizeof(GLuint), indices, GL_STATIC_DRAW);
        delete[] indices;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Water::CreateBuffers()
{
    glGenFramebuffers(1, &reflectionFbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, reflectionFbo_);

    glGenTextures(1, &reflectionTexture_);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth_, textureHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, reflectionTexture_, 0);
    
    glGenRenderbuffers(1, &reflectionDepthBuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, reflectionDepthBuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, textureWidth_, textureHeight_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, reflectionDepthBuffer_);


    GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw new std::exception("Water reflection fbo problem");
    
    glGenFramebuffers(1, &refractionFbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, refractionFbo_);

    glGenTextures(1, &refractionTexture_);
    glBindTexture(GL_TEXTURE_2D, refractionTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth_, textureHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, refractionTexture_, 0);

    glGenRenderbuffers(1, &refractionDepthBuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, refractionDepthBuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, textureWidth_, textureHeight_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, refractionDepthBuffer_);

    glDrawBuffers(1, buffers);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw new std::exception("Water refraction fbo problem");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
