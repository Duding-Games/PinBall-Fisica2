#include "Globals.h"
#include "Application.h"
#include "ModuleMenu.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL_scancode.h"

ModuleMenu::ModuleMenu(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleMenu::~ModuleMenu()
{}

// Load assets
bool ModuleMenu::Start()
{
	menuScreen = App->textures->Load("pinball/menuScreen.png");
	playButton = App->textures->Load("pinball/playButtons.png");

	LOG("Loading menu");
	return true;

}
// Update: draw background
update_status ModuleMenu::Update()
{
	App->renderer->Blit(menuScreen, 0, 0);
	SDL_Rect rect = { 152,414,270,135 };
	App->renderer->Blit(playButton, 150, 500, &rect);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		SDL_Rect rect = { 152,205,270,135 };
		App->renderer->Blit(playButton, 150, 500, &rect);
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
		App->fadeToBlack->FadeToBlack(App->scene_menu, (Module*)App->scene_intro, 20.0f);
	}
	
	
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





