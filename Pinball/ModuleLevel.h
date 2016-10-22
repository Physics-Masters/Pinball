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
	void OnCollision(PhysBody*, PhysBody*);


public:
	PhysBody* groundchain;
	PhysBody* lvl1chain;
	PhysBody* lvl2chain;
	PhysBody* lvl1growndsensor0;
	PhysBody* lvl1growndsensor1;
	PhysBody* lvl2growndsensor0;
	PhysBody* lvl2growndsensor1;
	PhysBody* lvl2growndsensor2;
	PhysBody* lvl1sensor;
	PhysBody* lvl2sensor0;
	PhysBody* lvl2sensor1;
	PhysBody* lvl2sensor2;
	
	bool atground;
	bool atlvl1;
	bool atlvl2;
	bool pground;
	bool plvl1;
	bool plvl2;

	SDL_Texture* ground;
	SDL_Texture* lvl1;
	SDL_Texture* lvl2;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
