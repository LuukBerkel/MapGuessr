#include "rotateComponent.h"

rotateComponent::rotateComponent(std::shared_ptr<gameObject> obj, float speed)
{
	this->obj = obj;
	this->speed = speed;
}

rotateComponent::~rotateComponent()
{
}

void rotateComponent::update(float elapsedTime)
{
	if (obj->enableRelatives) {
		obj->relativeRotation.y = obj->relativeRotation.y + (speed * elapsedTime);
		if (obj->relativeRotation.y > 360) obj->relativeRotation.y = 0;
	}
	else {
		obj->rotation.y = obj->rotation.y + (speed * elapsedTime);
		if (obj->rotation.y > 360) obj->relativeRotation.x = obj->relativeRotation.y -360;
	}
}
