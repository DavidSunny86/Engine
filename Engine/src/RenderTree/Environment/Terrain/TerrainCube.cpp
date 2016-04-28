#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <iostream>

#include "TerrainCube.h"
#include "DensityTexture.h"
#include "RandomTextures.h"
#include "Manager/GLSLProgramManager.h"
#include "OpenGL/GLSLProgram.h"
#include "Constant.h"

bool TerrainCube::constantBufferLoaded_ = false;
GLuint TerrainCube::caseNumPolys_ = 0;
GLuint TerrainCube::edgeConnectList_ = 0;
GLuint TerrainCube::firstPassPointVao_ = 0;
GLuint TerrainCube::firstPassPointVbo_ = 0;
GLuint TerrainCube::finalOutputTbo_ = 0;
GLuint TerrainCube::firstPassOutputTbo_ = 0;
GLuint TerrainCube::secondPassOutputTbo_ = 0;
GLuint TerrainCube::vertexIdTexture_ = 0;
GLuint TerrainCube::secondPassVao_ = 0;
GLuint TerrainCube::thirdPassVao_ = 0;
GLuint TerrainCube::vertexIDframebuffer_ = 0;
GLuint TerrainCube::finalOutputIbo_ = 0;

GLSLProgram* TerrainCube::firstPassProgram_ = nullptr;
GLSLProgram* TerrainCube::secondPassProgram_ = nullptr;
GLSLProgram* TerrainCube::thirdPassProgram_ = nullptr;
GLSLProgram* TerrainCube::fourthPassProgram_ = nullptr;
GLSLProgram* TerrainCube::fifthPassProgram_ = nullptr;

TerrainCube::TerrainCube(const glm::vec3& position, RandomTextures* noise)
    : position_(position)
    , firstPassnumberOfPrimitiveGenerated_(0)
    , secondPassnumberOfPrimitiveGenerated_(0)
    , thirdPassnumberOfPrimitiveGenerated_(0)
{
    if (!constantBufferLoaded_)
    {
        LoadCaseNumberPoly();
        LoadEdgeConnectList();
        GenerateStaticBuffers();
        firstPassProgram_ = GLSLProgramManager::Instance()->GetProgram("FirstPassGenerateTerrain");
        secondPassProgram_ = GLSLProgramManager::Instance()->GetProgram("SecondPassGenerateTerrain");
        thirdPassProgram_ = GLSLProgramManager::Instance()->GetProgram("ThirdPassGenerateTerrain");
        fourthPassProgram_ = GLSLProgramManager::Instance()->GetProgram("FourthPassGenerateTerrain");
        fifthPassProgram_ = GLSLProgramManager::Instance()->GetProgram("FifthPassGenerateTerrain");
        constantBufferLoaded_ = true;
    }
    Initialize(noise);
}

void TerrainCube::Render()
{
    if (numberOfTrianges_ != 0)
    {
        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, numberOfTrianges_ * 3, GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
    }
}

TerrainCube::~TerrainCube()
{
    if (thirdPassnumberOfPrimitiveGenerated_ != 0)
    {
        LiberateGPUMemory();
    }

}

void TerrainCube::Initialize(RandomTextures* noise)
{
    DensityTexture* texture = new DensityTexture(position_, noise);
    if (FirstPass(texture))
    {
        SecondPass(texture);
        ThirdPass(texture);
        FourthPass(texture);
        FifthPass(texture);
        SaveVertexData();
    }
    delete texture;
}

