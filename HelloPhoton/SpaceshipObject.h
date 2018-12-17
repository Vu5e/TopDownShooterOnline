#ifndef _SPACESHIPOBJECT_H
#define _SPACESHIPOBJECT_H

#include "GameObject.h"
#include "ObjectPool.h"
#include "BulletObject.h"
#include "MissileObject.h"
using namespace objectpool;

class Spaceship : public GameObject
{
private:
	float m_ShootCooldownTimer;
	float m_MissileCooldownTimer;

public:

	int Health = 3;

	float ShootCooldown = 0.3f;
	float MissileCooldown = 2.0f;

	Spaceship();

	void update(double elapsedTime) override;

	void TakeDamage(int damage);

	void Shoot(Vector target);

	void FireMissile(Vector target);
};

#endif // !_SPACESHIPOBJECT_H


#pragma once
