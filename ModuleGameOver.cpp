#include "Globals.h"
#include "Application.h"
#include "ModuleGameOver.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleAudio.h"
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
	App->audio->PlayMusic("pinball/Audios/Music/gameOverMusic.ogg");
	char lookupTable[] = { "0123456789" };
	scoreFont = App->fonts->Load("pinball/NumsPinball.png", lookupTable, 1);
	playAgainButtons = App->textures->Load("pinball/playagainButtons.png");

	LOG("Loading game over");
	return true;

}
// Update: draw background
update_status ModuleGameOver::Update()
{
	App->renderer->Blit(gameOverScreen, 0, 0);
	SDL_Rect rect = { 152,412,265, 140 };
	App->renderer->Blit(playAgainButtons, 150, 100, &rect);

	sprintf_s(scoreText, 10, "%d", App->player->prevScore);
	App->fonts->BlitText(291, 548, scoreFont, scoreText);

	sprintf_s(scoreText, 10, "%d", App->player->highScore);
	App->fonts->BlitText(337, 617, scoreFont, scoreText);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		SDL_Rect rect = { 152,200,265, 140 };
		App->renderer->Blit(playAgainButtons, 150, 100, &rect);
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
		App->fadeToBlack->FadeToBlack(App->scene_gameOver, (Module*)App->scene_menu, 100.0f);
	}

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
