#pragma once
#include <vector>
#include <glm/common.hpp>
#include "Input/Handler/IKeyboardKeyHandler.h"

class GLSLProgram;
class TerrainCube;
class RandomTextures;
class Light;

class Terrain : public IKeyboardKeyHandler
{
public:
    Terrain();
    ~Terrain();
    void Render(const glm::mat4& view, const glm::mat4& projection, Light* light);
    virtual void HandleKeyboardKey(int key, int action, int modifier) override;
protected:
    std::vector<TerrainCube*> cubes_;
    RandomTextures* textures_;
    GLSLProgram* renderProgram_;
    bool renderWireframe_;
};

