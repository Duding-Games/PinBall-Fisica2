#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void OnExitCollision(PhysBody* bodyA, PhysBody* bodyB);

	struct Player {
		int x = 500;
		int y = 600;
		int radius = 8;
	};

	Player player;

public:
	PhysBody* pbody;
	bool resetBall = false;
	bool isInside = false;
	bool boost_points_1 = false;
	bool boost_points_2 = false;
	bool boost_points_3 = false;
	uint boost_fx;
	uint lose_fx;
	uint score = 000;
	uint prevScore = 000;
	uint highScore = 000;
	SDL_Texture* ptex;
	int scoreFont = -1;
	int offset = 8;
	char scoreText[10] = { "\0" };

	 
};