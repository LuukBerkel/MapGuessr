#include "playerComponent.h"

playerComponent::playerComponent(std::shared_ptr<fpsCamera> cam, gameObject* obj, glm::vec3 relativePos)
{
	camera = cam;
	this->obj = obj;
	obj->relativeRotation = glm::vec3(0, 0, 0);
}

playerComponent::~playerComponent()
{
}

void playerComponent::update(float elapsedTime)
{
	// Setting actual pos camera
	obj->position = camera->position;
	obj->rotation.x = camera->rotation.x;

	// Setting relative postion object
	obj->enableRelatives = true;
	obj->relativePos = pos;
}
