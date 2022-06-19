#pragma once

#include <list>
#include <memory>
#include "gameObject.h"
#include <vector>
#include "tigl.h"

class gameTile {
private:
	std::list<std::shared_ptr<gameObject>> gameObjects;
	std::vector<tigl::Vertex> vertices;
public:
	// Position in the game
	glm::vec3 gamePosition;
	glm::vec3 worldPosition;

	// For setting objects
	gameTile();
	~gameTile();

	// For adding extra objects
	void addObject(std::shared_ptr<gameObject> object);
	void addObjects(std::list<std::shared_ptr<gameObject>> objects);

	// Standard methods
	void draw();
	void update(float elapsedTime);
};