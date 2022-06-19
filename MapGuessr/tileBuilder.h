#pragma once
 
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "gameTile.h"

class tileCollector;

class tileBuilder {
public:
	// This holds the differend terraintypes available to render
	enum class zoneType {
		GRASS,
		HOMES,
		INDUSTRIAL,
		FOREST,
		WATER,
		COMMERCIAL,
		FOODPROD,
		ROAD,
		EMPTY
	};

	// Holds a area with terrain type.
	class tileZone {
	public:
		tileBuilder::zoneType type;
		std::vector<std::vector<glm::vec2>> perimeter;
	};

	// Holds the tile data.
	class tileData {
	public:
		glm::vec4 location;
		std::vector<std::shared_ptr<tileZone>> data;
	};

	tileBuilder();

private:
	tileCollector* collector;

	std::shared_ptr<gameTile> collectTile(glm::vec4 location);

	void collectTileAsync(glm::vec4 location, std::shared_ptr<gameTile> tile);

	bool checkCache(glm::vec4 location);

	void collectCacheAsync(glm::vec4 location, std::shared_ptr<gameTile> tile);

	void addCache(std::shared_ptr<gameTile> tile);

};


