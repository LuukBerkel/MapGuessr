#pragma once
#include "gameComponent.h"

class drawComponent : gameComponent {
public:
	drawComponent();
	~drawComponent();
	virtual void draw() = 0;
};