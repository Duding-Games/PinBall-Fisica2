#include "Globals.h"
#include "Application.h"
#include "ModuleMenu.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
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

	

	LOG("Loading menu");
	return true;

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



// Update: draw background
update_status ModuleMenu::Update()
{

	return UPDATE_CONTINUE;
}


