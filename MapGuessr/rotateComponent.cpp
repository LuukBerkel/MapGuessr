#include "rotateComponent.h"

rotateComponent::rotateComponent(gameObject* obj, float speed)
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
		obj->relativeRotation.x = obj->relativeRotation.x + (speed * elapsedTime);
		if (obj->relativeRotation.x > 360) obj->relativeRotation.x = 0;
	}
	else {
		obj->rotation.x = obj->rotation.x + (speed * elapsedTime);
		if (obj->rotation.x > 360) obj->relativeRotation.x = 0;
	}
}
