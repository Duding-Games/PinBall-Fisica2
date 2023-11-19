#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"


class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void LoadMapCollisions();

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* spring;
	PhysBody* springPoint;
	PhysBody* sensor;
	PhysBody* R_Paleta;
	PhysBody* R_PaletaPoint;
	PhysBody* L_Paleta;
	PhysBody* L_PaletaPoint;
	bool sensed;
	float springForce = 0;

	SDL_Texture* map;
	SDL_Texture* flipper;
	SDL_Texture* life;
	SDL_Texture* muelle;
	SDL_Texture* scoreText;
	uint bonus_fx;
	uint muelle_fx;
	uint boost_fx;
	uint flipper_fx;
	p2Point<int> ray;
	bool ray_on;

	int lives = 3;
	int offsetX = 33;
	int offsetY = 7;

	p2List<PhysBody*> mapColliders;

};
