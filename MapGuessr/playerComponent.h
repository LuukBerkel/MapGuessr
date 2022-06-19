#pragma once
#include "gameComponent.h"
#include "fpsCamera.h"
#include <memory>

class playerComponent : public gameComponent {
private:
	std::shared_ptr<fpsCamera> camera;
	gameObject* obj;
	glm::vec3 pos;
public:
	playerComponent(std::shared_ptr<fpsCamera> cam, gameObject* obj, glm::vec3 relativePos);
	~playerComponent();
	virtual void update(float elapsedTime) override;
};
