#pragma once
#include "gameTile.h"
#include "tileBuilder.h"
#include "fpsCamera.h"


const int viewDistance = 3;

class gameScene {
private:
	// This sets the zero point as map coord.
	glm::vec2 worldStartingPoint = glm::vec2(51.59530934936532, 4.778385088898252);
	glm::uvec2 worldIndexing = glm::uvec2(-1, -1);

	std::shared_ptr<gameTile> gameTiles[viewDistance][viewDistance];
	std::vector<std::shared_ptr<gameObject>> gameObjects;

	tileBuilder builder;
	std::shared_ptr<fpsCamera> cam;

	float calculateDistance(glm::vec3 l1, glm::vec3 l2);
public:
	gameScene(std::shared_ptr<fpsCamera> c);
	void changedWorldIndex(glm::uvec2 worldIndex);

	void draw();
	void update(float elapsedTime);
};