#include "tileCollector.h"
#include <curl/curl.h>
#include <rapidxml/rapidxml.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <string.h> 
#include <mutex>
#include <fstream>
#include <algorithm>
#include "floorComponent.h"

#define ERROR_MESSAGE_LENGTH 593

static tileBuilder::zoneType checkZoneType(rapidxml::xml_node<>* tag_node);
static std::shared_ptr<tileBuilder::tileData> parseData(std::string& incoming, glm::vec4 location, std::shared_ptr<tileBuilder::tileData> data);

size_t writeCallback(char* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::mutex curlMutex;

static std::string collectFromAPI(std::string input) {

	curlMutex.lock();
	// Setting up curl
	CURL* curl;
	std::string rawData;
	curl = curl_easy_init();
	// If setup then start setting up request and sending it.
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_URL, "https://overpass-api.de/api/interpreter");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		// Conversion to C char pointer
		const char* dataCharPTR = input.c_str();
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataCharPTR);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rawData);

		curl_easy_perform(curl);
	}
	curl_easy_cleanup(curl);

	curlMutex.unlock();

	return rawData;
}

std::shared_ptr<tileBuilder::tileData> tileCollector::collectTileDataInternet(glm::vec4& location)
{
	// Creating a raii pointer to the data loaded.
	std::shared_ptr<tileBuilder::tileData> data = std::make_shared<tileBuilder::tileData>();

	// Building request string...
	std::stringstream landuse;
	landuse << "data=%5Bbbox%3A" << std::fixed << std::setprecision(15) << location.x << "%2C" << location.y << "%2C" << location.z << "%2C" << location.w << "%5D%3B%0Anwr%20%5Blanduse%5D%3B%0Aout%20geom%3B";

	// Parsing landusage
	std::string res = collectFromAPI(landuse.str());
	parseData(res, location, data);

	// Building request string...
	std::stringstream water;
	water << "data=%5Bbbox%3A" << std::fixed << std::setprecision(15) << location.x << "%2C" << location.y << "%2C" << location.z << "%2C" << location.w << "%5D%3B%0Anwr%20%5B%22natural%22%3D%22water%22%5D%3B%0Aout%20geom%3B";
	
	// Parsing water
	res = collectFromAPI(water.str());
	parseData(res, location, data);

	std::cout << "Parsed tile with specs: " << location.x << ", " << location.y << "; It also has " << data->data.size() << "areas with a size in bytes of " << sizeof(data) << std::endl;
	return data;
}

