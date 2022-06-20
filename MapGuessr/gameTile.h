#pragma once

#include <list>
#include <memory>
#include "gameObject.h"
#include <vector>
#include "tigl.h"

class gameTile {
private:
	std::vector<tigl::Vertex> vertices;
public:
	// Position in the game
	glm::vec3 gamePosition;
	glm::vec4 boundries;

	std::list<std::shared_ptr<gameObject>> gameObjects;

	// For setting objects
	gameTile(glm::vec4 boundries);
	~gameTile();

	// For adding extra objects
	void addObject(std::shared_ptr<gameObject> object);
	void addObjects(std::list<std::shared_ptr<gameObject>> objects);

	// Standard methods
	void draw();
	void update(float elapsedTime);
};