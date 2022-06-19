#include "gameScene.h"
#include <cmath>

gameScene::gameScene(std::shared_ptr<fpsCamera> c)
{
	builder = tileBuilder();
	cam = c;
	

	gameTiles[1][1] = builder.collectTile(glm::vec4 (worldStartingPoint.x -0.005f, worldStartingPoint.y - 0.005f, worldStartingPoint.x + 0.005f, worldStartingPoint.y + 0.005f));
}

static float pow(float input) {
	return input * input;
}


float gameScene::calculateDistance(glm::vec3 l1, glm::vec3 l2)
{
	return std::sqrt(pow(l1.x - l2.x) + pow(l1.z, l2.z));
}



void gameScene::draw()
{
	for (size_t i = 0; i < viewDistance; i++)
	{
		for (size_t j = 0; j < viewDistance; j++)
		{
			if (gameTiles[i][j] != nullptr)
			gameTiles[i][j]->draw();
		}
	}

	for (std::shared_ptr<gameObject> tile : gameObjects) {
		tile->draw();
	}
}

void gameScene::changedWorldIndex(glm::vec2 worldIndex) {
	for (size_t i = 0; i < viewDistance; i++)
	{
		for (size_t j = 0; j < viewDistance; j++)
		{
			float ofsettX = -0.01f +  (i * 0.01);
			float ofsettY = -0.01f + (j * 0.01);
			glm::vec4 pos(
				worldStartingPoint.x + (worldIndex.x * 0.01) + ofsettX - 0.005, 
				worldStartingPoint.y + (worldIndex.y * 0.01) + ofsettY - 0.005, 
				worldStartingPoint.x + (worldIndex.x * 0.01) + ofsettX + 0.005,
				worldStartingPoint.y + (worldIndex.y * 0.01) + ofsettY + 0.005);

			gameTiles[i][j] = builder.collectTile(pos);
			gameTiles[i][j]->gamePosition = glm::vec3(i - (viewDistance/2) + worldIndex.x, 0, j - (viewDistance/2) + worldIndex.y);
		}
	}
}
	

void gameScene::update(float elapsedTime)
{
	glm::vec2 location = glm::vec2(worldStartingPoint.x - (cam->position.x / 100.0f), (worldStartingPoint.y - cam->position.z / 100.0f));

	float xDelta = location.x - worldStartingPoint.x;
	float yDelta = location.y - worldStartingPoint.y;

	float xIndex = xDelta / 0.01;
	float yIndex = yDelta / 0.01;

	// Updating world index;
	if (worldIndexing != glm::vec2((int)xIndex, (int)yIndex)) {
		changedWorldIndex(glm::vec2((int)xIndex, (int)yIndex));
		worldIndexing = glm::vec2((int)xIndex, (int)yIndex);
	}


	for (size_t i = 0; i < viewDistance; i++)
	{
		for (size_t j = 0; j < viewDistance; j++)
		{
			if (gameTiles[i][j] != nullptr)
			gameTiles[i][j]->update(elapsedTime);
			else {
				std::cout << i << " " << j << std::endl;
			}
		}
	}


	for (std::shared_ptr<gameObject> tile : gameObjects) {
		tile->update(elapsedTime);
	}
}
