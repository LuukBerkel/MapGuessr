#pragma once
#include "drawComponent.h"
#include "tileBuilder.h"
#include "tigl.h"

#define FLOOR_HEIGHT -1.0f

class floorComponent : public drawComponent {
private:
	tigl::VBO* vbo;
public:
	floorComponent(tileBuilder::tileZone zone);
	~floorComponent();

	virtual void draw() override;
};