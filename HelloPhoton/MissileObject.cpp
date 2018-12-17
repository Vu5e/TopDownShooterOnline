#include "MissileObject.h"
#include "Application.h"
#include <math.h>

# define PI		3.141592653589793238462643383279502884

//New addition
Missile::Missile(Vector dir)
{
	//Warning; modify this sprite
	if (Application::getInstance().BulletSprite != nullptr)
	{
		this->m_sprite = Application::getInstance().BulletSprite;
	}

	IsAlive = true;
	m_scale = Vector(0.1, 0.1, 0.1);
	Life = 3.0f;

	//Duration that ignores turning rotation(optional)
	forwardThrustTimer = 0.25f;

	
	currDir = dir;
	
	ThrustSpeed = 500.0f;
	//Limit for rotation..this is to prevent snapping
	//Below is conversion from degrees to radian
	MaxRotation = 5.0f / (180.0f * PI);
}

void Missile::update(double elapsedTime)
{

	Life -= elapsedTime;
	forwardThrustTimer -= elapsedTime;

	//Checks rotation towards target to prevent snapping
	if (forwardThrustTimer <= 0.0f)
	{
		setMissileRotation();
	}
	//Moved downward to calculate rotation first
	GameObject::update(elapsedTime);

	if (Life <= 0)
	{
		IsAlive = false;
	}
	else
	{
		float distance = (m_position - Application::getInstance().EnemySpaceship.getPos()).length();

		//Check for collision with ships
		if ((this->m_position - Application::getInstance().EnemySpaceship.getPos()).length() <= 25.0f)
		{
			//If it's this player's bullet
			if (Owner == Application::getInstance().PlayerID)
			{
				std::cout << "Enemy Hit!" << std::endl;
				Application::getInstance().EnemySpaceship.TakeDamage(2);
				IsAlive = false;

				//Only P1 should send damage notify
				if (Application::getInstance().PlayerID == 1)
				{
					Application::getInstance().sendDamageNotify(2, 2);
				}
			}
		}
		else if ((this->m_position - Application::getInstance().MySpaceship.getPos()).length() <= 25.0f)
		{
			//If it's this enemy's bullet
			if (Owner != Application::getInstance().PlayerID)
			{
				std::cout << "I am Hit!" << std::endl;
				Application::getInstance().EnemySpaceship.TakeDamage(2);
				IsAlive = false;

				//Only p1 should send damage notify
				if (Application::getInstance().PlayerID == 1)
				{
					Application::getInstance().sendDamageNotify(1, 2);
				}

			}
		}
	}
}

void Missile::setMissileRotation()
{
	Vector targetDir;

	if(Owner == Application::getInstance().PlayerID)
		targetDir = Application::getInstance().EnemySpaceship.getPos() - m_position;
	else if (Owner != Application::getInstance().PlayerID)
		targetDir = Application::getInstance().MySpaceship.getPos() - m_position;
	targetDir.normalize();

	
	float targetdist = targetDir.length();

	float targetRot = atan2(targetDir.mVal[1], targetDir.mVal[0]);
	if (targetRot < 0.0f)
		targetRot = 2.0f * PI + targetRot;
	float currRot = atan2(currDir.mVal[1], currDir.mVal[0]);
	if (currRot < 0.0f)
		currRot = 2.0f * PI + currRot;


	//Compare if rotation towards target exceeds max rotation
	if (targetRot - currRot > MaxRotation)
	{
		targetRot = currRot + MaxRotation;
		std::cout << "+" << std::endl;
	}
	else if (targetRot - currRot < (-1.0f * MaxRotation))
	{
		targetRot = currRot - MaxRotation;
		std::cout << "-" << std::endl;
	}

	Vector dir = Vector(cos(targetRot) * targetdist, sin(targetRot) * targetdist, 0.0f);

	setVelocity(dir * ThrustSpeed);

	currDir = getVelocity();
}
