#pragma once
#include "gameComponent.h"
#include "gameObject.h"

class rotateComponent : public gameComponent{
private:
	std::shared_ptr<gameObject> obj;
	float speed;
public:
	rotateComponent(std::shared_ptr<gameObject> obj, float speed);
	~rotateComponent();
	virtual void update(float elapsedTime) override;
};