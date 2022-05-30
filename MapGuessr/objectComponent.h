#pragma once
#include "gameComponent.h"
#include "textureComponent.h"
#include "tigl.h"
#include <String>
#include <Vector>
#include <Memory>

class objectComponent : gameComponent{
private:
	// This holds the vertices.
	class ObjectGroup {
	public:
		std::string name;					// Object group name
		tigl::VBO *bufferedObjectVertices;	// List of vertices
		int materialIndex;					// Index for the texture file
	};

	// Holds the texture data.
	class MaterialInfo {
	public:
		MaterialInfo();
		std::string name;
		std::shared_ptr<textureComponent> texture;
	};

	// Holds the object
	std::vector<std::shared_ptr<ObjectGroup>> groups;
	std::vector<std::shared_ptr<MaterialInfo>> materials;
	
	// Loads in the texture data.
	void loadMaterialFile(const std::string& fileName, const std::string& dirName);

public:
	/**
	 * @brief Loads an object in to memory for rendering with openGL
	 * 
	 * @param fileName The path to the file.
	*/
	objectComponent() = default;
	objectComponent(const std::string& fileName);
	~objectComponent();

	/**
	* @brief Inherted function for drawing the object.
	*/
	void draw() override;
};
