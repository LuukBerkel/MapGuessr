#pragma once
#include "tileBuilder.h"
#include "gameTile.h"
#include <memory>

class tileCollector {
	// This class holds all the methods of caching and collecting tile data
public:
	std::shared_ptr<tileBuilder::tileData> collectTileDataInternet(glm::vec4& location);
	std::shared_ptr<gameTile> collectGameTileChache(glm::vec4& location);
	void writeGameTileTooCache(std::shared_ptr<gameTile> tile);
};