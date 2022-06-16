#pragma once
#include "glm/glm.hpp"
#include "gameObject.h"


class gameComponent  {
private:
	gameObject* owner;
public : 
	/**
	 * @brief Interface for the update method for component
	 * 
	 * @param timeElapsed This is the time passed since last frame update.
	*/
	virtual void update(float timeElapsed) {};
	void setOwner(gameObject* o);

	/**
	 * @brief Constructor and destructor of the gameComponent
	*/
	gameComponent();
	~gameComponent();
};
