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
	int DiamondCount = 0;
	bool AbleSpecial;
	bool SpH = false;
	bool TLH = false;
	bool TRH = false;
	bool TCH = false;
	
	
	PhysBody* LTriBounce;
	PhysBody* RTriBounce;
	PhysBody* Dome;
	PhysBody* SpecialHole;
	PhysBody* TLHole;
	PhysBody* TRHole;
	PhysBody* TCHole;
	p2List<PhysBody*> Holes;
	//ANIMATIONS AND TEXTURES
	SDL_Texture* dome;

private:
	ModuleSensors* sensors[MAX_SENSORS];
	
};
