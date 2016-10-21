#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"


class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> paddlesL;
	p2List<PhysBody*> paddlesR;
	p2List<PhysBody*> boxes;
	PhysBody* pad;
	
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* paddle;
	SDL_Texture* paddle2;
	SDL_Texture* box;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
