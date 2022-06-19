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

void gameObject::draw(const glm::mat4& parentMatrix)
{
	if (!drawableComponent)
		return;

	glm::mat4 modelMatrix = parentMatrix;
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, scale);

	// Can be used a secondary usage of moving when in position.
	if (enableRelatives) {
		modelMatrix = glm::translate(modelMatrix, relativePos);
		modelMatrix = glm::rotate(modelMatrix, relativeRotation.x, glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, relativeRotation.y, glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, relativeRotation.z, glm::vec3(0, 0, 1));
	}

	tigl::shader->setModelMatrix(modelMatrix);

	for (auto& c : components) {
		std::shared_ptr<drawComponent> dc = dynamic_pointer_cast<drawComponent>(c);
		if (dc != nullptr)
			dc->draw();
	}
}

void gameObject::update(float elapsedTime)
{
}
