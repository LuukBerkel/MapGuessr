#include "gameComponent.h"

void gameComponent::update(float timeElapsed)
{
	// Updating behaviours for for example moving animations and so on.
	for (auto behaviour : behaviours) {
		behaviour->update(timeElapsed);
	}
}
