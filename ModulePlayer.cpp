#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
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
	boost_fx = App->audio->LoadFx("pinball/Audios/Fx/boost.wav");
	lose_fx = App->audio->LoadFx("pinball/Audios/Fx/losesound.wav");
	char lookupTable[] = { "0123456789" };
	scoreFont = App->fonts->Load("pinball/NumsPinball2.png", lookupTable, 2);
	ptex = App->textures->Load("pinball/player.png");

	LOG("Loading player");
	return true;
	
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	App->physics->GetWorld()->DestroyBody(pbody->body);
	LOG("Unloading player");

	return true;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->type == ColliderType::BALL && bodyB->type == ColliderType::SENSOR) {
		resetBall = true;
		App->audio->PlayFx(lose_fx);
		App->scene_intro->lives--;
		if (App->scene_intro->lives == 0) {
			prevScore = score;
			score = 0;
		}
	}
	else if (bodyA->type == ColliderType::BALL && bodyB->type == ColliderType::BOOST) {
		isInside = true;
		/*score += 5;*/
		App->audio->PlayFx(boost_fx);

	}
	else if (bodyA->type == ColliderType::BALL && bodyB->type == ColliderType::METEOR) {
		score += 1;
		App->audio->PlayFx(App->scene_intro->bonus_fx);
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

	App->renderer->Blit(ptex, METERS_TO_PIXELS(pbody->body->GetPosition().x - offset), METERS_TO_PIXELS(pbody->body->GetPosition().y - offset));

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

	if (score > highScore) highScore = score;
	
	sprintf_s(scoreText, 10, "%d", score);
	App->fonts->BlitText(10, 5, scoreFont, scoreText);
	sprintf_s(scoreText, 10, "%d", prevScore);
	App->fonts->BlitText(510, 5, scoreFont, scoreText);

	return UPDATE_CONTINUE;
}


