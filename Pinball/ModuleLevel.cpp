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
	atground = false;
	atlvl1 = false;
	atlvl2 = false;
	pground = true;
	plvl1 = false;
	plvl2 = false;

}

ModuleLevel::~ModuleLevel()
{}

// Load assets
bool ModuleLevel::Start()
{
	LOG("Loading Level");
	bool ret = true;

	int slvl[12] = {
		68, 134,
		67, 138,
		71, 138,
		74, 135,
		74, 131,
		70, 132
	};
		
	//rampa esquerra pujada
	lvl2sensor0 = App->physics->CreatePolySensor(-62, 30, slvl, 12, GROUND, GROUND|BALL);
	lvl2sensor0->listener = this;
	//rampa dreta pujada
	lvl2sensor1 = App->physics->CreatePolySensor(168, 30, slvl, 12, GROUND, GROUND | BALL);
	lvl2sensor1->listener = this;
	
	//rampa metall pujada
	lvl1sensor = App->physics->CreatePolySensor(-10, -10, slvl, 12, GROUND, GROUND | BALL);
	lvl1sensor->listener = this;

	//rampa metall baixada
	lvl1growndsensor0 = App->physics->CreatePolySensor(0, 0, slvl, 12, LVL1, LVL1|BALL);
	lvl1growndsensor0->listener = this;

	lvl2growndsensor0 = App->physics->CreatePolySensor(-55, 50, slvl, 12, LVL2, LVL2 | BALL);
	lvl2growndsensor0->listener = this;
	
	lvl2growndsensor1 = App->physics->CreatePolySensor(163, 50, slvl, 12, LVL2, LVL2 | BALL);
	lvl2growndsensor1->listener = this;
	
	lvl2growndsensor2 = App->physics->CreatePolySensor(122, -85, slvl, 12, LVL2, LVL2 | BALL);
	lvl2growndsensor2->listener = this;
	
	int elvl1[12] = {
		40, 311,
		45, 308,
		51, 312,
		51, 310,
		45, 306,
		40, 309
	};
	//rampa metall forat
	lvl1growndsensor1 = App->physics->CreatePolySensor(0, 18, elvl1, 12, LVL1, LVL1|BALL);
	lvl1growndsensor1->listener = this;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	ground = App->textures->Load("pinball/background.png");
	lvl1 = App->textures->Load("pinball/level1.png");
	lvl2 = App->textures->Load("pinball/level2.png");

		int background[202] = {
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
			13, 208,
			0, 169, 
			-5, 132,
			15, 144,
			21, 175,
			32, 204, 
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
			219, 190,
			212, 192,
			208, 205, 
			215, 202, 
			219, 195,
			226, 176,
			233, 146,
			251, 149, 
			245, 179,
			235, 208,
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
		groundchain = App->physics->CreateChain(0, 0, background, 202, GROUND, GROUND | BALL);
	
		int level1[132] = {
			70, 138,
			68, 128,
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
			94, 130
		};
		lvl1chain = App->physics->CreateChain(0, 16, level1, 132, LVL1, LVL1 | BALL);
	
	
		int level2[108] = {
			32, 204,
			21, 175,
			15, 144, 
			11, 107,
			11, 81,
			17, 61,
			28, 45,
			40, 34,
			58, 26,
			84, 20,
			113, 18,
			146, 19,
			157, 22,
			169, 28,
			179, 39,
			182, 44,
			190, 67, 
			208, 64,
			203, 46,
			193, 28,
			205, 32,
			215, 39,
			226, 52,
			232, 64,
			236, 80,
			237, 99,
			236, 122,
			233, 146, 
			226, 176,
			219, 195,
			215, 202,
			235, 208, 
			245, 179,
			251, 149, 
			253, 118,
			253, 73,
			250, 59,
			241, 41,
			226, 25,
			205, 13,
			168, 3,
			150, 0,
			124, -1,
			93, 1,
			59, 8,
			38, 15,
			19, 27,
			5, 43,
			-2, 60,
			-5, 77,
			-7, 104,
			-5, 132,
			0, 169, 
			13, 208
		};
		lvl1chain = App->physics->CreateChain(0, 0, level2, 108, LVL2, LVL2 | BALL);
	
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
	p2List_item<PhysBody*>* c;
	c = App->scene_intro->circles.getFirst();

	App->renderer->Blit(ground, 0, 0, { (256, 432, 0, 0) }, 1.0f);
	while (c != NULL)
	{

		int x, y;
		c->data->GetPosition(x, y);
		
		if (c->data->body->GetFixtureList()->GetFilterData().categoryBits == GROUND)
		{
			App->renderer->Blit(App->scene_intro->circle, x, y, NULL, 1.0f);
		}
		c = c->next;
	}
	c = App->scene_intro->circles.getFirst();
	App->renderer->Blit(lvl1, 0, 15, { (256, 432, 0, 0) }, 1.0f);
	while (c != NULL)
	{

		int x, y;
		c->data->GetPosition(x, y);

		if (c->data->body->GetFixtureList()->GetFilterData().categoryBits == LVL1)
		{
			App->renderer->Blit(App->scene_intro->circle, x, y, NULL, 1.0f);
		}
		c = c->next;
	}
	c = App->scene_intro->circles.getFirst();
	App->renderer->Blit(lvl2, 0, 0, { (256, 432, 0, 0) }, 1.0f);
	while (c != NULL)
	{

		int x, y;
		c->data->GetPosition(x, y);

		if (c->data->body->GetFixtureList()->GetFilterData().categoryBits == LVL2)
		{
			App->renderer->Blit(App->scene_intro->circle, x, y, NULL, 1.0f);
		}
		c = c->next;
	}

	if (atground == true)
	{
		b2Filter ball;
		ball.categoryBits = GROUND;
		ball.maskBits = GROUND|BALL;
		App->scene_intro->circles.getLast()->data->body->GetFixtureList()->SetFilterData(ball);
		atground = false;
		pground = true;
	}
	if (atlvl1 == true)
	{
		b2Filter ball;
		ball.categoryBits = LVL1;
		ball.maskBits = LVL1 | BALL;
		App->scene_intro->circles.getLast()->data->body->GetFixtureList()->SetFilterData(ball);
		atlvl1 = false;

		plvl1 = true;
		pground = false;
		plvl2 = false;
	}
	if (atlvl2 == true)
	{
		b2Filter ball;
		ball.categoryBits = LVL2;
		ball.maskBits = LVL2 | BALL;
		App->scene_intro->circles.getLast()->data->body->GetFixtureList()->SetFilterData(ball);
		atlvl2 = false;

		plvl2 = true;
		plvl1 = false;
		pground = false;
	}
	fVector normal(0.0f, 0.0f);
	
	return UPDATE_CONTINUE;
}


void ModuleLevel::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->body != nullptr && bodyB->body != nullptr)
	{
		if (bodyA->body->GetType() == b2Shape::e_circle )
		{
			if (bodyB == lvl1sensor)
			{
				atlvl1 = true;
				LOG("bodyB at lvl 1");
			}
			if (bodyB == lvl2sensor0 || bodyB == lvl2sensor1)
			{
				atlvl2 = true;
				LOG("Body at lvl 2")
			}
			if (bodyB == lvl1growndsensor0|| bodyB == lvl1growndsensor1 || bodyB == lvl2growndsensor0 || bodyB == lvl2growndsensor1 || bodyB == lvl2growndsensor2)
			{
				atground = true;
				LOG("bodyB at lvl 1");
			}

		}
	}
}
