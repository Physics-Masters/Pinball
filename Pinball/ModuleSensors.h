#pragma once

#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

#define MAX_SENSORS 200
class Sensor;
class ModuleSensors : public Module
{
public:
	ModuleSensors(Application* app, bool start_enabled = true);
	~ModuleSensors();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody*, PhysBody*);

public:
	int DomeCounter;
	bool AbleSpecial;
	
	PhysBody* LTriBounce;
	PhysBody* RTriBounce;
	PhysBody* Dome;
	PhysBody* SpecialHole;
	p2List<PhysBody*> Holes;
	//ANIMATIONS AND TEXTURES
	SDL_Texture* dome;

private:
	ModuleSensors* sensors[MAX_SENSORS];
	
};
