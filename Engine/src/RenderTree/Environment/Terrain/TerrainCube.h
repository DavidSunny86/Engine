#pragma once

#include <GL/glew.h>
#include <glm/common.hpp>

class GLSLProgram;
class RandomTextures;
class TerrainCube
{
public:
    TerrainCube(const glm::vec3& position, RandomTextures* noise);

    void Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection);
    ~TerrainCube();
protected:
    void Initialize(RandomTextures* noise);
    void LoadCaseNumberPoly();
    void LoadEdgeConnectList();
    void GeneratePointVao();

    void CreateBuffers();
    void LiberateMemory();
    void ResizeBuffer();
    glm::vec3 position_;
    int numberOfTrianglesGenerated_;
    GLSLProgram* generateProgram_;
    GLSLProgram* renderProgram_;
    GLuint tbo_;
    GLuint vao_;

    static GLuint caseNumPolys_;
    static GLuint edgeConnectList_;
    static GLuint pointVao_;
    static GLuint pointVbo_;
    static bool constantBufferLoaded_;
};

