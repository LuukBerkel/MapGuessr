#pragma once

#include "drawComponent.h"
#include "textureComponent.h"

#include <List>
#include <Memory>
#include <Vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <queue>
#include "tigl.h"
#include <mutex>
#include <map>

#define GL_CALLS_HANDLE_AMOUNT 10


class OBJComponent : public drawComponent {
private:
	// Animation data
	float animationDelay = 0;
	float animationTime = 0;
	int animationIndex = 0;

	// This holds the vertices.
	class ObjectGroup {
	public:
		std::string name;					// Object group name
		tigl::VBO* bufferedObjectVertices;	// List of vertices
		int materialIndex;					// Index for the texture file
	};

	// Holds the texture data.
	class MaterialInfo {
	public:
		MaterialInfo();
		std::string name;
		std::shared_ptr<textureComponent> texture;
		glm::vec4 specular;
		glm::vec4 ambient;
		glm::vec4 diffuse;
	};

public:
	// Holds a object file
	class ObjectFile {
	public:
		// Holds the object
		std::vector<std::shared_ptr<ObjectGroup>> groups;
		std::vector<std::shared_ptr<MaterialInfo>> materials;
		int animationIndex;
	};

	// Object that is created when building to communicate with gl thread.
	class ObjectBuilderContainer {
	public:
		tigl::VBO* asyncObjectVBOCall(std::vector<tigl::Vertex> vertices);
		std::shared_ptr<textureComponent> asyncObjectTextureCall(std::string path);

		// Used for operation safety on objects.
		std::mutex buildLock;
		bool inputGiven = false;
		bool outputGiven = false;

		// Type of build operation
		int operation = -1;

		// Used for VBO create calls.
		std::vector<tigl::Vertex> verticesRequest;
		tigl::VBO* vboResponse;

		// Used for texure create calls.
		std::string pathRequest;
		std::shared_ptr<textureComponent> textureResponse;
	};

private:
	
	// Holds the information about the object
	std::shared_ptr<ObjectFile> objectData = nullptr;

	// Loads in the texture data.
	void loadObjectFile(const std::string fileName, std::shared_ptr<ObjectBuilderContainer> context, int listIndex);
	void loadMaterialFile(const std::string& fileName, const std::string& dirName, std::shared_ptr<ObjectFile>& file, std::shared_ptr<ObjectBuilderContainer> context);
public:
	/**
	 * @brief This method loads object model on a async thread.
	 * @param fileName The filename and path from run directory if needed for the object file.
	 * @param loaded This is a pointer that says wether a file is loaded in chache or not.
	*/
	OBJComponent(const std::string& fileName);
	~OBJComponent();

	virtual void draw() override;
};

// Global private caching map.
static std::map<std::string, std::shared_ptr<OBJComponent::ObjectFile>> cachedObjects;
static std::mutex cachedObjectsLock;

// Holds the amount of working threads.
static std::queue<std::shared_ptr<OBJComponent::ObjectBuilderContainer>> buildQueue;
static std::mutex buildQueueLock;

// Loads the objects and is called once every cycle.
void loadObjectIterate();