static std::shared_ptr<tileBuilder::tileData> parseData(std::string& incoming, glm::vec4 location, std::shared_ptr<tileBuilder::tileData> data) {

	if (incoming.length() == 0 || incoming.length() == 711) {
		return nullptr;
	}

	// Parsing response data
	rapidxml::xml_document<> doc;
	doc.parse<0>(&incoming[0]);

	// Find our root node
	rapidxml::xml_node<>* root_node = doc.first_node("osm");
	if (root_node == nullptr) {
		return nullptr;
	}

	// Looping through the data on the way manner
	for (rapidxml::xml_node<>* perimter_node = root_node->first_node("way"); perimter_node; perimter_node = perimter_node->next_sibling())
	{
		std::shared_ptr<tileBuilder::tileZone> zone;

		bool terrainTypeLoaded = false;

		// Parsing terraintype
		for (rapidxml::xml_node<>* tag_node = perimter_node->first_node("tag"); tag_node; tag_node = tag_node->next_sibling()) {
			if (checkZoneType(tag_node) != tileBuilder::zoneType::EMPTY) {
				bool created = true;
				for (std::shared_ptr<tileBuilder::tileZone> type : data->data) {
					if (type->type == checkZoneType(tag_node)) {
						created = false;
						zone = type;
					}
				}

				// If it is not in the list then
				if (created) {
					zone = std::make_shared<tileBuilder::tileZone>();
					zone->type = checkZoneType(tag_node);
				}
				
				terrainTypeLoaded = true;
			}
		}

		// If no terrain type then ignore else execute loading perimeter
		if (terrainTypeLoaded) {
			std::vector<glm::vec2> coordinates;
			// Parsing perimter
			for (rapidxml::xml_node<>* coord_node = perimter_node->first_node("nd"); coord_node; coord_node = coord_node->next_sibling())
			{
				if (coord_node->first_attribute("lat") != nullptr || coord_node->first_attribute("lon") != nullptr) {
					float lat = (location.x - std::stof(coord_node->first_attribute("lat")->value()));
					float lon =	(location.y - std::stof(coord_node->first_attribute("lon")->value()));
					if (lat < 0 && lat > (location.x - location.z) && lon < 0 && lon > (location.y - location.w) || zone->type == tileBuilder::zoneType::WATER) {
						lat *= 100;
						lon *= 100;
						coordinates.push_back(glm::vec2(lat, lon));
					}
				}
			}
			zone->perimeter.push_back(coordinates);
			data->data.push_back(zone);
		}
	}

	// Looping through data on members and relations.
	for (rapidxml::xml_node<>* relation_node = root_node->first_node("relation"); relation_node; relation_node = relation_node->next_sibling())
	{
		std::shared_ptr<tileBuilder::tileZone> zone;

		bool terrainTypeLoaded = false;

		// Parsing terraintype
		for (rapidxml::xml_node<>* tag_node = relation_node->first_node("tag"); tag_node; tag_node = tag_node->next_sibling()) {
			if (checkZoneType(tag_node) != tileBuilder::zoneType::EMPTY) {
				bool created = true;
				for (std::shared_ptr<tileBuilder::tileZone> type : data->data) {
					if (type->type == checkZoneType(tag_node)) {
						created = false;
						zone = type;
					}
				}

				// If it is not in the list then
				if (created) {
					zone = std::make_shared<tileBuilder::tileZone>();
					zone->type = checkZoneType(tag_node);
				}

				terrainTypeLoaded = true;
			}
		}

		// If no terrain type then ignore else execute loading perimeter
		if (terrainTypeLoaded) {
			// Parsing perimter
			for (rapidxml::xml_node<>* member_node = relation_node->first_node("member"); member_node; member_node = member_node->next_sibling()) {
				std::vector<glm::vec2> coordinates;
				for (rapidxml::xml_node<>* coord_node = member_node->first_node("nd"); coord_node; coord_node = coord_node->next_sibling())
				{
					if (coord_node->first_attribute("lat") != nullptr || coord_node->first_attribute("lon") != nullptr) {
						float lat = (location.x - std::stof(coord_node->first_attribute("lat")->value()));
						float lon = (location.y - std::stof(coord_node->first_attribute("lon")->value()));
						if (lat < 0 && lat >(location.x - location.z) && lon < 0 && lon >(location.y - location.w) || zone->type == tileBuilder::zoneType::WATER) {
							lat *= 100;
							lon *= 100;
							coordinates.push_back(glm::vec2(lat, lon));
						}
					}
				}
				zone->perimeter.push_back(coordinates);
				data->data.push_back(zone);
			}
		}
	}

	return data;
}

tileBuilder::zoneType checkZoneType(rapidxml::xml_node<>* tag_node) {
	std::string value = std::string(tag_node->first_attribute("v")->value());
	std::string key = std::string(tag_node->first_attribute("k")->value());
	// First degree
	if (value._Equal("commercial") || value._Equal("retail")) {
		return tileBuilder::zoneType::COMMERCIAL;
	}
	if (value._Equal("construction") || value._Equal("education") || value._Equal("residential")) {
		return tileBuilder::zoneType::HOMES;
	}
	if (value._Equal("farmland") || value._Equal("grass") || value._Equal("recreation_ground") || value._Equal("meadow")) {
		return tileBuilder::zoneType::GRASS;
	}
	if (value._Equal("allotments") || value._Equal("orchard") || value._Equal("vineyard")) {
		return tileBuilder::zoneType::FOODPROD;
	}
	if (value._Equal("forest") || value._Equal("wood")) {
		return tileBuilder::zoneType::FOREST;
	}
	if (value._Equal("water")) {
		return tileBuilder::zoneType::WATER;
	}
	if (key._Equal("highway")) {
		return tileBuilder::zoneType::ROAD;
	}

	return tileBuilder::zoneType::EMPTY;
}

