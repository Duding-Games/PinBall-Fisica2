#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

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
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//draw map
	App->renderer->Blit(map, 0, 0);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		if (springForce < 200) {
			springForce += 5;
		}

		spring->body->ApplyForceToCenter(b2Vec2(0, springForce), 1);

		
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
		springForce = 0;
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
	spring = App->physics->CreateRectangle(504, 700, 24, 10);
	spring->body->SetFixedRotation(true);

	// spring point 
	springPoint = App->physics->CreateCircle(500, 710, 3);
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

	int background_collision[84] = {
		513, 767,
		514, 262,
		505, 234,
		496, 214,
		482, 191,
		463, 169,
		440, 149,
		407, 129,
		373, 116,
		337, 107,
		300, 103,
		263, 103,
		233, 105,
		206, 109,
		177, 117,
		148, 129,
		121, 145,
		102, 161,
		84, 184,
		72, 206,
		64, 228,
		60, 247,
		60, 559,
		84, 579,
		74, 585,
		60, 571,
		60, 655,
		76, 720,
		170, 765,
		170, 808,
		363, 808,
		363, 762,
		461, 711,
		479, 643,
		478, 365,
		460, 392,
		459, 546,
		449, 546,
		449, 389,
		476, 346,
		488, 346,
		489, 767
	};

	App->physics->CreateChain(0, 0, background_collision, 84, b2_staticBody);

	//left slide
	int left_slide[12] = {
		90, 617,
		106, 687,
		215, 744,
		220, 733,
		117, 680,
		100, 615
	};

	App->physics->CreateChain(0, 0, left_slide, 12, b2_staticBody);
	
	// right slide
	int right_slide [12] = {
		434, 615,
		417, 680,
		316, 734,
		321, 744,
		426, 688,
		445, 617
	};

	App->physics->CreateChain(0, 0, right_slide, 12, b2_staticBody);

	//left triangle
	int left_triangle[6] = {
		146, 593,
		146, 673,
		190, 694
	};

	App->physics->CreateChain(0, 0, left_triangle, 6, b2_staticBody);

	//right triangle
	int right_triangle[6] = {
		391, 595,
		391, 672,
		350, 694
	};
	
	App->physics->CreateChain(0, 0, right_triangle, 6, b2_staticBody);

	//center arrow
	int center_arrow[12] = {
		254, 597,
		274, 577,
		295, 597,
		286, 606,
		274, 592,
		261, 606
	};

	App->physics->CreateChain(0, 0, center_arrow, 12, b2_staticBody);

	//trapezium
	int trapezium[10] = {
		91, 302,
		105, 296,
		189, 356,
		190, 456,
		91, 515
	};

	App->physics->CreateChain(0, 0, trapezium, 10, b2_staticBody);
}
