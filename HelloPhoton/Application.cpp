#include "Application.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "MyPhoton.h"

using namespace std;

// The higher networkFPS, the higher bandwidth requirement of your game.
// ** about 30FPS is common data sync rate for real time games
// ** slower-paced action games could use as low as 10 FPS

const float networkFPS = 60.0f;
const float gNetworkFrameTime = 1.0f/networkFPS;

Application::Application ()
{
	PlayerID = -1;
}

Application::~Application ()
{
	m_method = 3;
}

void Application::start ()
{
	srand(time(0));

	m_gameStarted = false;
	
	MyPhoton::getInstance().connect();

	P1Spaceship = new Sprite ("../media/Spaceship1.png");
	P2Spaceship = new Sprite("../media/Spaceship2.png");
	BulletSprite = new Sprite("../media/Particle.png");

	MySpaceship.setSprite(P1Spaceship);
	bool validSpot = false;
	do
	{
		MySpaceship.setPos(Vector(rand() % 800, rand() % 600, 0));

		if (MySpaceship.getPos().mVal[0] > 25 && MySpaceship.getPos().mVal[0] < 775 &&
			MySpaceship.getPos().mVal[1] > 25 && MySpaceship.getPos().mVal[1] < 575)
		{
			validSpot = true;
		}
	} while (!validSpot);

	MySpaceship.setBlendMode(ADDITIVE);
	MySpaceship.setScale(Vector(0.1, 0.1, 0.1));
	MySpaceship.setRotation(0);

	EnemySpaceship.setSprite(P1Spaceship);
	EnemySpaceship.setPos(Vector(200, 200, 0));
	EnemySpaceship.setBlendMode(ADDITIVE);
	EnemySpaceship.setScale(Vector(0.1, 0.1, 0.1));
	EnemySpaceship.setRotation(0);

	m_lastReceivedPos = EnemySpaceship.getPos();
}

void Application::sendMyData(void)
{
	Bullet* t_myBullet[100];
	int myBulletCount = 0;

	list<Bullet*>::iterator iter = MyBulletList.begin();
	while (iter != MyBulletList.end())
	{
		Bullet* bullet = *iter;

		iter++;
	}

	MyPhoton::getInstance().sendMyData(MySpaceship, t_myBullet, myBulletCount);
}

void Application::sendShootCommand(int command, Vector dir)
{
	MyPhoton::getInstance().sendMyShot(PlayerID, command, dir);
}

void Application::sendMissileCommand(int command, Vector dir)
{
	MyPhoton::getInstance().sendMyMissile(PlayerID, command, dir);
}

void Application::sendDamageNotify(int playerID, int damage)
{
	if (playerID == 1)
	{
		std::cout << "(Sent) Player " << playerID  << " damaged for " << playerID << " damage" << std::endl;
	}
	else if (playerID == 2)
	{
		std::cout << "(Sent) Player " << playerID << " damaged for " << playerID << " damage" << std::endl;
	}
	MyPhoton::getInstance().sendDamageNotify(playerID, damage);
}

void Application::networkUpdate()
{
	static double prevTime = glfwGetTime();
	
	double time = glfwGetTime();
	if(time - prevTime >= gNetworkFrameTime) 
	{
		sendMyData();
		prevTime = time;
	}
}

void Application::limitVelAndPos(GameObject* go)
{
	if(go->getVelocity().length() > 400.0f)
	{
		Vector vec = go->getVelocity();
		vec.normalize();
		vec *= 400.0f;
		go->setVelocity(vec);
	}
}

void Application::update (double elapsedTime)
{
	MyPhoton::getInstance().run();

	if(!m_gameStarted)
		return;

	MySpaceship.update(elapsedTime);
	MySpaceship.setAcceleration(Vector(0.0f, 0.0f, 0.0f));
	limitVelAndPos(&MySpaceship);

	if (MyBulletList.size() > 0)
	{
		list<Bullet*>::iterator iter = MyBulletList.begin();
		while (iter != MyBulletList.end())
		{
			Bullet* bullet = *iter;

			bullet->update(elapsedTime);

			if (!bullet->IsAlive)
			{
				Bullet* t_b = *iter;
				iter = MyBulletList.erase(iter);
				delete t_b;
			}
			else
			{
				iter++;
			}
		}
	}
	//New addition
	if (MyMissileList.size() > 0)
	{
		list<Missile*>::iterator iter = MyMissileList.begin();
		while (iter != MyMissileList.end())
		{
			Missile* missile = *iter;

			missile->update(elapsedTime);

			if (!missile->IsAlive)
			{
				Missile* t_b = *iter;
				iter = MyMissileList.erase(iter);
				delete t_b;
			}
			else
			{
				iter++;
			}
		}
	}

	EnemySpaceship.update(elapsedTime);
	limitVelAndPos(&EnemySpaceship);

	if (EnemyBulletList.size() > 0)
	{
		list<Bullet*>::iterator iter = EnemyBulletList.begin();
		while (iter != EnemyBulletList.end())
		{
			Bullet* bullet = *iter;
			bullet->update(elapsedTime);
			if (!bullet->IsAlive)
			{
				Bullet* t_b = *iter;
				iter = EnemyBulletList.erase(iter);
				delete t_b;
			}
			else
			{
				iter++;
			}
		}
	}
	//New addition
	if (EnemyMissileList.size() > 0)
	{
		list<Missile*>::iterator iter = EnemyMissileList.begin();
		while (iter != EnemyMissileList.end())
		{
			Missile* missile = *iter;

			missile->update(elapsedTime);

			if (!missile->IsAlive)
			{
				Missile* t_b = *iter;
				iter = EnemyMissileList.erase(iter);
				delete t_b;
			}
			else
			{
				iter++;
			}
		}
	}

	networkUpdate();
}

