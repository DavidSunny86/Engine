#pragma once

#include <GL/glew.h>
#include <glm/common.hpp>

class GLSLProgram;
class RandomTextures;
class DensityTexture;

class TerrainCube
{
public:
    TerrainCube(const glm::vec3& position, RandomTextures* noise);

    void Render();
    ~TerrainCube();
protected:
    void Initialize(RandomTextures* noise);
    void LoadCaseNumberPoly();
    void LoadEdgeConnectList();
    void GenerateStaticBuffers();

    bool FirstPass(DensityTexture* density);
    void SecondPass(DensityTexture* density);
    void ThirdPass(DensityTexture* density);
    void FourthPass(DensityTexture* density);
    void FifthPass(DensityTexture* density);



    void LiberateGPUMemory();
    void SaveVertexData();
    glm::vec3 position_;
    int firstPassnumberOfPrimitiveGenerated_;
    int secondPassnumberOfPrimitiveGenerated_;
    int thirdPassnumberOfPrimitiveGenerated_;
    int numberOfTrianges_;
    GLuint vbo_;
    GLuint vao_;
    GLuint ibo_;

    static GLuint caseNumPolys_;
    static GLuint edgeConnectList_;
    static GLuint firstPassPointVao_;
    static GLuint firstPassPointVbo_;

    static GLuint secondPassVao_;
    static GLuint thirdPassVao_;

    static GLuint firstPassOutputTbo_;
    static GLuint secondPassOutputTbo_;
    static GLuint vertexIdTexture_;
    static GLuint vertexIDframebuffer_;
    static GLuint finalOutputTbo_;
    static GLuint finalOutputIbo_;
    static bool constantBufferLoaded_;

    static GLSLProgram* firstPassProgram_;
    static GLSLProgram* secondPassProgram_;
    static GLSLProgram* thirdPassProgram_;
    static GLSLProgram* fourthPassProgram_;
    static GLSLProgram* fifthPassProgram_;
};

