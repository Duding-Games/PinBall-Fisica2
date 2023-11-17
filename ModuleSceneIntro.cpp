#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
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

	circle = App->textures->Load("pinball/wheel.png");
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	map = App->textures->Load("pinball/Pinball.png");


	/*sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);*/

	LoadMapCollisions();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->player->Disable();
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//draw map
	App->renderer->Blit(map, 0, 0);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		if (springForce < 150) {
			springForce += 5;
		}

		spring->body->ApplyForceToCenter(b2Vec2(0, springForce), 1);


	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
		springForce = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		R_Paleta->body->ApplyForceToCenter(b2Vec2(0, -50), 1);
	}

	

	//if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	//{
	//	ray_on = !ray_on;
	//	ray.x = App->input->GetMouseX();
	//	ray.y = App->input->GetMouseY();
	//}

	//if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	//{
	//	circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
	//	circles.getLast()->data->listener = this;
	//}

	//if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	//{
	//	boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	//}


	// Prepare for raycast ------------------------------------------------------

	//iPoint mouse;
	//mouse.x = App->input->GetMouseX();
	//mouse.y = App->input->GetMouseY();
	//int ray_hit = ray.DistanceTo(mouse);

	//fVector normal(0.0f, 0.0f);

	//// All draw functions ------------------------------------------------------
	//p2List_item<PhysBody*>* c = circles.getFirst();

	//while (c != NULL)
	//{
	//	int x, y;
	//	c->data->GetPosition(x, y);
	//	if (c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
	//		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
	//	c = c->next;
	//}

	//c = boxes.getFirst();

	//while (c != NULL)
	//{
	//	int x, y;
	//	c->data->GetPosition(x, y);
	//	App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
	//	if (ray_on)
	//	{
	//		int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
	//		if (hit >= 0)
	//			ray_hit = hit;
	//	}
	//	c = c->next;
	//}

	//c = ricks.getFirst();

	//while (c != NULL)
	//{
	//	int x, y;
	//	c->data->GetPosition(x, y);
	//	App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
	//	c = c->next;
	//}

  //// ray -----------------
	//if (ray_on == true)
	//{
	//	fVector destination(mouse.x - ray.x, mouse.y - ray.y);
	//	destination.Normalize();
	//	destination *= ray_hit;

	//	App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

	//	if (normal.x != 0.0f)
	//		App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	//}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);







	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
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
	R_Paleta = App->physics->CreateRectangle(293, 740, 45, 10, b2_dynamicBody);
	R_Paleta->body->SetFixedRotation(false);
	R_Paleta->type = ColliderType::UNKNOWN;

	R_PaletaPoint = App->physics->CreateCircle(310, 740, 3, ColliderType::POINT, b2_staticBody, 0);

	b2RevoluteJointDef R_PaletaDef;

	R_PaletaDef.bodyA = R_Paleta->body;
	R_PaletaDef.bodyB = R_PaletaPoint->body;

	R_PaletaDef.referenceAngle = 0 * DEGTORAD;
	R_PaletaDef.enableLimit = true;
	R_PaletaDef.lowerAngle = - 30 * DEGTORAD;
	R_PaletaDef.upperAngle = 30 * DEGTORAD;
	R_PaletaDef.localAnchorA.Set(PIXEL_TO_METERS(15), 0);
	R_PaletaDef.localAnchorB.Set(0, 0);

	b2RevoluteJoint* R_PaletaJoint = (b2RevoluteJoint*)App->physics->GetWorld()->CreateJoint(&R_PaletaDef);

	

	// circles
	App->physics->CreateCircle(178, 225, 30, ColliderType::UNKNOWN, b2_staticBody, 0.7f);

	App->physics->CreateCircle(258, 179, 30, ColliderType::UNKNOWN, b2_staticBody, 0.7f);

	App->physics->CreateCircle(254, 281, 30, ColliderType::UNKNOWN, b2_staticBody, 0.7f);

	App->physics->CreateCircle(239, 437, 15, ColliderType::UNKNOWN, b2_staticBody, 0.7f);
	App->physics->CreateCircle(382, 500, 15, ColliderType::UNKNOWN, b2_staticBody, 0.7f);
	App->physics->CreateCircle(444, 580, 12, ColliderType::UNKNOWN, b2_staticBody, 0.7f);

	// boost
	App->physics->CreateRectangleSensor(75, 430, 40, 280, ColliderType::BOOST);
	// Sensor

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 50, 566, 100, ColliderType::SENSOR);

	

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

	App->physics->CreateChain(0, 0, background_collision, 78, b2_staticBody, 0);

	//left slide
	int left_slide[12] = {
		100, 614,
		118, 684,
		185, 717,
		180, 727,
		107, 689,
		89, 617
	};

	App->physics->CreateChain(0, 0, left_slide, 12, b2_staticBody, 0.9f);

	// right slide
	int right_slide[12] = {
		347, 717,
		417, 682,
		433, 614,
		445, 617,
		426, 689,
		352, 726
	};

	App->physics->CreateChain(0, 0, right_slide, 12, b2_staticBody, 0.9f);

	//left triangle
	int left_triangle[6] = {
		184, 673,
		132, 618,
		151, 671
	};

	App->physics->CreateChain(0, 0, left_triangle, 6, b2_staticBody, 0.9f);

	//right triangle
	int right_triangle[6] = {
		349, 674,
		402, 617,
		383, 670
	};

	App->physics->CreateChain(0, 0, right_triangle, 6, b2_staticBody, 0.9f);

	//center arrow
	int center_arrow[12] = {
		245, 588,
		266, 566,
		287, 587,
		279, 594,
		266, 581,
		253, 594
	};

	App->physics->CreateChain(0, 0, center_arrow, 12, b2_staticBody, 0.9f);

	//trapezium
	int trapezium[10] = {
		91, 302,
		105, 296,
		189, 356,
		190, 456,
		91, 515
	};

	App->physics->CreateChain(0, 0, trapezium, 10, b2_staticBody, 0.9f);

	//center rectangle
	int center_rectangle[8] = {
		275, 506,
		334, 425,
		344, 432,
		285, 513
	};

	App->physics->CreateChain(0, 0, center_rectangle, 8, b2_staticBody, 0.7f);

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

	App->physics->CreateChain(0, 0, mini_rect1, 16, b2_staticBody, 0.7f);

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

	App->physics->CreateChain(0, 0, mini_rect2, 16, b2_staticBody, 0.7f);

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

	App->physics->CreateChain(0, 0, mini_rect3, 16, b2_staticBody, 0.7f);

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

	App->physics->CreateChain(0, 0, mini_rect4, 16, b2_staticBody, 0.7f);
}
