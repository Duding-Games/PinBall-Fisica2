#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "SDL/include/SDL_scancode.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	score = 0;

	pbody = App->physics->CreateCircle(player.x, player.y, player.radius, ColliderType::BALL, b2_dynamicBody, 0.1f);
	pbody->listener = this;
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	char lookupTable[] = { "0123456789" };
	scoreFont = App->fonts->Load("pinball/NumsPinball.png", lookupTable, 2);

	LOG("Loading player");
	return true;
	
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->type == ColliderType::BALL && bodyB->type == ColliderType::SENSOR) {
		resetBall = true;	
	}
	else if (bodyA->type == ColliderType::BALL && bodyB->type == ColliderType::BOOST) {
		isInside = true;
		score += 100;
		App->audio->PlayFx(bonus_fx);

	}
	else if (bodyA->type == ColliderType::BALL && bodyB->type == ColliderType::UNKNOWN) {
		score += 1;
	}
}

void ModulePlayer::OnExitCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->type == ColliderType::BALL && bodyB->type == ColliderType::BOOST) {
		isInside = false;
	}
}



// Update: draw background
update_status ModulePlayer::Update()
{

	if (resetBall) {
		pbody->body->GetWorld()->DestroyBody(pbody->body);
		pbody = App->physics->CreateCircle(player.x, player.y, player.radius, ColliderType::BALL, b2_dynamicBody, 0.1f);
		pbody->listener = this;

		resetBall = false;
	}


	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		pbody->body->GetWorld()->DestroyBody(pbody->body);

		pbody = App->physics->CreateCircle(player.x, player.y, player.radius, ColliderType::BALL, b2_dynamicBody, 0.1f);

		pbody->listener = this;
	}

	if (isInside) {
		pbody->body->ApplyForceToCenter(b2Vec2(0.05f, -2), 1);
	}
	
	sprintf_s(scoreText, 10, "%d", score);
	App->fonts->BlitText(10, 10, scoreFont, scoreText);

	return UPDATE_CONTINUE;
}


