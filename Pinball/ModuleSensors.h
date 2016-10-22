#pragma once

#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class ModuleSensors : public Module
{
public:
	ModuleSensors(Application* app, bool start_enabled = true);
	~ModuleSensors();

	bool Start();
	update_status Update();
	bool CleanUp();
	//void OnCollision(PhysBody*, PhysBody*);
public:
	PhysBody* LTriBounce;
	PhysBody* RTriBounce;
	
};
