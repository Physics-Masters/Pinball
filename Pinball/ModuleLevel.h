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
	
	//start functions
	void createchains();
	void createsensors();

public:

	int puntuation;
	int playerslife;
	bool inmortal;
	int bestpuntuation;

	//dynamic bodys
	PhysBody* circle;
	p2List<PhysBody*> paddlesL;
	p2List<PhysBody*> paddlesR;
	p2List<PhysBody*> circles;
	p2List<PhysBody*> circlestodelete;

	//sensors
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
	
	p2List<PhysBody*> mapchains;

	//bools for changing lvls
	bool atground;
	bool atlvl1;
	bool atlvl2;
	bool start;
	bool rdytostart;
	bool ballatcannon;
	bool cannonanim = false;

	//Animations
	Animation LeftTriAnim;
	Animation RightTriAnim;
	Animation LightsAnim;
	Animation ArrowAnim;
	Animation ParticleAnim;
	Animation CannonAnim;
	//efects textures
	SDL_Texture* LightsS;
	SDL_Texture* diamonds;
	SDL_Texture* Top;
	SDL_Texture* Win;
	SDL_Texture* Arrow;
	SDL_Texture* Particle;
	SDL_Texture* cannon;
	//sound effects
	uint holeS;
	uint domeS;
	uint launchS;
	uint bounceS;
	uint paddleS;
	//lvl textures
	SDL_Texture* Tri; 
	SDL_Texture* ground;
	SDL_Texture* lvl1;
	SDL_Texture* lvl2;
	SDL_Texture* circletexture;
	SDL_Texture* paddletexture;
	SDL_Texture* paddle2texture;

	uint bonus_fx;
};
