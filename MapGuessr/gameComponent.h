#pragma once
#include "glm/glm.hpp"


class gameComponent  {
public : 
	/**
	 * @brief Interface for the update method for component
	 * 
	 * @param timeElapsed This is the time passed since last frame update.
	*/
	virtual void update(float timeElapsed) {};

	/**
	 * @brief Constructor and destructor of the gameComponent
	*/
	gameComponent();
	~gameComponent();
};
