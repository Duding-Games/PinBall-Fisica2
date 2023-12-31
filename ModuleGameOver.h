#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

class ModuleGameOver : public Module
{
public:
	ModuleGameOver(Application* app, bool start_enabled = true);
	virtual ~ModuleGameOver();

	bool Start();
	update_status Update();
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void OnExitCollision(PhysBody* bodyA, PhysBody* bodyB);

	SDL_Texture* gameOverScreen;
	SDL_Texture* playAgainButtons;
	int scoreFont = -1;
	char scoreText[10] = { "\0" };

};