void Application::draw()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(!m_gameStarted)
		return;

	if (MyBulletList.size() > 0)
	{
		for (list<Bullet*>::iterator iter = MyBulletList.begin(); iter != MyBulletList.end(); iter++)
		{
			Bullet* bullet = *iter;
			bullet->draw();
		}
	}

	if (EnemyBulletList.size() > 0)
	{
		for (list<Bullet*>::iterator iter = EnemyBulletList.begin(); iter != EnemyBulletList.end(); iter++)
		{
			Bullet* bullet = *iter;
			bullet->draw();
		}
	}

	//New addition
	if (MyMissileList.size() > 0)
	{
		for (list<Missile*>::iterator iter = MyMissileList.begin(); iter != MyMissileList.end(); iter++)
		{
			Missile* missile = *iter;
			missile->draw();
		}
	}

	if (EnemyMissileList.size() > 0)
	{
		for (list<Missile*>::iterator iter = EnemyMissileList.begin(); iter != EnemyMissileList.end(); iter++)
		{
			Missile* missile = *iter;
			missile->draw();
		}
	}

	MySpaceship.draw();
	EnemySpaceship.draw();

}

void Application::OnReceivedOpponentData(float* data)
{
	if(m_gameStarted == false)
	{
		if (PlayerID == 1)
		{
			MySpaceship.setSprite(P1Spaceship);
			EnemySpaceship.setSprite(P2Spaceship);
		}
		else
		{
			MySpaceship.setSprite(P2Spaceship);
			EnemySpaceship.setSprite(P1Spaceship);
		}

		m_gameStarted = true;
		EnemySpaceship.setPos(Vector(data[0], data[1], 0));
		EnemySpaceship.setVelocity(Vector(data[2], data[3], 0));
		EnemySpaceship.setRotation(data[6]);

		m_lastReceivedPos = EnemySpaceship.getPos();
		m_prevReceivedTime = glfwGetTime();
		return;
	}

	m_lastReceivedPos = Vector(data[0], data[1], 0);
	EnemySpaceship.setVelocity(Vector(data[2], data[3], 0));
	EnemySpaceship.setRotation(data[6]);

	Vector v0 = EnemySpaceship.getPos();
	Vector v1 = m_lastReceivedPos;
	EnemySpaceship.setPos(Vector::lerp(v0, v1, 1/60.0f));
	//Iterate through enemy bullet list
	int counter = 8;
	list<Bullet*>::iterator iter = EnemyBulletList.begin();
}


void Application::OnReceivedOpponentCommand(float* data)
{
	if ((int)data[1] <= 1)
	{
		Vector dir = Vector(data[2], data[3], 0);
		Bullet* bullet = new Bullet();
		bullet->setPos(EnemySpaceship.getPos());
		bullet->setVelocity(dir * 1000);
		bullet->Owner = (int)data[0];
		Application::getInstance().EnemyBulletList.push_back(bullet);
	}
}

void Application::OnReceivedDamageNotify(int * data)
{
	if (data[0] == 1)
	{
		std::cout << "Player " << data[0] << " damaged for " << data[1] << " damage" << std::endl;
	}
	else if (data[0] == 2)
	{
		std::cout << "Player " << data[0] << " damaged for " << data[1] << " damage" << std::endl;
	}
}
//New addition
void Application::OnReceivedOpponentMissile(float * data)
{
	if ((int)data[1] == 4)
	{
		Vector dir = Vector(data[2], data[3], 0);
		Missile* missile = new Missile(dir);
		missile->setPos(EnemySpaceship.getPos());
		missile->setVelocity(dir * missile->ThrustSpeed);
		missile->Owner = (int)data[0];
		Application::getInstance().EnemyMissileList.push_back(missile);
	}
}

void Application::onKeyPressed (int key)
{

}

void Application::onKeyReleased (int key)
{
}

void Application::onKeyHold (int key)
{
	if(!m_gameStarted)
		return;

	float speed = 500.0f;
	
	if (key == GLFW_KEY_W)
	{
			MySpaceship.setAcceleration(MySpaceship.getAcceleration()+Vector(0.0f, speed, 0.0f));
	}
	if(key == GLFW_KEY_A)
	{
			MySpaceship.setAcceleration(MySpaceship.getAcceleration()+Vector(-speed, 0.0f, 0.0f));
	}
	if(key == GLFW_KEY_S)
	{
			MySpaceship.setAcceleration(MySpaceship.getAcceleration()+Vector(0.0f, -speed, 0.0f));
	}
	if(key == GLFW_KEY_D)
	{
			MySpaceship.setAcceleration(MySpaceship.getAcceleration()+Vector(speed, 0.0f, 0.0f));
	}
	if (key == GLFW_KEY_SPACE)
	{
			//Fire homing missile
			MySpaceship.FireMissile(m_mousePos);
	}
}

void Application::onMousePressed (int button)
{
}

void Application::onMouseReleased (int button)
{

}

void Application::onMouseHold (int button)
{
	if (button == GLFW_MOUSE_BUTTON_1)
	{
		if (m_gameStarted)
		{
			MySpaceship.Shoot(m_mousePos);
		}
	}
}

void Application::onMouseMoved (double mousePosX, double mousePosY)
{
	double rad = atan2(mousePosY - MySpaceship.getPos().mVal[1], mousePosX - MySpaceship.getPos().mVal[0]);

	MySpaceship.setRotation(rad - 1.5708);

	m_mousePos = Vector(mousePosX, mousePosY, 0);
}
