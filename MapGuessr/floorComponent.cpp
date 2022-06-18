#include "floorComponent.h"
#include <cmath>


static float pow(float input) {
	return input * input;
}

static glm::vec4 collectColor(tileBuilder::zoneType type) {
	if (type == tileBuilder::zoneType::COMMERCIAL || type == tileBuilder::zoneType::HOMES) {
		return glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	}
	if (type == tileBuilder::zoneType::GRASS) {
		return glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
	}
	if (type == tileBuilder::zoneType::FOODPROD) {
		return glm::vec4(0.588f, 0.294f, 0.0f, 1.0f);
	}
	if (type == tileBuilder::zoneType::FOREST) {
		return glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
	}
	return glm::vec4(1, 1, 1, 1);
}

static float calculateDistance(glm::vec2 coordinate1, glm::vec2 coordinate2) {
	return std::sqrt(pow(coordinate1.x - coordinate2.x) + pow(coordinate1.y - coordinate2.y));
}

static bool guardCheck(glm::vec2 coordinates[3], tigl::Vertex vertex) {
	for (int i = 0; i < 3; i++)
	{
		if (coordinates[i].x == vertex.position.x && coordinates[i].y == vertex.position.y) {
			return false;
		}
	}

	return true;
}

floorComponent::floorComponent(tileBuilder::tileZone zones)
{
	// Setting color for vertices
	glm::vec4 color= collectColor(zones.type);

	// Trainuglated problem fix is this...
	for (int g = 2; g < 5; g++) {
		int i = 0;
		int j = 1;
		int k = zones.perimeter.size() / g;
		while (i < zones.perimeter.size()) {
			vertices.push_back(tigl::Vertex::PC(glm::vec3(zones.perimeter[i].x, FLOOR_HEIGHT, zones.perimeter[i].y), color));
			vertices.push_back(tigl::Vertex::PC(glm::vec3(zones.perimeter[j].x, FLOOR_HEIGHT, zones.perimeter[j].y), color));
			vertices.push_back(tigl::Vertex::PC(glm::vec3(zones.perimeter[k].x, FLOOR_HEIGHT, zones.perimeter[k].y), color));

			// Looping thingies.
			i++;
			j++;
			k++;
			if (j == zones.perimeter.size()) j = 0;
			if (k == zones.perimeter.size()) k = 0;
		}
	}
}

floorComponent::~floorComponent()
{
}

void floorComponent::draw()
{
	tigl::drawVertices(GL_TRIANGLES, vertices);
}
