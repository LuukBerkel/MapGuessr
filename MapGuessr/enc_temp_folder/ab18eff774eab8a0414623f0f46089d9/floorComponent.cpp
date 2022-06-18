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
	if (type == tileBuilder::zoneType::WATER) {
		return glm::vec4(0, 0, 1, 1);
	}
	if (type == tileBuilder::zoneType::ROAD) {
		return glm::vec4(0, 0, 0, 1);
	}
	return glm::vec4(1, 1, 1, 1);
}



static bool calculateIntersect(glm::vec2 incoming[3], tileBuilder::tileZone zone) {


	int k = 1;
	glm::vec2 input[2];
	for (int l = 0; l < 3; l++)
	{
		input[0] = incoming[l].x < incoming[k].x ? incoming[l] : incoming[k];
		input[1] = incoming[l].x < incoming[k].x ? incoming[k] : incoming[l];

		int j = 1;
		glm::vec2 compare[2];
		for (int i =0 ; i < zone.perimeter.size(); i++) {
			// Collecting points
			compare[0] = zone.perimeter[i].x < zone.perimeter[j].x ? zone.perimeter[i] : zone.perimeter[j];
			compare[1] = zone.perimeter[i].x < zone.perimeter[j].x ? zone.perimeter[j] : zone.perimeter[i];

			// Calculating formulas..
			float fxComp = (compare[1].x - compare[0].x) / (compare[1].y - compare[0].y);
			float aComp = compare[0].y - fxComp * compare[0].x;
			float fxInp = (input[1].x - input[0].x) / (input[1].y - input[0].y);
			float aInp = input[0].y - fxInp * input[0].x;

			// calculating intersect point;
			double totalDirectionCoefficient =  fxInp - fxComp ;
			double totalAddSum = aComp - aInp;
			double xResult = totalAddSum / totalDirectionCoefficient;
			double yResult = fxComp * xResult + aComp;

			// checking wether it is on the line piece..

			if (xResult < compare[0].x || xResult > compare[1].x || xResult < input[0].x || xResult > input[1].x) {
				return false;
			}

			if (compare[0].y < compare[1].y && (yResult < compare[0].y || yResult > compare[1].y)) {
				return false;
			}
			else if (compare[0].y > compare[1].y && (yResult >= compare[0].y || yResult <= compare[1].y)) {
				return false;
			}

			if (input[0].y < input[1].y && (yResult <= input[0].y || yResult > input[1].y)) {
				return false;
			}
			else if (input[0].y > input[1].y && (yResult >= input[0].y || yResult < input[1].y)) {
				return false;
			}

			if ()



			j++;
			if (j >= zone.perimeter.size()) {
				j = 0;
			}
		}
		k++;
		if (k >= 3) {
			k = 0;
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
			glm::vec2 input[3] = { zones.perimeter[i], zones.perimeter[j] , zones.perimeter[k] };
			if (calculateIntersect(input, zones)) {
				vertices.push_back(tigl::Vertex::PC(glm::vec3(zones.perimeter[i].x, FLOOR_HEIGHT, zones.perimeter[i].y), color));
				vertices.push_back(tigl::Vertex::PC(glm::vec3(zones.perimeter[j].x, FLOOR_HEIGHT, zones.perimeter[j].y), color));
				vertices.push_back(tigl::Vertex::PC(glm::vec3(zones.perimeter[k].x, FLOOR_HEIGHT, zones.perimeter[k].y), color));
			}
			else {
				std::cout << "invalid";
			}

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
