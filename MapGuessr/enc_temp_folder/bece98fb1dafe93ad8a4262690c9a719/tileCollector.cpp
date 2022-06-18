#include "tileCollector.h"
#include <curl/curl.h>
#include <rapidxml/rapidxml.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <string.h>    


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
		//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataCharPTR);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rawData);

		curl_easy_perform(curl);
	}
	curl_easy_cleanup(curl);

	std::cout << rawData.size() << std::endl;

	//// Parsing response data
	//rapidxml::xml_document<> doc;
	//doc.parse<0>(&rawData[0]);

	//// Find our root node
	//rapidxml::xml_node<>* root_node = doc.first_node("osm");

	//// Looping through all perimters.
	//for (rapidxml::xml_node<>* perimter_node = root_node->first_node("way"); perimter_node; perimter_node = perimter_node->next_sibling())
	//{
	//	// Guard if to check if it has land usage.
	//	if (strcmp(perimter_node->first_node("tag")->first_attribute("k")->value(), "landuse") == 0 ){
	//		
	//		tileBuilder::tileZone zone = tileBuilder::tileZone::tileZone();

	//		// Checking what type of terrain it is...
	//		

	//		// Parsing perimter
	//		for (rapidxml::xml_node<>* coord_node = coord_node->first_node("nd"); coord_node; coord_node = coord_node->next_sibling())
	//		{
	//			
	//		}

	//	}
	//}

	





	return std::shared_ptr<tileBuilder::tileData>();
}

std::shared_ptr<gameTile> tileCollector::collectGameTileChache(glm::vec4& location)
{
	return std::shared_ptr<gameTile>();
}

void tileCollector::writeGameTileTooCache(std::shared_ptr<gameTile> tile)
{
}