void TerrainCube::LoadCaseNumberPoly()
{
    const int numberOfPossibleCase = 256;
    int caseToNumberPoly[numberOfPossibleCase];
    std::ifstream file;
    file.open(".\\Terrain\\case_to_numPolys.txt");
    if (file.is_open())
    {
        int i = 0;
        std::string number;
        while (i < 256)
        {
            file >> number;
            caseToNumberPoly[i++] = std::stoi(number);
        }
        file.close();
        glGenBuffers(1, &caseNumPolys_);
        glBindBuffer(GL_UNIFORM_BUFFER, caseNumPolys_);
        glBufferData(GL_UNIFORM_BUFFER, numberOfPossibleCase * sizeof(int), caseToNumberPoly, GL_STATIC_DRAW);
    }
    else
    {
        throw new std::exception("Incapable d'ouvrir case_to_numPolys.txt");
    }
}

void TerrainCube::LoadEdgeConnectList()
{
    const int numberOfPossibleCase = 256;
    const int maxNumberOfTriangle = 5;
    const int numberOfVerticesPerTriangle = 3; 
    int edgeConnectList[numberOfPossibleCase * numberOfVerticesPerTriangle * maxNumberOfTriangle];
    std::ifstream file;
    file.open(".\\Terrain\\edge_connect_list.txt");
    if (file.is_open())
    {
        int i = 0;
        while (i < 256)
        {
            char line[1024];
            file.getline(line, 1024);
            int index = 0;
            int j = 0;
            std::string number;
            while (line[index] != '\0')
            {
                if (line[index] != ' ' && line[index] != ',')
                {
                    number += line[index];
                }
                else if (line[index] == ',')
                {
                    index++;
                }
                else if(number != "")
                {
                    edgeConnectList[i * 5 * 3 + j] = std::stoi(number);
                    j++;
                    number.clear();
                }
                index++;
            }
            i++;
        }
        file.close();
        glGenBuffers(1, &edgeConnectList_);
        glBindBuffer(GL_UNIFORM_BUFFER, edgeConnectList_);
        glBufferData(GL_UNIFORM_BUFFER, numberOfPossibleCase * numberOfVerticesPerTriangle * maxNumberOfTriangle * sizeof(int), edgeConnectList, GL_STATIC_DRAW);
    }
    else
    {
        throw new std::exception("Incapable d'ouvrir edge_connect_list.txt");
    }
}

