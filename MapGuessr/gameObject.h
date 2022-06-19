#pragma once

#include <list>
#include <iostream>
#include <glm/glm.hpp>
#include <list>
#include <memory>


class drawComponent;
class gameComponent;

class gameObject
{
	std::shared_ptr<drawComponent> drawableComponent;
	std::list<std::shared_ptr<gameComponent>> components;
public:
	gameObject();
	~gameObject();

	glm::vec3 position;
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	bool enableRelatives = false;
	glm::vec3 relativePos;
	glm::vec3 relativeRotation;


	void addComponent(std::shared_ptr<gameComponent> component);
	std::list<std::shared_ptr<gameComponent>> getComponents();

	void draw(const glm::mat4 & = glm::mat4(1.0f));
	void update(float elapsedTime);


	template<class T>
	std::shared_ptr<T> getComponent()
	{
		for (auto c : components)
		{
			std::shared_ptr<T> t = dynamic_pointer_cast<T>(c);
			if (t)
				return t;
		}
		return nullptr;
	}

	void removeComponent(const std::shared_ptr<gameComponent>& component)
	{
		for (auto& c : components)
		{
			if (c == component)
			{
				c = nullptr;
			}
		}
		components.erase(std::remove(components.begin(), components.end(), nullptr), components.end());
	}
};
