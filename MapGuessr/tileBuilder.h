#pragma once
 
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class tileBuilder {
public: 
	// This holds the differend terraintypes available to render
	enum class zoneType {
		GRASS,
		HOMES,
		INDUSTRIAL,
		FOREST,
		WATER,
		SAND,
		SNOW
	};

	// Holds a area with terrain type.
	class tileZone {
	public:
		tileBuilder::zoneType type;
		std::vector<glm::vec2> perimeter;
	};

	// Holds the tile data.
	class tileData {
	public:
		glm::vec4 location;
		std::vector<tileZone> data;
	};


};