#pragma once
#include <vector>
#include <glm/common.hpp>

class GLSLProgram;
class TerrainCube;
class RandomTextures;
class Terrain
{
public:
    Terrain();
    ~Terrain();
    void Render(const glm::mat4& view, const glm::mat4& projection);
protected:
    std::vector<TerrainCube*> cubes_;
    RandomTextures* textures_;
    GLSLProgram* renderProgram_;
};

