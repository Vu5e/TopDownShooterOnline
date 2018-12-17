#pragma once
#include "Sprite.h"
#include "GameObject.h"
#include "BulletObject.h"
#include "MissileObject.h" //New addition
#include "SpaceshipObject.h"

#include <list>

class Application
{
protected:
	int m_method;

	Vector m_lastReceivedPos;
	double m_prevReceivedTime;
	bool m_gameStarted;
	Vector m_mousePos;

	void networkUpdate();

	void limitVelAndPos(GameObject* go);

public:
	
	int			PlayerID;

	Sprite*		P1Spaceship;
	Sprite*		P2Spaceship;
	Sprite*		BulletSprite;
	Spaceship	MySpaceship;
	Spaceship	EnemySpaceship;
	GameObject	m_object3;

	std::list<Bullet*> MyBulletList;
	std::list<Missile*> MyMissileList; //New addition

	std::list<Bullet*> EnemyBulletList;
	std::list<Missile*> EnemyMissileList; //New addition

	Application();
	~Application();

	static Application& getInstance()
	{
		static Application app;
		return app;
	}

	void sendMyData(void);
	void sendShootCommand(int command, Vector dir);
	void sendMissileCommand(int command, Vector dir); //New addition
	void sendDamageNotify(int playerID, int damage);

	void OnReceivedOpponentData(float* data);
	void OnReceivedOpponentCommand(float* data);
	void OnReceivedDamageNotify(int* data);
	void OnReceivedOpponentMissile(float* data); //New addition

	virtual void start();	
	virtual void update(double elapsedTime);
	virtual void draw();
	virtual void onKeyPressed(int key);
	virtual void onKeyReleased(int key);
	virtual void onKeyHold(int key);
	virtual void onMousePressed(int button);
	virtual void onMouseReleased(int button);
	virtual void onMouseHold(int button);
	virtual void onMouseMoved(double mousePosX, double mousePosY);
};

