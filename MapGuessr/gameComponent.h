#pragma once
#include "glm/glm.hpp"
#include "gameBehaviour.h"
#include <vector>
#include <memory>

class gameComponent  {
	std::vector<std::shared_ptr<gameBehaviour>> behaviours; 

public : 
	glm::vec3 position = glm::vec3(0,0,100);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(0.1f, 0.1f, 0.1f);

	/**
	 * @brief Interface of the draw method for component
	*/
	virtual void draw() = 0;

	/**
	 * @brief Interface for the update method for component
	 * 
	 * @param timeElapsed This is the time passed since last frame update.
	*/
	virtual void update(float timeElapsed);

	/**
	 * @brief This template function collects the specific instance and type of a behaviour based on type
	 * 
	 * @tparam T the type of behaviour
	 * @return a pointer to the instance or nullptr
	*/
	template<class T>
	std::shared_ptr<T> getBehaviour() {
		for (auto c : behaviours) {
			std::shared_ptr<T> t = std::dynamic_pointer_cast<T>(c);
			if (t)
				return t;
		}
	}

	/**
	 * @brief This template function allows you to add a type. This will create a new the pointer.
	 * 
	 * @tparam T the type of behaviour you want to create.
	*/
	template<class T>
	void addBehaviour() {
		std::shared_ptr<gameBehaviour> behaviour = std::make_shared<T>();
		if (behaviour != nullptr) {
			behaviours.push_back(behaviour);
		}
	}

	/**
	 * @brief This functions adds a instance of a behaviour.
	 * 
	 * @param behaviour The instance you want to pass.
	*/
	void addBehaviour(std::shared_ptr<gameBehaviour> behaviour) {
		if (behaviour != nullptr) {
			behaviours.push_back(behaviour);
		}
	}

	gameComponent();
	~gameComponent();
	
};
