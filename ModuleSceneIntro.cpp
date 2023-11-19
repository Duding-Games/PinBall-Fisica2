#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleFadeToBlack.h"
#include "ModuleGameOver.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	App->player->Enable();
	App->renderer->camera.x = App->renderer->camera.y = 0;

	map = App->textures->Load("pinball/Pinball.png");
	flipper = App->textures->Load("pinball/paleta.png");
	life = App->textures->Load("pinball/player.png");
	muelle = App->textures->Load("pinball/muelle.png");
	//Audios
	App->audio->PlayMusic("pinball/Audios/Music/gameplayMusic.ogg");
	bonus_fx = App->audio->LoadFx("pinball/Audios/Fx/bonus.wav");
	muelle_fx = App->audio->LoadFx("pinball/Audios/Fx/muelle.wav");
	boost_fx = App->audio->LoadFx("pinball/Audios/Fx/boost.wav");
	flipper_fx = App->audio->LoadFx("pinball/Audios/Fx/flipper.wav");

	lives = 3;

	mapColliders.clear();
	/*sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);*/

	LoadMapCollisions();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->player->Disable();
	App->physics->GetWorld()->DestroyBody(R_Paleta->body);
	App->physics->GetWorld()->DestroyBody(R_PaletaPoint->body);
	App->physics->GetWorld()->DestroyBody(L_Paleta->body);
	App->physics->GetWorld()->DestroyBody(L_PaletaPoint->body);
	App->physics->GetWorld()->DestroyBody(spring->body);
	App->physics->GetWorld()->DestroyBody(springPoint->body);

	for (int i = 0; i < mapColliders.count(); i++)
	{
		PhysBody* body;
		mapColliders.at(i, body);
		App->physics->GetWorld()->DestroyBody(body->body);
	}
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//draw map 
	App->renderer->Blit(map, 0, 0);
	SDL_Rect rect = { 0,0,66,14 };
	App->renderer->Blit(flipper, METERS_TO_PIXELS(R_Paleta->body->GetPosition().x - offsetX), METERS_TO_PIXELS(R_Paleta->body->GetPosition().y - offsetY), &rect, SDL_FLIP_NONE, 1, R_Paleta->GetRotation());
	App->renderer->Blit(flipper, METERS_TO_PIXELS(L_Paleta->body->GetPosition().x - offsetX), METERS_TO_PIXELS(L_Paleta->body->GetPosition().y - offsetY), &rect, SDL_FLIP_HORIZONTAL, 1, L_Paleta->GetRotation());
	App->renderer->Blit(muelle, METERS_TO_PIXELS(spring->body->GetPosition().x -12), METERS_TO_PIXELS(spring->body->GetPosition().y- 5));

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		if (springForce < 150) {
			springForce += 5;
		}

		spring->body->ApplyForceToCenter(b2Vec2(0, springForce), 1);

	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
		springForce = 0;
		App->audio->PlayFx(muelle_fx);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		R_Paleta->body->ApplyForceToCenter(b2Vec2(0, -50), 1);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
		App->audio->PlayFx(flipper_fx);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		L_Paleta->body->ApplyForceToCenter(b2Vec2(0, -50), 1);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
		App->audio->PlayFx(flipper_fx);
	}
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		if (App->player->pbody->body->GetFixtureList()->GetRestitution() < 1) {
			float res = App->player->pbody->body->GetFixtureList()->GetRestitution();
			App->player->pbody->body->GetFixtureList()->SetRestitution(res += 0.1f);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		if (App->player->pbody->body->GetFixtureList()->GetRestitution() > 0) {
			float res = App->player->pbody->body->GetFixtureList()->GetRestitution();
			App->player->pbody->body->GetFixtureList()->SetRestitution(res -= 0.1f);
		}
	}


	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
		lives = 0;
	}
	if (lives >= 3) {
		App->renderer->Blit(life, 20, 660);
	}
	if (lives >= 2) {
		App->renderer->Blit(life, 20, 680);
	}
	if (lives >= 1) {
		App->renderer->Blit(life, 20, 700);
	}

	if (lives <= 0) {
		App->fadeToBlack->FadeToBlack(this, App->scene_gameOver);
	}

	LOG("Restituition: ------------------------------- %f", App->player->pbody->body->GetFixtureList()->GetRestitution());
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);
}

