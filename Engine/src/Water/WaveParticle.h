#pragma once
#include "glm/glm.hpp"

class WaveParticle
{

public:
	WaveParticle();
	~WaveParticle();

	glm::vec2 GetPosition(float totalSimulationTime);

	float GetHeight();
	void Subdivide();

	void Initialize(glm::vec2 direction,
					glm::vec2 startPoint,
					float amplitude,
					float speed,
					float initialTime,
					float radius,
					float dispersionAngle);

	glm::vec2 direction_;
	glm::vec2 startPoint_;
	float amplitude_;
	float speed_;
	float initialTime_;
	float radius_;
	float dispersionAngle_;
	bool alive_;
};

