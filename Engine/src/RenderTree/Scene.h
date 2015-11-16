#pragma once
#include "RenderTree/Environment/Environment.h"
#include "RenderTree/Node/RenderTree.h"
#include "Utility/Camera/Camera.h"
#include "RenderTree/Node/Skybox.h"
class Scene
{
public:
    Scene();
    ~Scene();

    void Render();
    void Update(double deltaT);

    void SetRenderTree(RenderTree* tree);
    void SetEnvironment(Environment* environment);
private:
    Environment* environment_;
    RenderTree* renderTree_;
    Camera* camera_;
};

