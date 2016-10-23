
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSensors.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSensors::ModuleSensors(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}
ModuleSensors::~ModuleSensors()
{
	
}
bool ModuleSensors::Start()
{
	int SLT[8] = {
		58, 314,
		75, 354,
		68, 348,
		58, 325
	};
	int SRT[8] = {
		182, 314,
		164, 354,
		168, 348,
		178, 325
	};
	dome = App->textures->Load("pinball/gems.png");

	Dome = App->physics->CreateRectangleSensor(120, 200, 38, 38, GROUND, GROUND | BALL);
	Dome->listener = this;
	LTriBounce = App->physics->CreateChain(0, 0, SLT, 8, GROUND, GROUND | BALL);;
	LTriBounce->body->GetFixtureList()->SetRestitution(1);
	LTriBounce->listener = this;
	RTriBounce = App->physics->CreateChain(0, 0, SRT, 8, GROUND, GROUND | BALL);;
	RTriBounce->body->GetFixtureList()->SetRestitution(1);
	return true;
	
}
update_status ModuleSensors::Update()
{
	
	
	
	return UPDATE_CONTINUE;
}
void ModuleSensors::OnCollision(PhysBody* A , PhysBody* B)
{
	

}
bool ModuleSensors::CleanUp()
{
	LOG("Unloading Sensors");

	return true;
}