void TerrainCube::GenerateStaticBuffers()
{
    int numberOfPoint = Constant::numberOfVoxelPerTerrainCube + 1;

    GLint* vertices = new GLint[numberOfPoint * numberOfPoint * numberOfPoint * 3];
    int index = 0;
    for (int i = 0; i < numberOfPoint; i++)
    {
        for (int j = 0; j < numberOfPoint; j++)
        {
            for (int k = 0; k < numberOfPoint; k++)
            {
                vertices[index + 0] = i;
                vertices[index + 1] = j;
                vertices[index + 2] = k;
                index += 3;
            }
        }
    }
    glGenVertexArrays(1, &firstPassPointVao_);
    glBindVertexArray(firstPassPointVao_);
    glGenBuffers(1, &firstPassPointVbo_);
    glBindBuffer(GL_ARRAY_BUFFER, firstPassPointVbo_);
    glBufferData(GL_ARRAY_BUFFER, 3 * numberOfPoint * numberOfPoint * numberOfPoint * sizeof(GLint), vertices, GL_STATIC_DRAW);
    glVertexAttribIPointer(0, 3, GL_INT, 0, NULL);
    glEnableVertexAttribArray(0);
    delete[] vertices;
    glBindVertexArray(0);

    glGenBuffers(1, &finalOutputTbo_);
    glBindBuffer(GL_ARRAY_BUFFER, finalOutputTbo_);
    glBufferData(GL_ARRAY_BUFFER, 5 * 8 * numberOfPoint * numberOfPoint * numberOfPoint * sizeof(float), nullptr, GL_STREAM_DRAW);

    glGenVertexArrays(1, &secondPassVao_);
    glBindVertexArray(secondPassVao_);
    glGenBuffers(1, &firstPassOutputTbo_);
    glBindBuffer(GL_ARRAY_BUFFER, firstPassOutputTbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * numberOfPoint * numberOfPoint * numberOfPoint, nullptr, GL_STREAM_DRAW);
    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &thirdPassVao_);
    glBindVertexArray(thirdPassVao_);
    glGenBuffers(1, &secondPassOutputTbo_);
    glBindBuffer(GL_ARRAY_BUFFER, secondPassOutputTbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * 3 * numberOfPoint * numberOfPoint * numberOfPoint, nullptr, GL_STREAM_DRAW);
    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    glGenBuffers(1, &finalOutputIbo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, finalOutputIbo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * numberOfPoint * numberOfPoint * numberOfPoint, nullptr, GL_STREAM_DRAW);

    glGenFramebuffers(1, &vertexIDframebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, vertexIDframebuffer_);

    glGenTextures(1, &vertexIdTexture_);
    glBindTexture(GL_TEXTURE_3D, vertexIdTexture_);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R32UI, numberOfPoint * 3, numberOfPoint, numberOfPoint, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, vertexIdTexture_, 0);
    GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool TerrainCube::FirstPass(DensityTexture* density)
{
    firstPassProgram_->Activate();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, density->ID());
    glUniform1i(firstPassProgram_->GetUniformLocation("densityTexture"), 0);
    glUniform1i(firstPassProgram_->GetUniformLocation("numberOfVoxel"), Constant::numberOfVoxelPerTerrainCube);
    glEnable(GL_RASTERIZER_DISCARD);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, firstPassOutputTbo_);
    glBeginTransformFeedback(GL_POINTS);
    GLuint query;
    glGenQueries(1, &query);
    glBeginQuery(GL_PRIMITIVES_GENERATED, query);
    int numberOfPoint = Constant::numberOfVoxelPerTerrainCube + 1;
    glBindVertexArray(firstPassPointVao_);
    glDrawArrays(GL_POINTS, 0, numberOfPoint * numberOfPoint * numberOfPoint);
    glBindVertexArray(0);
    glEndQuery(GL_PRIMITIVES_GENERATED);
    glGetQueryObjectiv(query, GL_QUERY_RESULT, &firstPassnumberOfPrimitiveGenerated_);
    glEndTransformFeedback();
    glDisable(GL_RASTERIZER_DISCARD);
    return firstPassnumberOfPrimitiveGenerated_ != 0;
}

void TerrainCube::SecondPass(DensityTexture* density)
{
    secondPassProgram_->Activate();
    glEnable(GL_RASTERIZER_DISCARD);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, secondPassOutputTbo_);
    glBeginTransformFeedback(GL_POINTS);
    GLuint query;
    glGenQueries(1, &query);
    glBeginQuery(GL_PRIMITIVES_GENERATED, query);
    glBindVertexArray(secondPassVao_);
    glDrawArrays(GL_POINTS, 0, firstPassnumberOfPrimitiveGenerated_);
    glBindVertexArray(0);
    glEndQuery(GL_PRIMITIVES_GENERATED);
    glGetQueryObjectiv(query, GL_QUERY_RESULT, &secondPassnumberOfPrimitiveGenerated_);
    glEndTransformFeedback();
    glDisable(GL_RASTERIZER_DISCARD);
}

void TerrainCube::ThirdPass(DensityTexture* density)
{
    thirdPassProgram_->Activate();
    glUniform1i(thirdPassProgram_->GetUniformLocation("densityTexture"), 0);
    glUniform1i(thirdPassProgram_->GetUniformLocation("numberOfVoxel"), Constant::numberOfVoxelPerTerrainCube);
    glUniform4fv(thirdPassProgram_->GetUniformLocation("cubePosition"), 1, glm::value_ptr(glm::vec4(position_,0.0)));
    glEnable(GL_RASTERIZER_DISCARD);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, finalOutputTbo_);
    glBeginTransformFeedback(GL_POINTS);
    GLuint query;
    glGenQueries(1, &query);
    glBeginQuery(GL_PRIMITIVES_GENERATED, query);
    glBindVertexArray(thirdPassVao_);
    glDrawArrays(GL_POINTS, 0, secondPassnumberOfPrimitiveGenerated_);
    glBindVertexArray(0);
    glEndQuery(GL_PRIMITIVES_GENERATED);
    glGetQueryObjectiv(query, GL_QUERY_RESULT, &thirdPassnumberOfPrimitiveGenerated_);
    glEndTransformFeedback();
    glDisable(GL_RASTERIZER_DISCARD);
}

