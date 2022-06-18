#pragma once

#include <list>
#include <memory>
#include "gameObject.h"

class gameTile {
private:
	std::list<std::shared_ptr<gameObject>> gameObjects;
public:
	// Position in the game
	glm::vec3 gamePosition;
	glm::vec3 worldPosition;

	// For setting objects
	gameTile(std::list<std::shared_ptr<gameObject>> parameterObjects);
	~gameTile();

	// For adding extra objects
	void addObject(std::shared_ptr<gameObject> object);

	// Standard methods
	void draw();
	void update(float elapsedTime);
};