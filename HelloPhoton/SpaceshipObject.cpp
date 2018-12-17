#include "SpaceshipObject.h"
#include "Application.h"

Spaceship::Spaceship()
{
	m_ShootCooldownTimer = ShootCooldown;
	m_MissileCooldownTimer = MissileCooldown; //New addition
	m_restrictMovement = true;
}

void Spaceship::update(double elapsedTime)
{
	GameObject::update(elapsedTime);
	m_ShootCooldownTimer += elapsedTime;
	m_MissileCooldownTimer += elapsedTime; //New addition
}

void Spaceship::TakeDamage(int damage)
{
	//Ouch!
}

void Spaceship::Shoot(Vector target)
{
	if (m_ShootCooldownTimer >= ShootCooldown)
	{
		m_ShootCooldownTimer = 0;

		Vector dir = target - m_position;
		dir.normalize();

		Bullet* bullet = new Bullet();
		bullet->setPos(m_position);
		bullet->setVelocity(dir * 1000);
		bullet->Owner = Application::getInstance().PlayerID;
		Application::getInstance().MyBulletList.push_back(bullet);

		Application::getInstance().sendShootCommand(1, dir);
	}	
}

//New addition
void Spaceship::FireMissile(Vector target)
{
	if (m_MissileCooldownTimer >= MissileCooldown)
	{
		m_MissileCooldownTimer = 0.0f;

		Vector dir = target - m_position;
		dir.normalize();

		Missile* missile = new Missile(dir);
		missile->setPos(m_position);
		missile->setVelocity(dir * missile->ThrustSpeed);
		missile->Owner = Application::getInstance().PlayerID;
		Application::getInstance().MyMissileList.push_back(missile);

		Application::getInstance().sendMissileCommand(4, dir);
	}
}
