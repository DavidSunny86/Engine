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
GLuint TerrainCube::pointVao_ = 0;
GLuint TerrainCube::pointVbo_ = 0;
GLuint TerrainCube::tbo_ = 0;

TerrainCube::TerrainCube(const glm::vec3& position, RandomTextures* noise)
    : position_(position)
    , numberOfTrianglesGenerated_(0)
{
    if (!constantBufferLoaded_)
    {
        LoadCaseNumberPoly();
        LoadEdgeConnectList();
        GenerateStaticBuffers();
        constantBufferLoaded_ = true;
    }
    generateProgram_ = GLSLProgramManager::Instance()->GetProgram("GenerateTerrain");

    Initialize(noise);
}

void TerrainCube::Render()
{
    if (numberOfTrianglesGenerated_ != 0)
    {
        glBindVertexArray(vao_);
        glDrawArrays(GL_TRIANGLES, 0, numberOfTrianglesGenerated_ * 3);
    }
}

TerrainCube::~TerrainCube()
{
    if (numberOfTrianglesGenerated_ != 0)
    {
        LiberateGPUMemory();
    }

}

void TerrainCube::Initialize(RandomTextures* noise)
{
    DensityTexture* texture = new DensityTexture(position_, noise);
    generateProgram_->Activate();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, texture->ID());
    glUniform1i(generateProgram_->GetUniformLocation("densityTexture"), 0);
    glUniform1i(generateProgram_->GetUniformLocation("numberOfVoxel"), Constant::numberOfVoxelPerTerrainCube);
    glUniform4fv(generateProgram_->GetUniformLocation("cubePosition"),1, glm::value_ptr(glm::vec4(position_.x,position_.y,position_.z,0.0)));
    
    GLuint uniformLocation = glGetUniformBlockIndex(generateProgram_->ID(), "case_to_numberPoly_t");
    glUniformBlockBinding(generateProgram_->ID(), uniformLocation, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, caseNumPolys_);
    
    uniformLocation = glGetUniformBlockIndex(generateProgram_->ID(), "edge_connect_list_t");
    glUniformBlockBinding(generateProgram_->ID(), uniformLocation, 1);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, edgeConnectList_);

    glBindVertexArray(pointVao_);
    glEnable(GL_RASTERIZER_DISCARD);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo_);
    glBeginTransformFeedback(GL_TRIANGLES);
    GLuint query;
    glGenQueries(1, &query);
    glBeginQuery(GL_PRIMITIVES_GENERATED, query);
    int numberOfPoint = Constant::numberOfVoxelPerTerrainCube + 1;
    glDrawArrays(GL_POINTS, 0, numberOfPoint * numberOfPoint * numberOfPoint);
    glEndQuery(GL_PRIMITIVES_GENERATED);
    glGetQueryObjectiv(query, GL_QUERY_RESULT, &numberOfTrianglesGenerated_);
    glEndTransformFeedback();
    glDisable(GL_RASTERIZER_DISCARD);

    if(numberOfTrianglesGenerated_ != 0)
    {
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
    glGenVertexArrays(1, &pointVao_);
    glBindVertexArray(pointVao_);
    GLfloat* vertices = new GLfloat[numberOfPoint * numberOfPoint * numberOfPoint * 3];
    for (int i = 0; i < numberOfPoint; i++)
    {
        for (int j = 0; j < numberOfPoint; j++)
        {
            for (int k = 0; k < numberOfPoint; k++)
            {
                vertices[(i * numberOfPoint * numberOfPoint * 3) + j * numberOfPoint * 3 + k * 3 + 0] = float(i) / numberOfPoint;
                vertices[(i * numberOfPoint * numberOfPoint * 3) + j * numberOfPoint * 3 + k * 3 + 1] = float(j) / numberOfPoint;
                vertices[(i * numberOfPoint * numberOfPoint * 3) + j * numberOfPoint * 3 + k * 3 + 2] = float(k) / numberOfPoint;
            }
        }
    }
    glGenBuffers(1, &pointVbo_);
    glBindBuffer(GL_ARRAY_BUFFER, pointVbo_);
    glBufferData(GL_ARRAY_BUFFER, 3 * numberOfPoint * numberOfPoint * numberOfPoint * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    delete[] vertices;
    glBindVertexArray(0);

    glGenBuffers(1, &tbo_);
    glBindBuffer(GL_ARRAY_BUFFER, tbo_);
    glBufferData(GL_ARRAY_BUFFER, 5 * 8 * numberOfPoint * numberOfPoint * numberOfPoint * sizeof(float), nullptr, GL_STREAM_DRAW);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo_);
}

void TerrainCube::LiberateGPUMemory()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

void TerrainCube::SaveVertexData()
{
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_COPY_READ_BUFFER, tbo_);
    glBindBuffer(GL_COPY_WRITE_BUFFER, vbo_);
    glBufferData(GL_COPY_WRITE_BUFFER, numberOfTrianglesGenerated_ * 24 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, NULL, NULL, numberOfTrianglesGenerated_ * 24 * sizeof(GLfloat));
    
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}
