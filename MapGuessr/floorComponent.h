#pragma once
#include "drawComponent.h"
#include "tileBuilder.h"
#include "tigl.h"

#define FLOOR_HEIGHT -0.2f

class floorComponent : public drawComponent {
public:
	std::vector<tigl::Vertex> vertices;

	floorComponent(std::shared_ptr<tileBuilder::tileZone> zones);
	floorComponent();
	~floorComponent();

	virtual void draw() override;
};