#pragma once
#include "RenderTree\Node\AbstractNode.h"

class WaveParticleRenderer;
class WaveParticleManager;

class Water :
    public AbstractNode
{
public:
    Water(AbstractNode* parent);
    virtual ~Water();

    virtual void Render(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane = glm::vec4(0)) override;
    virtual void RenderReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement, const glm::vec4& clipPlane, glm::mat4 shadowModel) override;
    virtual void RenderShadowMap(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection) override;
    virtual void RenderFirstPass(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, glm::vec4& clipPlane = glm::vec4(0.0)) override;
    virtual void Update(double deltaT) override;
    
    void RenderWaterReflection(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement);
    void RenderWaterRefraction(glm::mat4 model, const glm::mat4& view, const glm::mat4& projection, Environment* environnement);
    void ApplyReflectionTransformation(glm::mat4& modelReflection);
    void RenderModel(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, Environment* e, const glm::vec4& clipPlane = glm::vec4(0));
    void RenderModelFirstPass(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, const glm::vec4& clipPlane = glm::vec4(0));
    void LoadModel();
private:
    static int waterNumberOfVertexWidth_;
    static int waterNumberOfVertexHeight_;
    static float shininess_;
    static glm::vec3 ambientMaterial_;
    static glm::vec3 diffuseMaterial_;
    static glm::vec3 specularMaterial_;
    
    bool renderingWater_;
    void CreateBuffers();
    
    GLuint reflectionFbo_;
    GLuint reflectionTexture_;
    GLuint reflectionDepthBuffer_;
    
    GLuint refractionFbo_;
    GLuint refractionTexture_;
    GLuint refractionDepthBuffer_;

    GLuint textureWidth_;
    GLuint textureHeight_;

    GLSLProgram* waterProgram_;
    GLSLProgram* firstPassProgram_;

    GLuint vao_;
    GLuint vbo_[3];

    double time_;
    float waveHeight_;
    float reflectionPerturbationFactor_;

    GLuint WaveParticleFbo_;
    GLuint waveStartPointDirectionTexture_;
    GLuint waveSpeedTimeAmplitudeRadiusTexture_;

    unsigned int numberOfFaces_;

    WaveParticleManager* particleManager_;
};

