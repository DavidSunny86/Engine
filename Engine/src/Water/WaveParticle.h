#pragma once
#include "glm/glm.hpp"

class WaveParticle
{

public:
	WaveParticle();
	~WaveParticle();

	glm::vec2 GetPosition();
    
    void Update(float deltaT, float* heightMap, int width, int height);
	float GetHeight(const glm::vec2& center, const glm::vec2& position);
    float BlendFuction(float position);
    float FindTimeSinceCollision(float position, float speed, float startPoint);
    void Subdivide();

	void Initialize(glm::vec2 direction,
					glm::vec2 startPoint,
					float amplitude,
					float speed,
					float time_,
					float radius,
					float dispersionAngle);


    
	glm::vec2 direction_;
	glm::vec2 startPoint_;
    bool alive_;
    float time_;
	float amplitude_;
	float speed_;
	float radius_;
	float dispersionAngle_;
    float minAmplitude_;
};

