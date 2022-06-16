#pragma once
#include "gameComponent.h"

class drawComponent : public gameComponent {
public:
	drawComponent();
	~drawComponent();
	virtual void draw() = 0;
};