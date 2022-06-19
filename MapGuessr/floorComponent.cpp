#include "floorComponent.h"
#include <cmath>
#include "triangulate.h"


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
		return 0.0001f;
	}
	if (type == tileBuilder::zoneType::GRASS) {
		return 0.0002f;
	}
	if (type == tileBuilder::zoneType::FOODPROD) {
		return 0.0002f;
	}
	if (type == tileBuilder::zoneType::FOREST) {
		return 0.0003f ;
	}
	if (type == tileBuilder::zoneType::WATER) {
		return -0.0001f;
	}
	if (type == tileBuilder::zoneType::ROAD) {
		return 0.0005f;
	}
	return 0;
}

floorComponent::floorComponent(std::shared_ptr<tileBuilder::tileZone> zones)
{
	// Setting color for vertices
	glm::vec4 color= collectColor(zones->type);
	float ofsett = collectOfsett(zones->type);

	// Creating all vertices and pushing them in a buffer.
	for (std::vector<glm::vec2> ve : zones->perimeter) {
		Vector2dVector a;

		for (glm::vec2 v : ve) {
			a.push_back(Vector2d(v.x, v.y));
		}

		// allocate an STL vector to hold the answer.
		Vector2dVector result;

		//  Invoke the triangulator to triangulate this polygon.
		Triangulate::Process(a, result);

		
		srand(time(NULL));

		if (zones->type == tileBuilder::zoneType::COMMERCIAL || zones->type == tileBuilder::zoneType::HOMES) {
			float height = 0.1;

			// Creating buildings
			if (ve.size() > 1) {
				int j = 1;
				for (size_t i = 0; i < ve.size(); i++)
				{
					// Drawing squares....
					vertices.push_back(tigl::Vertex::PCN(glm::vec3(ve[i].x, FLOOR_HEIGHT, ve[i].y), color, glm::vec3(1, 0, 1)));
					vertices.push_back(tigl::Vertex::PCN(glm::vec3(ve[j].x, FLOOR_HEIGHT, ve[j].y), color, glm::vec3(1, 0, 1)));
					vertices.push_back(tigl::Vertex::PCN(glm::vec3(ve[i].x, FLOOR_HEIGHT + height, ve[i].y), color, glm::vec3(1, 0, 1)));

					vertices.push_back(tigl::Vertex::PCN(glm::vec3(ve[i].x, FLOOR_HEIGHT + height, ve[i].y), color, glm::vec3(1, 0, 1)));
					vertices.push_back(tigl::Vertex::PCN(glm::vec3(ve[j].x, FLOOR_HEIGHT + height, ve[j].y), color, glm::vec3(1, 0, 1)));
					vertices.push_back(tigl::Vertex::PCN(glm::vec3(ve[j].x, FLOOR_HEIGHT, ve[j].y), color, glm::vec3(1, 0, 1)));

					j++;
					if (j >= ve.size()) j = 0;
				}
			}

			// Adding in result
			for (Vector2d vec : result)
			{
				vertices.push_back(tigl::Vertex::PCN(glm::vec3(vec.GetX(), FLOOR_HEIGHT + height, vec.GetY()), color, glm::vec3(0, 1, 0)));
			}
		}
		else {

			// Adding in result
			for (Vector2d vec : result)
			{
				vertices.push_back(tigl::Vertex::PCN(glm::vec3(vec.GetX(), FLOOR_HEIGHT + ofsett, vec.GetY()), color, glm::vec3(0, 1, 0)));
			}
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
