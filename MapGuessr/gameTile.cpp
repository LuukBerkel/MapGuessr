#include "gameTile.h"
#include <glm/gtc/matrix_transform.hpp>
#include "tigl.h"
#include "floorComponent.h"

gameTile::gameTile()
{
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(0, FLOOR_HEIGHT -0.002, 0), glm::vec4(0.76, 0.69, 0.5, 1.0f ), glm::vec3(1, 0, 1)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1, FLOOR_HEIGHT - 0.002, 0), glm::vec4(0.76, 0.69, 0.5, 1.0f), glm::vec3(1, 0, 1)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1, FLOOR_HEIGHT - 0.002, -1), glm::vec4(0.76, 0.69, 0.5, 1.0f), glm::vec3(1, 0, 1)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(0, FLOOR_HEIGHT - 0.002, -1), glm::vec4(0.76, 0.69, 0.5, 1.0f), glm::vec3(1, 0, 1)));
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

	tigl::drawVertices(GL_QUADS, vertices);


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