void TerrainCube::FourthPass(DensityTexture* density)
{
    glBindFramebuffer(GL_FRAMEBUFFER,vertexIDframebuffer_);
    glViewport(0, 0, (Constant::numberOfVoxelPerTerrainCube + 1) * 3, (Constant::numberOfVoxelPerTerrainCube + 1));
    glClear(GL_COLOR_BUFFER_BIT);
    fourthPassProgram_->Activate();
    glUniform1i(fourthPassProgram_->GetUniformLocation("numberOfVoxel"), Constant::numberOfVoxelPerTerrainCube + 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(thirdPassVao_);
    glDrawArrays(GL_POINTS, 0, secondPassnumberOfPrimitiveGenerated_);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Constant::ViewportWidth, Constant::ViewPortHeight);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glFinish();
}

void TerrainCube::FifthPass(DensityTexture* density)
{
    fifthPassProgram_->Activate();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, vertexIdTexture_);
    glUniform1i(fifthPassProgram_->GetUniformLocation("vertexIDTexure"), 0);
    glUniform1i(fifthPassProgram_->GetUniformLocation("numberOfVoxel"), Constant::numberOfVoxelPerTerrainCube);
    GLuint uniformLocation = glGetUniformBlockIndex(fifthPassProgram_->ID(), "case_to_numberPoly_t");
    glUniformBlockBinding(fifthPassProgram_->ID(), uniformLocation, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, caseNumPolys_);
    uniformLocation = glGetUniformBlockIndex(fifthPassProgram_->ID(), "edge_connect_list_t");
    glUniformBlockBinding(fifthPassProgram_->ID(), uniformLocation, 1);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, edgeConnectList_);
    GLuint query;
    glGenQueries(1, &query);
    glBeginQuery(GL_PRIMITIVES_GENERATED, query);
    glEnable(GL_RASTERIZER_DISCARD);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, finalOutputIbo_);
    glBeginTransformFeedback(GL_TRIANGLES);
    glBindVertexArray(secondPassVao_);
    glDrawArrays(GL_POINTS, 0, firstPassnumberOfPrimitiveGenerated_);
    glBindVertexArray(0);
    glEndQuery(GL_PRIMITIVES_GENERATED);
    glGetQueryObjectiv(query, GL_QUERY_RESULT, &numberOfTrianges_);
    glEndTransformFeedback();
}

void TerrainCube::LiberateGPUMemory()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

void TerrainCube::SaveVertexData()
{
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_COPY_READ_BUFFER, finalOutputTbo_);
    glBindBuffer(GL_COPY_WRITE_BUFFER, vbo_);
    glBufferData(GL_COPY_WRITE_BUFFER, thirdPassnumberOfPrimitiveGenerated_ * 8 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, NULL, NULL, thirdPassnumberOfPrimitiveGenerated_ * 8 * sizeof(GLfloat));
    glGenBuffers(1, &ibo_);
    glBindBuffer(GL_COPY_READ_BUFFER, finalOutputIbo_);
    glBindBuffer(GL_COPY_WRITE_BUFFER, ibo_);
    glBufferData(GL_COPY_WRITE_BUFFER, numberOfTrianges_ * 3 * sizeof(GLuint), NULL, GL_STATIC_DRAW);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, NULL, NULL, numberOfTrianges_ * 3 * sizeof(GLuint));


    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBindVertexArray(0);
}
