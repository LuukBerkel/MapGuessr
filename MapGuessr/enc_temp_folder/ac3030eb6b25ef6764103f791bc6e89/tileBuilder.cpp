#include "tileBuilder.h"
#include "floorComponent.h"

std::shared_ptr <gameTile> tileBuilder::collectTile(glm::vec4 location)
{
	std::shared_ptr<gameTile> tile = nullptr;
	// TODO implement loading cache

	// Else load from internet...
	std::shared_ptr<tileBuilder::tileData> data = collector.collectTileDataInternet(location);

	
}




tileBuilder::tileBuilder()
{
	collector = tileCollector();
}

void tileBuilder::collectTileAsync(glm::vec4 location)
{
	// Adding some objects for some sphere in my game...

	// Adding the underground components to the game...
	std::list<std::shared_ptr<gameObject>> objects;
	for (tileBuilder::tileZone zone : data->data) {
		std::shared_ptr<gameObject> object = std::make_shared<gameObject>();
		object->addComponent(std::make_shared<floorComponent>(zone));
		object->position = glm::vec3(0.0f);
		object->scale = glm::vec3(1.0f);
		objects.push_back(object);
	}

	// Setting gameposition
	tile = std::make_shared<gameTile>(objects);
	tile->worldPosition = location;
	tile->gamePosition = glm::vec3(0.0f);

	// Adding item to cache
}

bool tileBuilder::checkCache(glm::vec4 location)
{
	return false;
}

void tileBuilder::collectCacheAsync(glm::vec4 location)
{
}

void tileBuilder::addCache(std::shared_ptr<gameTile> tile)
{
}
