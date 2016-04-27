#pragma once
#include <vector>
#include <glm/common.hpp>

class GLSLProgram;
class TerrainCube;
class RandomTextures;
class Light;

class Terrain
{
public:
    Terrain();
    ~Terrain();
    void Render(const glm::mat4& view, const glm::mat4& projection, Light* light);
protected:
    std::vector<TerrainCube*> cubes_;
    RandomTextures* textures_;
    GLSLProgram* renderProgram_;
    bool renderWireframe_;
};

