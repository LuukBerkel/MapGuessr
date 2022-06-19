#include "gameScene.h"
#include <cmath>
#include "objectComponent.h"
#include "playerComponent.h"
#include "rotateComponent.h"

gameScene::gameScene(std::shared_ptr<fpsCamera> c)
{
	// Setting up builder and camera...
	builder = tileBuilder();
	cam = c;

	// Adding simple object files...
	// Adding a helicopter
	std::shared_ptr <gameObject> heli = std::make_shared<gameObject>();
	heli->addComponent(std::make_shared<objectComponent>("resources/chopper/chopper.obj"));
	heli->addComponent(std::make_shared<playerComponent>(cam, heli, glm::vec3(0, -0.5f, -20)));
	heli->scale = glm::vec3(0.01f);
	gameObjects.push_back(heli);


	// Adding some clouds
	std::shared_ptr <gameObject> cloud0 = std::make_shared<gameObject>();
	cloud0->addComponent(std::make_shared<objectComponent>("resources/cloud/cloud.obj"));
	cloud0->addComponent(std::make_shared<playerComponent>(cam, cloud0, glm::vec3(-30, -0.5f, -30)));
	cloud0->addComponent(std::make_shared<rotateComponent>(cloud0, 0.67f));
	cloud0->scale = glm::vec3(0.005f);
	gameObjects.push_back(cloud0);

	std::shared_ptr <gameObject> cloud1 = std::make_shared<gameObject>();
	cloud1->addComponent(std::make_shared<objectComponent>("resources/cloud/cloud.obj"));
	cloud1->addComponent(std::make_shared<playerComponent>(cam, cloud1, glm::vec3(25, -0.5f, -25)));
	cloud1->addComponent(std::make_shared<rotateComponent>(cloud1, 0.61f));
	cloud1->scale = glm::vec3(0.007f);
	gameObjects.push_back(cloud1);

	std::shared_ptr <gameObject> cloud2 = std::make_shared<gameObject>();
	cloud2->addComponent(std::make_shared<objectComponent>("resources/cloud/cloud.obj"));
	cloud2->addComponent(std::make_shared<playerComponent>(cam, cloud2, glm::vec3(10, -0.5f, -40)));
	cloud2->addComponent(std::make_shared<rotateComponent>(cloud2, 0.61f));
	cloud2->scale = glm::vec3(0.003f);
	gameObjects.push_back(cloud2);

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
