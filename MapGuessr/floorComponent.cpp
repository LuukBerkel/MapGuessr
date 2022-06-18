#include "floorComponent.h"
#include <cmath>
#include "triangulate.h"


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
		return glm::vec4(0.3f, 0.5f, 0.3f, 1.0f);
	}
	if (type == tileBuilder::zoneType::WATER) {
		return glm::vec4(0, 0, 1, 1);
	}
	if (type == tileBuilder::zoneType::ROAD) {
		return glm::vec4(0, 0, 0, 1);
	}
	return glm::vec4(1, 1, 1, 1);
}

static float collectOfsett(tileBuilder::zoneType type) {
	if (type == tileBuilder::zoneType::COMMERCIAL || type == tileBuilder::zoneType::HOMES) {
		return 0.00001f;
	}
	if (type == tileBuilder::zoneType::GRASS) {
		return 0.00002f;
	}
	if (type == tileBuilder::zoneType::FOODPROD) {
		return 0.00002f;
	}
	if (type == tileBuilder::zoneType::FOREST) {
		return 0.00003f ;
	}
	if (type == tileBuilder::zoneType::WATER) {
		return -0.0004f;
	}
	if (type == tileBuilder::zoneType::ROAD) {
		return 0.0005f;
	}
	return 0;
}

floorComponent::floorComponent(tileBuilder::tileZone zones)
{
	std::vector<tigl::Vertex> vertices;

	// Setting color for vertices
	glm::vec4 color= collectColor(zones.type);
	float ofsett = collectOfsett(zones.type);

	Vector2dVector a;
	for (glm::vec2 v : zones.perimeter) {
		a.push_back(Vector2d(v.x, v.y));
	}

	// allocate an STL vector to hold the answer.
	Vector2dVector result;

	//  Invoke the triangulator to triangulate this polygon.
	Triangulate::Process(a, result);
	
	for (Vector2d vec : result)
	{
		vertices.push_back(tigl::Vertex::PCN(glm::vec3(vec.GetX(), FLOOR_HEIGHT + ofsett, vec.GetY()), color, glm::vec3(0,1,0)));
	}

	vbo = tigl::createVbo(vertices);
}

floorComponent::~floorComponent()
{
}

void floorComponent::draw()
{
	tigl::drawVertices(GL_TRIANGLES, vbo);
}
