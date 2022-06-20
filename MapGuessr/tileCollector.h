#pragma once
#include "tileBuilder.h"
#include "gameTile.h"
#include <memory>


class tileCollector {
	// This class holds all the methods of caching and collecting tile data
public:
	/**
	 * @brief This function collects the polygons of the internet and send them back in a sorted form.
	 * @param location The location boundries in which is used as BBOX for the request.
	 * @return A shared pointer with data that is collected. 
	*/
	std::shared_ptr<tileBuilder::tileData> collectTileDataInternet(glm::vec4& location);

	/**
	 * @brief This functions collects the polygons of the harddrive that have been cached there.
	 * @param location The location boundries which is used filename.
	 * @return A shared pointer with data that is collected. 
	*/
	std::shared_ptr<gameTile> collectGameTileChache(glm::vec4& location);

	/**
	 * @brief This functions check wether a file is on the harddrive with the pologon data.
	 * @param location The location boundries which is used filename.
	 * @return A bool that says if it there then true.
	*/
	bool checkGameTileChache(glm::vec4& location);

	/**
	 * @brief Writes a tile to the cache.
	 * @param tile The tile that is writen to the cache.
	*/
	void writeGameTileTooCache(std::shared_ptr<gameTile> tile);
};