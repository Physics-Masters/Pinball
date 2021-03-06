#pragma once
#include "Module.h"

#include "ModuleLevel.h"
#include "Globals.h"
#include "Animation.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;


public:
	int width, height;
	b2Body* body;
	Module* listener;
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

	PhysBody* CreateCircle(int x, int y, int radius, uint16 categorybits, uint16 maskbits);
	PhysBody* CreateRectangle(int x, int y, int width, int height, uint16 categorybits, uint16 maskbits);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, uint16 categorybits, uint16 maskbits);
	PhysBody* CreatePolySensor(int x, int y, int* points, int size, uint16 categorybits, uint16 maskbits);
	PhysBody* CreateChain(int x, int y, int* points, int size, uint16 categorybits, uint16 maskbits);
	PhysBody* CreatePaddleL(int x, int y, float angd, float angu, uint16 categorybits, uint16 maskbits);
	PhysBody* CreatePaddleR(int x, int y, float angd, float angu, uint16 categorybits, uint16 maskbits);
	void PaddleMoveR();
	void PaddleStopR();
	void PaddleMoveL();
	void PaddleStopL();

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);
	void DestroyBodys(PhysBody &body);

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2RevoluteJoint* paddles;
	p2List<b2RevoluteJoint*> paddleList;
	p2List<b2RevoluteJoint*> paddleListR;
	b2Body* ground;
};