#include "tileBuilder.h"

#include "floorComponent.h"
#include <thread>
#include "tileCollector.h"


tileBuilder::tileBuilder()
{
	collector = new tileCollector();
}

static std::map<std::vector<float>, std::shared_ptr<gameTile>> cachedTiles;
static std::mutex cachedObjectsTiles;

std::shared_ptr<gameTile> tileBuilder::collectTile(glm::vec4 location)
{
	cachedObjectsTiles.lock();
	for (auto [key, val] : cachedTiles)
	{
		if (key[0] == location.x && key[1] == location.y && key[2] == location.z && key[3] == location.w) {
			cachedObjectsTiles.unlock();
			return val;
		}
	}

	std::shared_ptr<gameTile> tile = std::make_shared<gameTile>();
	std::vector<float> locations = {location.x, location.y, location.z, location.w};
	
	cachedTiles.emplace(locations, tile );
	cachedObjectsTiles.unlock();

	// Check cache if it on the pc
	if (checkCache(location)) {
		std::thread thread(&tileBuilder::collectCacheAsync, this, location, tile);
		thread.detach();
	}
	else {
		std::thread thread(&tileBuilder::collectTileAsync, this, location, tile);
		thread.detach();
	}

	return tile;
}



void tileBuilder::collectTileAsync(glm::vec4 location, std::shared_ptr<gameTile> tile)
{
	// Collecting from the internet...
	std::shared_ptr<tileBuilder::tileData> data = collector->collectTileDataInternet(location);

	// Adding some objects for some sphere in my game...
	if (data == nullptr) {
		return;
	}

	// Adding the underground components to the game...
	std::list<std::shared_ptr<gameObject>> objects;
	for (std::shared_ptr<tileBuilder::tileZone> zone : data->data) {
		std::shared_ptr<gameObject> object = std::make_shared<gameObject>();
		object->addComponent(std::make_shared<floorComponent>(zone));
		object->position = glm::vec3(0, 0,0 );
		object->scale = glm::vec3(1.0f);
		object->rotation = glm::vec3(0, 0, 0);
		objects.push_back(object);
	}

	// Setting gameposition
	tile->addObjects(objects);
	tile->worldPosition = location;

	// Adding item to cache
	// TODO create cache funtions...
}

bool tileBuilder::checkCache(glm::vec4 location)
{
	return false;
}

void tileBuilder::collectCacheAsync(glm::vec4 location, std::shared_ptr<gameTile> tile)
{
}

void tileBuilder::addCache(std::shared_ptr<gameTile> tile)
{
}