void ModuleSceneIntro::LoadMapCollisions()
{
	// spring
	spring = App->physics->CreateRectangle(504, 700, 24, 10, b2_dynamicBody);
	spring->body->SetFixedRotation(true);
	spring->type = ColliderType::POINT;



	springPoint = App->physics->CreateCircle(500, 710, 3, ColliderType::POINT, b2_staticBody, 0);

	springPoint->body->SetType(b2_staticBody);

	b2DistanceJointDef springDef;

	springDef.bodyA = spring->body;
	springDef.bodyB = springPoint->body;

	springDef.localAnchorA.Set(0, 0);
	springDef.localAnchorB.Set(0, 0);

	springDef.length = 1.5f;

	springDef.collideConnected = true;

	springDef.frequencyHz = 7.0f;
	springDef.dampingRatio = 0.05f;

	b2PrismaticJoint* springJoint = (b2PrismaticJoint*)App->physics->GetWorld()->CreateJoint(&springDef);

	// paleta R
	R_Paleta = App->physics->CreateRectangle(293, 740, 70, 10, b2_dynamicBody);
	R_Paleta->body->SetFixedRotation(false);
	R_Paleta->type = ColliderType::UNKNOWN;

	R_PaletaPoint = App->physics->CreateCircle(335, 725, 3, ColliderType::POINT, b2_staticBody, 0);

	b2RevoluteJointDef R_PaletaDef;

	R_PaletaDef.bodyA = R_Paleta->body;
	R_PaletaDef.bodyB = R_PaletaPoint->body;

	R_PaletaDef.referenceAngle = 0 * DEGTORAD;
	R_PaletaDef.enableLimit = true;
	R_PaletaDef.lowerAngle = -25 * DEGTORAD;
	R_PaletaDef.upperAngle = 30 * DEGTORAD;
	R_PaletaDef.localAnchorA.Set(PIXEL_TO_METERS(25), 0);
	R_PaletaDef.localAnchorB.Set(0, 0);

	b2RevoluteJoint* R_PaletaJoint = (b2RevoluteJoint*)App->physics->GetWorld()->CreateJoint(&R_PaletaDef);

	// paleta L
	L_Paleta = App->physics->CreateRectangle(200, 740, 70, 10, b2_dynamicBody);
	L_Paleta->body->SetFixedRotation(false);
	L_Paleta->type = ColliderType::UNKNOWN;

	L_PaletaPoint = App->physics->CreateCircle(195, 725, 3, ColliderType::POINT, b2_staticBody, 0);

	b2RevoluteJointDef L_PaletaDef;

	L_PaletaDef.bodyA = L_Paleta->body;
	L_PaletaDef.bodyB = L_PaletaPoint->body;

	L_PaletaDef.referenceAngle = 0 * DEGTORAD;
	L_PaletaDef.enableLimit = true;
	L_PaletaDef.lowerAngle = -25 * DEGTORAD;
	L_PaletaDef.upperAngle = 30 * DEGTORAD;
	L_PaletaDef.localAnchorA.Set(PIXEL_TO_METERS(-25), 0);
	L_PaletaDef.localAnchorB.Set(0, 0);

	b2RevoluteJoint* L_PaletaJoint = (b2RevoluteJoint*)App->physics->GetWorld()->CreateJoint(&L_PaletaDef);

	
	// circles
	mapColliders.add(App->physics->CreateCircle(178, 225, 30, ColliderType::SPECIAL_METEOR_1, b2_staticBody, 0.7f));
	mapColliders.add(App->physics->CreateCircle(258, 179, 30, ColliderType::SPECIAL_METEOR_2, b2_staticBody, 0.7f));
	mapColliders.add(App->physics->CreateCircle(254, 281, 30, ColliderType::SPECIAL_METEOR_3, b2_staticBody, 0.7f));
	mapColliders.add(App->physics->CreateCircle(239, 437, 15, ColliderType::METEOR, b2_staticBody, 0.7f));
	mapColliders.add(App->physics->CreateCircle(382, 500, 15, ColliderType::METEOR, b2_staticBody, 0.7f));
	mapColliders.add(App->physics->CreateCircle(444, 580, 12, ColliderType::METEOR, b2_staticBody, 0.7f));

	// boost
	mapColliders.add(App->physics->CreateRectangleSensor(75, 420, 40, 280, ColliderType::BOOST));

	// Sensor

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 50, 566, 100, ColliderType::SENSOR);
	mapColliders.add(sensor);



	int background_collision[78] = {
		514, 767,
		514, 262,
		507, 237,
		489, 203,
		467, 173,
		432, 143,
		400, 126,
		358, 110,
		318, 104,
		266, 103,
		229, 106,
		200, 110,
		184, 115,
		160, 161,
		144, 130,
		116, 149,
		90, 175,
		75, 200,
		65, 223,
		59, 250,
		59, 556,
		82, 577,
		74, 585,
		58, 571,
		58, 655,
		76, 721,
		174, 767,
		174, 829,
		355, 829,
		355, 767,
		458, 716,
		478, 643,
		478, 560,
		460, 547,
		449, 547,
		449, 390,
		477, 347,
		489, 347,
		489, 768
	};
	mapColliders.add(App->physics->CreateChain(0, 0, background_collision, 78, b2_staticBody, 0));


	//left slide
	int left_slide[12] = {
		100, 614,
		118, 684,
		185, 717,
		180, 727,
		107, 689,
		89, 617
	};
	mapColliders.add(App->physics->CreateChain(0, 0, left_slide, 12, b2_staticBody, 0.2f));


	// right slide
	int right_slide[12] = {
		347, 717,
		417, 682,
		433, 614,
		445, 617,
		426, 689,
		352, 726
	};
	mapColliders.add(App->physics->CreateChain(0, 0, right_slide, 12, b2_staticBody, 0.2f));


	//left triangle
	int left_triangle[6] = {
		184, 673,
		132, 618,
		151, 671
	};
	mapColliders.add(App->physics->CreateChain(0, 0, left_triangle, 6, b2_staticBody, 0.2f));


	//right triangle
	int right_triangle[6] = {
		349, 674,
		402, 617,
		383, 670
	};
	mapColliders.add(App->physics->CreateChain(0, 0, right_triangle, 6, b2_staticBody, 0.2f));


	//center arrow
	int center_arrow[12] = {
		245, 588,
		266, 566,
		287, 587,
		279, 594,
		266, 581,
		253, 594
	};
	mapColliders.add(App->physics->CreateChain(0, 0, center_arrow, 12, b2_staticBody, 0.2f));


	//trapezium
	int trapezium[10] = {
		91, 290,
		105, 296,
		189, 356,
		190, 456,
		91, 515
	};
	mapColliders.add(App->physics->CreateChain(0, 0, trapezium, 10, b2_staticBody, 0.4f));


	//center rectangle
	int center_rectangle[8] = {
		275, 506,
		334, 425,
		344, 432,
		285, 513
	};
	mapColliders.add(App->physics->CreateChain(0, 0, center_rectangle, 8, b2_staticBody, 0.4f));


	//mini rect1
	int mini_rect1[16] = {
		297, 380,
		297, 346,
		300, 342,
		305, 342,
		308, 346,
		308, 380,
		305, 383,
		301, 383
	};
	mapColliders.add(App->physics->CreateChain(0, 0, mini_rect1, 16, b2_staticBody, 0.7f));


	//mini rect2
	int mini_rect2[16] = {
		334, 379,
		334, 345,
		337, 342,
		342, 342,
		345, 345,
		345, 379,
		342, 383,
		337, 383
	};
	mapColliders.add(App->physics->CreateChain(0, 0, mini_rect2, 16, b2_staticBody, 0.7f));


	//mini rect3
	int mini_rect3[16] = {
		371, 380,
		371, 345,
		374, 342,
		379, 342,
		381, 345,
		381, 379,
		379, 383,
		374, 383
	};
	mapColliders.add(App->physics->CreateChain(0, 0, mini_rect3, 16, b2_staticBody, 0.7f));


	//mini rect4
	int mini_rect4[16] = {
		408, 380,
		408, 345,
		411, 342,
		416, 342,
		419, 345,
		419, 380,
		416, 383,
		412, 383
	};
	mapColliders.add(App->physics->CreateChain(0, 0, mini_rect4, 16, b2_staticBody, 0.7f));

}
