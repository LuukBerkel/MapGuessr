#pragma once
#include "drawComponent.h"
#include "tileBuilder.h"
#include "tigl.h"

#define FLOOR_HEIGHT -1.0f

class floorComponent : public drawComponent {
private:
	std::vector<tigl::Vertex> vertices;
public:
	floorComponent(std::shared_ptr<tileBuilder::tileZone> zones);
	~floorComponent();

	virtual void draw() override;
};