
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
	int SpHole[8] = {
		239, 215,
		249, 225,
		250, 217,
		245, 212
	};
	int Hole[8] = {
		145, 131,
		161, 131,
		157, 123,
		148, 123
	};
	int LHole[8] = {
		98, 100,
		114, 100,
		114, 104,
		99, 104
	};
	int SHole[8] = {
		196, 85,
		207, 89,
		209, 84,
		197, 80
	};
	TCHole = (App->physics->CreatePolySensor(0, 0, Hole, 8, GROUND, GROUND | BALL));
	TLHole = (App->physics->CreatePolySensor(0, 0, LHole, 8, GROUND, GROUND | BALL));
	TRHole = (App->physics->CreatePolySensor(0, 0, SHole, 8, GROUND, GROUND | BALL));
	SpecialHole = App->physics->CreatePolySensor(0, 0, SpHole,8, GROUND, GROUND | BALL);

	LTriBounce = App->physics->CreateChain(0, 0, SLT, 8, GROUND, GROUND | BALL);;
	LTriBounce->body->GetFixtureList()->SetRestitution(1);
	LTriBounce->listener = this;
	RTriBounce = App->physics->CreateChain(0, 0, SRT, 8, GROUND, GROUND | BALL);;
	RTriBounce->body->GetFixtureList()->SetRestitution(1);
	return true;
	
}
update_status ModuleSensors::Update()
{
	if (SpH == true)
	{
		App->renderer->Blit(App->level->Particle, 230, 207, &(App->level->ParticleAnim.GetCurrentFrame()), 1.0f);
		if (App->level->ParticleAnim.Finished())
		{
			App->level->ParticleAnim.Reset();
			SpH = false;
		}
		
	}
	if (TLH == true)
	{
		App->renderer->Blit(App->level->Particle, 94, 91, &(App->level->ParticleAnim.GetCurrentFrame()), 1.0f);
		if (App->level->ParticleAnim.Finished())
		{
			App->level->ParticleAnim.Reset();
			TLH = false;
		}

	}
	if (TCH == true)
	{
		App->renderer->Blit(App->level->Particle, 140,120, &(App->level->ParticleAnim.GetCurrentFrame()), 1.0f);
		if (App->level->ParticleAnim.Finished())
		{
			App->level->ParticleAnim.Reset();
			TCH = false;
		}

	}
	if (TRH == true)
	{
		App->renderer->Blit(App->level->Particle, 189, 72, &(App->level->ParticleAnim.GetCurrentFrame()), 1.0f);
		if (App->level->ParticleAnim.Finished())
		{
			App->level->ParticleAnim.Reset();
			TRH = false;
		}

	}
	//printef = false;
	
	
	return UPDATE_CONTINUE;
}
void ModuleSensors::OnCollision(PhysBody* A , PhysBody* B)
{
 	if (B == SpecialHole)
	{
		SpH = true;
		if (AbleSpecial == true)
		{
			DiamondCount++;
			if (DomeCounter >= 3)
			{
				DomeCounter = 0;
				AbleSpecial = false;
			}
		}
	}
	if (B == TLHole)
	{
		TLH = true;
	}
	
	if (B == TCHole)
	{
		TCH = true;
	}
	if (B == TRHole)
	{
		TRH = true;
	}
	
	//DomeCounter++;

}
bool ModuleSensors::CleanUp()
{
	LOG("Unloading Sensors");

	return true;
}
