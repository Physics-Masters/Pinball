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

}

ModuleLevel::~ModuleLevel()
{}

// Load assets
bool ModuleLevel::Start()
{
	LOG("Loading Level");
	bool ret = true;

	int slvl[12] = {
		69, 139,
		71, 138,
		76, 133,
		77, 131,
		74, 133,
		71, 136
	};
		
	//rampa esquerra pujada
	lvl2sensor0 = App->physics->CreatePolySensor(-62, 30, slvl, 12, GROUND, GROUND|BALL);
	lvl2sensor0->listener = this;
	//rampa dreta pujada
	lvl2sensor1 = App->physics->CreatePolySensor(168, 30, slvl, 12, GROUND, GROUND | BALL);
	lvl2sensor1->listener = this;
	
	//rampa metall pujada
	lvl1sensor = App->physics->CreatePolySensor(-3, -3, slvl, 12, GROUND, GROUND | BALL);
	lvl1sensor->listener = this;

	//rampa metall baixada
	lvl1growndsensor0 = App->physics->CreatePolySensor(6, 9, slvl, 12, LVL1, LVL1|BALL);
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
	Tri = App->textures->Load("pinball/triangles.png");
	LightsS = App->textures->Load("pinball/lights.png");
	LightsAnim.PushBack({ 0,0,222,152 });
	LightsAnim.PushBack({ 222,0,222,152 });
	LightsAnim.PushBack({ 0,153,222,152 });
	LightsAnim.PushBack({ 222,153,222,152 });
	LightsAnim.PushBack({ 0,305,222,152 });
	LightsAnim.speed = 0.02;
	LeftTriAnim.PushBack({ 0,0,21,40 });
	LeftTriAnim.PushBack({ 21,0,21,40 });
	LeftTriAnim.speed = 0.05;
	RightTriAnim.PushBack({ 42,0,21,40 });
	RightTriAnim.PushBack({ 63,0,21,40 });
	RightTriAnim.speed = 0.05;

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
			25, 184,
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
		
		int groundtolvl1[32] = {
			65, 141,
			55, 152,
			52, 152,
			52, 149,
			54, 146,
			54, 128,
			57, 118,
			63, 113,
			71, 113,
			78, 118,
			84, 124,
			92, 131,
			87, 136,
			80, 130,
			64, 114,
			54, 131
		};

		int groundcircle[30] = {
			35, 96,
			37, 85,
			44, 75,
			54, 67,
			62, 63,
			69, 63,
			71, 61,
			70, 59,
			60, 60,
			52, 64,
			42, 71,
			36, 79,
			33, 87,
			32, 92,
			33, 95
		};

		int groundcircle1[56] = {
			103, 60,
			96, 59,
			90, 59,
			89, 61,
			90, 63,
			96, 63,
			103, 65,
			109, 68,
			114, 72,
			117, 76,
			118, 81,
			91, 82,
			85, 83,
			85, 96,
			86, 103,
			97, 103,
			98, 97,
			103, 93,
			109, 93,
			114, 97,
			115, 103,
			125, 103,
			125, 95,
			124, 84,
			123, 81,
			119, 72,
			113, 65,
			108, 62
		};

		int groundleft[26] = {
			36, 316,
			32, 321,
			32, 360,
			35, 364,
			81, 387,
			83, 387,
			83, 381,
			87, 381,
			83, 379,
			45, 359,
			39, 353,
			36, 345,
			36, 328
		};

		int groundright[24] = {
			204, 317,
			204, 345,
			201, 354,
			196, 359,
			152, 381,
			155, 383,
			157, 387,
			161, 386,
			201, 366,
			204, 364,
			208, 360,
			208, 322
		};
		int leftTri[8] = {
			57, 318,
			57, 341,
			75, 350,
			58, 313
		};
		int rightTri[8] = {
			165, 351,
			183, 342,
			182, 313,
			164, 349
		};

		groundchains.add(App->physics->CreateChain(0, 0, groundtolvl1, 32, GROUND, GROUND | BALL));
		groundchains.add(App->physics->CreateChain(0, 0, groundcircle, 30, GROUND, GROUND | BALL));
		groundchains.add(App->physics->CreateChain(0, 0, groundcircle1, 56, GROUND, GROUND | BALL));
		groundchains.add(App->physics->CreateChain(0, -1, groundleft, 26, GROUND, GROUND | BALL));
		groundchains.add(App->physics->CreateChain(0, -1, groundright, 24, GROUND, GROUND | BALL));
		groundchains.add(App->physics->CreateChain(0, 4, leftTri, 8, GROUND, GROUND | BALL));
		groundchains.add(App->physics->CreateChain(0, 4, rightTri, 8, GROUND, GROUND | BALL));

		int level1[132] = {
			70, 140,
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
	
	// render ground
	App->renderer->Blit(ground, 0, 0, { (256, 432, 0, 0) }, 1.0f);
	App->renderer->Blit(Tri, 57, 316, &(LeftTriAnim.GetCurrentFrame()), 1.0f);
	App->renderer->Blit(Tri, 163, 316, &(RightTriAnim.GetCurrentFrame()), 1.0f);
	App->renderer->Blit(LightsS, 14, 105, &(LightsAnim.GetCurrentFrame()), 1.0f);
	// render all balls at background
	c = App->scene_intro->circles.getFirst();
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
	c = App->scene_intro->paddlesL.getFirst();
	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);

		App->renderer->Blit(App->scene_intro->paddle, x - 1, y, NULL, 1.0f, c->data->GetRotation(), 0, 0);
		c = c->next;
	}

	//render lvl 1
	App->renderer->Blit(lvl1, 0, 15, { (256, 432, 0, 0) }, 1.0f);
	//render all balls at lvl 1
	c = App->scene_intro->circles.getFirst();
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

	//render lvl 2
	App->renderer->Blit(lvl2, 0, 0, { (256, 432, 0, 0) }, 1.0f);
	//render balls at lvl 2
	c = App->scene_intro->circles.getFirst();
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
	
	fVector normal(0.0f, 0.0f);
	
	return UPDATE_CONTINUE;
}


void ModuleLevel::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->body != nullptr && bodyB->body != nullptr)
	{
		if (bodyA->body->GetType() == b2_dynamicBody && bodyB->body->GetFixtureList()->IsSensor() == true)
		{
			p2List_item<PhysBody*>* c = App->scene_intro->circles.getFirst();
			b2Filter balllvl;
			balllvl.categoryBits = bodyA->body->GetFixtureList()->GetFilterData().categoryBits;
			balllvl.maskBits = bodyA->body->GetFixtureList()->GetFilterData().maskBits;
			while (c != NULL)
			{
				if (c->data->body == bodyA->body)
				{
					if (bodyB == lvl1sensor)
					{
						balllvl.categoryBits = LVL1;
						balllvl.maskBits = LVL1 | BALL;
					}
					if (bodyB == lvl2sensor0 || bodyB == lvl2sensor1)
					{
						balllvl.categoryBits = LVL2;
						balllvl.maskBits = LVL2 | BALL;
					}
					if (bodyB == lvl1growndsensor0 || bodyB == lvl1growndsensor1 || bodyB == lvl2growndsensor0 || bodyB == lvl2growndsensor1 || bodyB == lvl2growndsensor2)
					{
						balllvl.categoryBits = GROUND;
						balllvl.maskBits = GROUND | BALL;
					}
					c->data->body->GetFixtureList()->SetFilterData(balllvl);
				}
				c = c->next;
			}
		}
	}
}
