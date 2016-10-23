#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

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
	p2List<PhysBody*> circles;
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
	PhysBody* Canonsensor;
	PhysBody* exitfromcanon;

	PhysBody* holesensor;
	
	p2List<PhysBody*> groundchains;

	bool atground;
	bool atlvl1;
	bool atlvl2;
	bool start;
	bool rdytostart;
	bool ballatcannon;
	//Animations
	Animation LeftTriAnim;
	Animation RightTriAnim;
	Animation LightsAnim;
	

	SDL_Texture* LightsS;
	SDL_Texture* diamonds;
	SDL_Texture* Tri;
	SDL_Texture* ground;
	SDL_Texture* lvl1;
	SDL_Texture* lvl2;
	uint bonus_fx;

	uint current_time;

	p2Point<int> ray;
	bool ray_on;
};
