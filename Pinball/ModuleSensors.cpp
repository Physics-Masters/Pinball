
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
{}
bool ModuleSensors::Start()
{
	LightsS = App->textures->Load("pinball/lights.png");
	LightsAnim.PushBack({ 0,0,222,152 });
	LightsAnim.PushBack({ 222,0,222,152 });
	LightsAnim.PushBack({ 0,153,222,152 });
	LightsAnim.PushBack({ 222,153,222,152 });
	LightsAnim.PushBack({ 0,305,222,152 });
	LightsAnim.speed = 0.02;

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
	LTriBounce = App->physics->CreateChain(0, 0, SLT, 8, GROUND, GROUND | BALL);;
	LTriBounce->body->GetFixtureList()->SetRestitution(1);
	RTriBounce = App->physics->CreateChain(0, 0, SRT, 8, GROUND, GROUND | BALL);;
	RTriBounce->body->GetFixtureList()->SetRestitution(1);
	return true;
}
update_status ModuleSensors::Update()
{
	App->renderer->Blit(LightsS, 14, 105, &(LightsAnim.GetCurrentFrame()), 1.0f);
	return UPDATE_CONTINUE;
}
bool ModuleSensors::CleanUp()
{
	LOG("Unloading Sensors");

	return true;
}
