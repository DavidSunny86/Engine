#pragma once
#include "RenderTree/Environment/Environment.h"
#include "RenderTree/Node/RenderTree.h"


class Camera;
class Environment;

class Scene
{
public:
    Scene();
    ~Scene();

    void Render();
    void Update(double deltaT);

    void SetRenderTree(RenderTree* tree);
    void SetEnvironment(Environment* environment);

	Camera* GetCamera() {return camera_;}
private:
    Environment* environment_;
    RenderTree* renderTree_;
    Camera* camera_;
};

