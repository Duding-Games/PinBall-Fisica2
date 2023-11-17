#include "Globals.h"
#include "Application.h"
#include "ModuleGameOver.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL_scancode.h"

ModuleGameOver::ModuleGameOver(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleGameOver::~ModuleGameOver()
{}

// Load assets
bool ModuleGameOver::Start()
{
	gameOverScreen = App->textures->Load("pinball/GameOver.png");
	LOG("Loading game over");
	return true;

}
// Update: draw background
update_status ModuleGameOver::Update()
{
	App->renderer->Blit(gameOverScreen, 0, 0);

	return UPDATE_CONTINUE;
}

// Unload assets
bool ModuleGameOver::CleanUp()
{
	LOG("Unloading game over");

	return true;
}

void ModuleGameOver::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

}

void ModuleGameOver::OnExitCollision(PhysBody* bodyA, PhysBody* bodyB)
{

}
