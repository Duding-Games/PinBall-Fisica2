#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

class ModuleMenu : public Module
{
public:
	ModuleMenu(Application* app, bool start_enabled = true);
	virtual ~ModuleMenu();

	bool Start();
	update_status Update();
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void OnExitCollision(PhysBody* bodyA, PhysBody* bodyB);

	SDL_Texture* menuScreen;
	SDL_Texture* playButton;

};