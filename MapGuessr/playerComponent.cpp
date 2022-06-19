#include "playerComponent.h"



playerComponent::playerComponent(std::shared_ptr<fpsCamera> cam, std::shared_ptr<gameObject> obj, glm::vec3 relativePos)
{
	camera = cam;
	this->obj = obj;
	obj->relativeRotation = glm::vec3(0, 0, 0);
	pos = relativePos;
}

playerComponent::~playerComponent()
{
}

void playerComponent::update(float elapsedTime)
{
	// Setting actual pos camera
	obj->position = -camera->position;
	obj->rotation.y = -camera->rotation.y;

	// Setting relative postion object
	obj->enableRelatives = true;
	obj->relativePos = pos;
}
