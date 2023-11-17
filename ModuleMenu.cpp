#include "Globals.h"
#include "Application.h"
#include "ModuleMenu.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL_scancode.h"

ModuleMenu::ModuleMenu(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleMenu::~ModuleMenu()
{}

// Load assets
bool ModuleMenu::Start()
{
	App->textures->Load("pinball/menuScreen.png");
	

	LOG("Loading menu");
	return true;

}
// Update: draw background
update_status ModuleMenu::Update()
{
	App->renderer->Blit(menuScreen, 0, 0);
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModuleMenu::CleanUp()
{
	LOG("Unloading player");

	return true;
}

void ModuleMenu::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	
}

void ModuleMenu::OnExitCollision(PhysBody* bodyA, PhysBody* bodyB)
{

}