/**
* Splits a string into substrings, based on a seperator
*/
std::vector<std::string> split(std::string str, const std::string& seperator)
{
	std::vector<std::string> ret;
	size_t index;
	while (true)
	{
		index = str.find(seperator);
		if (index == std::string::npos)
			break;
		ret.push_back(str.substr(0, index));
		str = str.substr(index + 1);
	}
	ret.push_back(str);
	return ret;
}

std::shared_ptr<gameTile> tileCollector::collectGameTileChache(glm::vec4& location)
{
	// Creating filename
	std::stringstream filename;
	filename << location.x << "_" << location.y << "_" << location.z << "_" << location.w;
	std::string result = filename.str();


	// Using algortim for removing data..
	result.erase(std::remove_if(result.begin(), result.end(), [](char c) { return c == '.'; }));
	std::ifstream file(result + ".txt");

	// Reading using factory pattern...
	std::shared_ptr<gameTile> tile = nullptr;
	std::shared_ptr<floorComponent> component = nullptr;
	while (!file.eof())
	{
		std::string line;
		std::getline(file, line);

		std::vector<std::string> params = split(line, " ");
	

		if (params[0] == "boundries") {
			tile = std::make_shared<gameTile>(glm::vec4((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str()), (float)atof(params[4].c_str())));
		}
		else if (params[0] == "floor") {
			if (tile != nullptr) {
				component = std::make_shared<floorComponent>();
				std::shared_ptr<gameObject> obj = std::make_shared<gameObject>();
				obj->addComponent(component);
				obj->position = glm::vec3(0, 0, 0);
				tile->addObject(obj);
			}
		}
		else if (params[0] == "v") {
			if (component != nullptr) {
				component->vertices.push_back(tigl::Vertex::PCN(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str())),
					glm::vec4((float)atof(params[4].c_str()), (float)atof(params[5].c_str()), (float)atof(params[6].c_str()), (float)atof(params[7].c_str())),
					glm::vec3((float)atof(params[8].c_str()), (float)atof(params[9].c_str()), (float)atof(params[10].c_str()))));
			}
		}
	}

	std::cout << "Loaded " << result << ".txt" <<"from hdd cache" << std::endl;

	return tile;
}

bool tileCollector::checkGameTileChache(glm::vec4& location)
{
	// Creating filename
	std::stringstream filename;
	filename << location.x << "_" << location.y << "_" << location.z << "_" << location.w;
	std::string result = filename.str();


	// Using algortim for removing data..
	result.erase(std::remove_if(result.begin(), result.end(), [](char c) { return c == '.'; }));
	std::ifstream file(result + ".txt");

	// Checking if it open.
	if (!file.is_open())
	{
		std::cout << "Could not open file " << result << ".txt" << std::endl;
		return false;
	}

	file.close();
	return true;
}

void tileCollector::writeGameTileTooCache(std::shared_ptr<gameTile> tile)
{
	// Creating filename
	std::stringstream filename;
	filename  << tile->boundries.x << "_" << tile->boundries.y << "_" << tile->boundries.z << "_" << tile->boundries.w;
	std::string result = filename.str();
	

	// Using algortim for removing data..
	result .erase(std::remove_if(result.begin(), result.end(), [](char c) { return c == '.'; } ));

	// Added an item to cache..
	std::cout << "Added " << result << ".txt to hdd cache" << std::endl;

	// Create and open a text file
	std::ofstream file(result + ".txt");

	// Write to the file
	file << "boundries " << tile->boundries.x << " " << tile->boundries.y << " " << tile->boundries.z << " " << tile->boundries.w << std::endl;
	for (std::shared_ptr<gameObject> object : tile->gameObjects) {
		std::shared_ptr<floorComponent> c = object->getComponent<floorComponent>();
		if (c != nullptr) {
			file << "floor" << std::endl;
			for (tigl::Vertex vert : c->vertices) {
				file << "v " << vert.position.x << " " << vert.position.y << " " << vert.position.z								// Position
					<< " " << vert.color.x << " " << vert.color.y << " " << vert.color.z << " " << vert.color.w					// Color
					<< " " << vert.normal.x << " " << vert.normal.y << " " << vert.normal.z << std::endl;						// Normal
			}
		}
	}

	// Close the file
	file.close();
}
