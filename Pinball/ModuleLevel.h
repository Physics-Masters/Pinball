#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleLevel : public Module
{
public:
	ModuleLevel(Application* app, bool start_enabled = true);
	~ModuleLevel();

	bool Start();
	update_status Update();
	bool CleanUp();


public:
	p2List<PhysBody*> groundchain;
	p2List<PhysBody*> lvl1chain;
	p2List<PhysBody*> lvl2chain;

	
	bool atground;
	bool atlvl1;
	bool atlvl2;

	SDL_Texture* ground;
	SDL_Texture* lvl1;
	SDL_Texture* lvl2;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
