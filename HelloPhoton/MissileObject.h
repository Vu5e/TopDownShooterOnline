#pragma once

#include "SpaceshipObject.h"
#include "GameObject.h"

//New addition
class Missile : public GameObject
{
private:

public:
	int IsAlive;

	int Owner;

	float Life;
	//Duration that ignores turning rotation(optional)
	float forwardThrustTimer;

	//Current missile direction
	Vector currDir;

	//Forward thrust speed
	float ThrustSpeed;
	//Limit for rotation..this is to prevent snapping
	float MaxRotation;


	Missile(Vector dir);

	void update(double elapsedTime) override;

	void setMissileRotation();
};
