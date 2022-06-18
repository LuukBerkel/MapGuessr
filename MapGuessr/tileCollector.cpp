#include "tileCollector.h"
#include <curl/curl.h>
#include <rapidxml/rapidxml.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <string.h>    

#define ERROR_MESSAGE_LENGTH 593

static tileBuilder::zoneType checkZoneType(rapidxml::xml_node<>* tag_node);
static std::shared_ptr<tileBuilder::tileData> parseData(std::string& incoming, glm::vec4 location);

size_t writeCallback(char* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}


std::shared_ptr<tileBuilder::tileData> tileCollector::collectTileDataInternet(glm::vec4& location)
{
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

		// Building request string...
		std::stringstream sstream;
		sstream << "data=%5Bbbox%3A" << std::fixed << std::setprecision(15) << location.x << "%2C" << location.y << "%2C" << location.z << "%2C" << location.w << "%5D%3B%0Anwr%20%5Blanduse%5D%3B%0Aout%20geom%3B";
		std::cout << sstream.str().c_str() << std::endl;

		// Conversion to C char pointer
		std::string dataString = sstream.str();
		const char* dataCharPTR = dataString.c_str();
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataCharPTR);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rawData);

		curl_easy_perform(curl);
	}
	curl_easy_cleanup(curl);

	// Dirty manner of checking wether or not the server send an error back.
	if (rawData.size() == ERROR_MESSAGE_LENGTH) {
		std::cout << "Error is send back )-;" << std::endl;
		return nullptr;
	}

	std::cout << "received shit" << std::endl;

	return parseData(rawData, location);
}

static std::shared_ptr<tileBuilder::tileData> parseData(std::string& incoming, glm::vec4 location) {
	// Creating object for saving data
	std::shared_ptr<tileBuilder::tileData> data = std::make_shared<tileBuilder::tileData>();

	// Parsing response data
	rapidxml::xml_document<> doc;
	doc.parse<0>(&incoming[0]);

	// Find our root node
	rapidxml::xml_node<>* root_node = doc.first_node("osm");

	// Looping through all perimters.
	for (rapidxml::xml_node<>* perimter_node = root_node->first_node("way"); perimter_node; perimter_node = perimter_node->next_sibling())
	{
		tileBuilder::tileZone zone = tileBuilder::tileZone::tileZone();
		zone.type = tileBuilder::zoneType::EMPTY;

		bool terrainTypeLoaded = false;

		// Parsing terraintype
		for (rapidxml::xml_node<>* tag_node = perimter_node->first_node("tag"); tag_node; tag_node = tag_node->next_sibling()) {
			if (checkZoneType(tag_node) != tileBuilder::zoneType::EMPTY) {
				zone.type = checkZoneType(tag_node);
				terrainTypeLoaded = true;
			}
		}

		// If no terrain type then ignore else execute loading perimeter
		if (terrainTypeLoaded) {
			perimter_node->first_node("tag")->first_attribute("v")->value();

			// Parsing perimter
			for (rapidxml::xml_node<>* coord_node = perimter_node->first_node("nd"); coord_node; coord_node = coord_node->next_sibling())
			{
				if (coord_node->first_attribute("lat") != nullptr || coord_node->first_attribute("lon") != nullptr) {
					float lat = (location.x - std::stof(coord_node->first_attribute("lat")->value()));
					float lon =	(location.y - std::stof(coord_node->first_attribute("lon")->value()));
					lat *= 100;
					lon *= 100;
					zone.perimeter.push_back(glm::vec2(lat, lon));
				}
			}

			data->data.push_back(zone);
		}
	}

	return data;
}

static tileBuilder::zoneType checkZoneType(rapidxml::xml_node<>* tag_node) {
	std::string value = std::string(tag_node->first_attribute("v")->value());
	// First degree
	if (value._Equal("commercial") || value._Equal("retail")) {
		return tileBuilder::zoneType::COMMERCIAL;
	}
	if (value._Equal("construction") || value._Equal("education") || value._Equal("residential")) {
		return tileBuilder::zoneType::HOMES;
	}
	if (value._Equal("farmland") || value._Equal("meadow")) {
		return tileBuilder::zoneType::GRASS;
	}
	if (value._Equal("allotments") || value._Equal("orchard") || value._Equal("vineyard")) {
		return tileBuilder::zoneType::FOODPROD;
	}
	if (value._Equal("forest")) {
		return tileBuilder::zoneType::FOREST;
	}

	return tileBuilder::zoneType::EMPTY;
}

std::shared_ptr<gameTile> tileCollector::collectGameTileChache(glm::vec4& location)
{
	return std::shared_ptr<gameTile>();
}

void tileCollector::writeGameTileTooCache(std::shared_ptr<gameTile> tile)
{
}
