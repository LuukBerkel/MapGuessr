#include "gameTile.h"
#include <glm/gtc/matrix_transform.hpp>
#include "tigl.h"

gameTile::gameTile()
{
}

gameTile::~gameTile()
{
}

void gameTile::addObject(std::shared_ptr<gameObject> object)
{
	if (object != nullptr) gameObjects.push_back(object);
}

void gameTile::addObjects(std::list<std::shared_ptr<gameObject>> objects)
{
	gameObjects = objects;
}

void gameTile::draw()
{
	// Calculating a new model matrix.
	const glm::mat4& relativePos = glm::mat4(1.0f);
	glm::mat4 modelMatrix = relativePos;
	modelMatrix = glm::translate(modelMatrix, gamePosition);

	tigl::shader->setModelMatrix(modelMatrix);


	// Drawing objects.
	for (auto& object : gameObjects) {
		if (object != nullptr)
		object->draw(modelMatrix);
	}
}

void gameTile::update(float elapsedTime)
{
	for (auto& object : gameObjects) {
		if (object != nullptr)
		object->update(elapsedTime);
	}
}
