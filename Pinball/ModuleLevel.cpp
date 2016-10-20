#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleLevel.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleLevel::ModuleLevel(Application* app, bool start_enabled) : Module(app, start_enabled)
{	
	ground = lvl1 = lvl2 = NULL;
	ray_on = false;
	atground = true;
	atlvl1 = false;
	atlvl2 = false;

}

ModuleLevel::~ModuleLevel()
{}

// Load assets
bool ModuleLevel::Start()
{
	LOG("Loading Level");
	bool ret = true;
	lvl1sensor = App->physics->CreateRectangleSensor(0, SCREEN_HEIGHT / 2 -62, 40, 1);
	lvl1sensor->listener = this;

	
	growndsensor = App->physics->CreateRectangleSensor(2, SCREEN_HEIGHT / 2 + -59, 40, 1);
	growndsensor->listener = this;

	lvl1sensor2 = App->physics->CreateRectangleSensor(SCREEN_WIDTH - 30, SCREEN_HEIGHT / 2 - 14, 30, 1);
	lvl1sensor2->listener = this;

	growndsensor2 = App->physics->CreateRectangleSensor(SCREEN_WIDTH - 30, SCREEN_HEIGHT / 2 - 11, 30, 1);
	growndsensor2->listener = this;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	ground = App->textures->Load("pinball/background.png");
	lvl1 = App->textures->Load("pinball/level1.png");
	lvl2 = App->textures->Load("pinball/level2.png");

	return ret;
}

// Load assets
bool ModuleLevel::CleanUp()
{
	LOG("Unloading Level");

	return true;
}

