#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -6.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum class ColliderType {
	UNKNOWN,
	METEOR,
	SPECIAL_METEOR_1,
	SPECIAL_METEOR_2,
	SPECIAL_METEOR_3,
	SENSOR,
	BALL,
	POINT,
	BOOST
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
	ColliderType type;
	
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();


	PhysBody* CreateCircle(int x, int y, int radius, ColliderType ctype, b2BodyType type, float restituition);

	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType type);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, ColliderType type);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType type, float restituition);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	b2World* GetWorld();

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
	b2Body* mouse_body;
};