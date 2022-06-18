#pragma once
#include "drawComponent.h"
#include "tileBuilder.h"
#include "tigl.h"

#define FLOOR_HEIGHT -0.1f

class floorComponent : public drawComponent {
private:
	std::vector<tigl::Vertex> vertices;
public:
	floorComponent(tileBuilder::tileZone zone);
	~floorComponent();

	virtual void draw() override;
};