// Update: draw background
update_status ModuleLevel::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	App->renderer->Blit(ground, 0, 0, { (256, 432, 0, 0) }, 1.0f);
	App->renderer->Blit(lvl1, 0, 15, { (256, 432, 0, 0) }, 1.0f);
	App->renderer->Blit(lvl2, 0, 0, { (256, 432, 0, 0) }, 1.0f);
	
	
	if (atground == true && groundchain == nullptr)
	{
		// Pivot 0, 0
		int background[206] = {
			92, 432,
			92, 416,
			32, 386,
			32, 416,
			27, 419,
			17, 419,
			12, 416,
			13, 322,
			15, 302,
			17, 296,
			24, 290,
			24, 286,
			19, 280,
			18, 242,
			20, 236,
			25, 235,
			26, 228,
			12, 207,
			-13, 133,
			11, 132,
			29, 196,
			33, 208,
			37, 210,
			38, 204,
			30, 184,
			33, 177,
			15, 137,
			12, 122,
			11, 101,
			14, 80,
			20, 67,
			32, 53,
			45, 45,
			60, 41,
			75, 39,
			95, 40,
			112, 44,
			121, 49,
			121, 36,
			123, 30,
			126, 28,
			134, 28,
			139, 33,
			139, 80,
			142, 89,
			143, 102,
			140, 120,
			143, 123,
			147, 118,
			158, 118,
			164, 122,
			164, 129,
			168, 130,
			171, 126,
			165, 119,
			157, 100,
			155, 89,
			156, 61,
			162, 48,
			172, 37,
			190, 30,
			210, 29,
			224, 34,
			237, 42,
			246, 56,
			249, 71,
			247, 97,
			237, 124,
			232, 135,
			220, 191,
			210, 192,
			207, 207,
			217, 206,
			219, 195,
			225, 174,
			234, 134,
			242, 126,
			252, 127,
			253, 137,
			251, 159,
			246, 181,
			238, 204,
			234, 211,
			230, 219,
			240, 209,
			249, 210,
			252, 216,
			252, 225,
			247, 232,
			234, 244,
			225, 251,
			230, 256,
			216, 289,
			223, 296,
			225, 305,
			227, 328,
			228, 416,
			223, 420,
			211, 419,
			207, 415,
			208, 387,
			149, 416,
			149, 432
		};
		groundchain = App->physics->CreateChain(0, 0, background, 206);
	}
	else if (groundchain != nullptr && atground == false)
	{
		App->physics->DestroyBodys(*groundchain);
		groundchain = nullptr;
	}


	if (atlvl1 == true && lvl1chain == nullptr)
	{
		int level1[130] = {
			66, 128,
			46, 107,
			33, 91,
			30, 80,
			30, 59,
			32, 41,
			37, 27,
			50, 14,
			61, 8,
			79, 2,
			95, 0,
			125, 6,
			132, 8,
			140, 13,
			144, 22,
			150, 35,
			152, 51,
			152, 72,
			148, 90,
			141, 108,
			134, 119,
			114, 138,
			32, 178,
			24, 185,
			18, 192,
			18, 252,
			20, 256,
			25, 259,
			45, 268,
			51, 275,
			53, 283,
			53, 296,
			56, 303,
			55, 310,
			48, 313,
			41, 313,
			35, 309,
			35, 302,
			37, 297,
			37, 290,
			35, 284,
			22, 277,
			8, 270,
			0, 262,
			0, 189,
			4, 179,
			17, 168,
			104, 125,
			122, 106,
			130, 88,
			134, 73,
			133, 41,
			128, 32,
			121, 27,
			113, 22,
			99, 18,
			82, 18,
			67, 22,
			56, 32,
			49, 48,
			48, 60,
			48, 73,
			51, 86,
			64, 101,
			79, 115
		};
		lvl1chain = App->physics->CreateChain(0, 16, level1, 130);
	}
	else if (lvl1chain != nullptr && atlvl1 == false)
	{
		App->physics->DestroyBodys(*lvl1chain);
		lvl1chain = nullptr;
	}

	if (atlvl2 == true && lvl2chain == nullptr)
	{
		// Pivot 0, 0
		int level2[130] = {
			66, 128,
			46, 107,
			33, 91,
			30, 80,
			30, 59,
			32, 41,
			37, 27,
			50, 14,
			61, 8,
			79, 2,
			95, 0,
			125, 6,
			132, 8,
			140, 13,
			144, 22,
			150, 35,
			152, 51,
			152, 72,
			148, 90,
			141, 108,
			134, 119,
			114, 138,
			32, 178,
			24, 185,
			18, 192,
			18, 252,
			20, 256,
			25, 259,
			45, 268,
			51, 275,
			53, 283,
			53, 296,
			56, 303,
			55, 310,
			48, 313,
			41, 313,
			35, 309,
			35, 302,
			37, 297,
			37, 290,
			35, 284,
			22, 277,
			8, 270,
			0, 262,
			0, 189,
			4, 179,
			17, 168,
			104, 125,
			122, 106,
			130, 88,
			134, 73,
			133, 41,
			128, 32,
			121, 27,
			113, 22,
			99, 18,
			82, 18,
			67, 22,
			56, 32,
			49, 48,
			48, 60,
			48, 73,
			51, 86,
			64, 101,
			79, 115
		};

		lvl1chain = App->physics->CreateChain(0, 0, level2, 130);
	}
	else if (lvl2chain != nullptr && atlvl2 == false)
	{
		App->physics->DestroyBodys(*lvl2chain);
		lvl2chain = nullptr;
	}

	fVector normal(0.0f, 0.0f);

	
	return UPDATE_CONTINUE;
}


void ModuleLevel::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;
	if (bodyB->body->GetType() == b2Shape::e_circle || bodyA->body->GetType() == b2Shape::e_circle)
	{
		if (bodyA == lvl1sensor || bodyB == lvl1sensor || bodyA == lvl1sensor2 || bodyB == lvl1sensor2)
		{
			if (atground == true)
			{
				atground = false;
				atlvl1 = true;
				LOG("AT LVL 1, %d g, %d l1", atground, atlvl1)
			}
		}
		if (bodyA == growndsensor || bodyB == growndsensor || bodyA == growndsensor2 || bodyB == growndsensor2)
		{
			if (atground == false)
			{
				atground = true;
				atlvl1 = false;
				LOG("AT background, %d g, %d lvl1", atground, atlvl1)
			}
		}		
	}
}
