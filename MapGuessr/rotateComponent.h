#pragma once
#include "gameComponent.h"
#include "gameObject.h"

class rotateComponent : public gameComponent{
private:
	gameObject* obj;
	float speed;
public:
	rotateComponent(gameObject* obj, float speed);
	~rotateComponent();
	virtual void update(float elapsedTime) override;
};