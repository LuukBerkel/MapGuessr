#include "gameObject.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "drawComponent.h"

gameObject::gameObject()
{
}

gameObject::~gameObject()
{
}

void gameObject::addComponent(std::shared_ptr<gameComponent> component)
{
	component->setOwner(this);
	components.push_back(component);

	if (drawableComponent == nullptr)
		drawableComponent = dynamic_pointer_cast<drawComponent>(component);
}

std::list<std::shared_ptr<gameComponent>> gameObject::getComponents()
{
	return std::list<std::shared_ptr<gameComponent>>();
}

void gameObject::draw(const glm::mat4&)
{
}

void gameObject::update(float elapsedTime)
{
}
