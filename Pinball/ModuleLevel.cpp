#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSensors.h"
#include "ModuleLevel.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"


ModuleLevel::ModuleLevel(Application* app, bool start_enabled) : Module(app, start_enabled)
{	
	ground = lvl1 = lvl2 = NULL;
	atground = false;
	atlvl1 = false;
	atlvl2 = false;
	start = false;
	rdytostart = false;

}

ModuleLevel::~ModuleLevel()
{}

// Load assets
bool ModuleLevel::Start()
{
	LOG("Loading Level");

	bool ret = true;
	
	inmortal = false;
	playerslife = 0;
	puntuation = 0;
	bestpuntuation = 0;

	//create all map chains and sensors
	createchains();
	createsensors();
	
	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Textures
	ground = App->textures->Load("pinball/background.png");
	lvl1 = App->textures->Load("pinball/level1.png");
	lvl2 = App->textures->Load("pinball/level2.png");
	Tri = App->textures->Load("pinball/triangles.png");
	LightsS = App->textures->Load("pinball/lights.png");
	diamonds = App->textures->Load("pinball/Diamonds.png");
	Win = App->textures->Load("pinball/Win.png");
	Arrow = App->textures->Load("pinball/Arrow.png");
	Particle = App->textures->Load("pinball/particle.png");
	cannon = App->textures->Load("pinball/cannon.png");
	circletexture = App->textures->Load("pinball/ball.png");
	paddletexture = App->textures->Load("pinball/paddle.png");
	paddle2texture = App->textures->Load("pinball/paddle2.png");
	Top = App->textures->Load("pinball/topdetails.png");
	Restart = App->textures->Load("pinball/restart.png");

	RestartAnim.PushBack({ 0,0,256,44 });
	RestartAnim.PushBack({ 0,44,256,44 });
	RestartAnim.speed = 0.1f;

	CannonAnim.PushBack({ 0,0,31,36 });
	CannonAnim.PushBack({ 0,0,31,36 });
	CannonAnim.PushBack({ 31,0,31,36 });
	CannonAnim.PushBack({ 62,0,31,36 });
	CannonAnim.PushBack({ 31,0,31,36 });
	CannonAnim.PushBack({ 0,0,31,36 });
	CannonAnim.loop = false;
	CannonAnim.speed = 0.2f;

	ParticleAnim.PushBack({ -24,0,24,24 });
	ParticleAnim.PushBack({ 0,0,24,24 });
	ParticleAnim.PushBack({ 24,0,24,24 });
	ParticleAnim.PushBack({ 48,0,24,24 });
	ParticleAnim.PushBack({ 72,0,24,24 });
	ParticleAnim.PushBack({ 96,0,24,24 });
	//ParticleAnim.PushBack({ 120,0,24,24 });
	ParticleAnim.loop = true;
	
	ParticleAnim.speed = 0.15f;

	
	ArrowAnim.PushBack({ 0,0,26,29 });
	ArrowAnim.PushBack({ 0,-1,26,31 });
	ArrowAnim.speed = 0.05f;

	

	//Audio
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	holeS = App->audio->LoadFx("pinball/hole.wav");
	paddleS = App->audio->LoadFx("pinball/paddleSound.wav");
	bounceS = App->audio->LoadFx("pinball/bounce.wav");
	domeS = App->audio->LoadFx("pinball/dome.wav");
	launchS = App->audio->LoadFx("pinball/Launch.wav");
	ballsafe = App->audio->LoadFx("pinball/ballsaved.wav");
	App->audio->PlayMusic("pinball/Nightmaren.ogg", 1.0f);

	//Lights Animations
	LightsAnim.PushBack({0,0,222,152 });
	LightsAnim.PushBack({ 222,0,222,152 });
	LightsAnim.PushBack({ 0,153,222,152 });
	LightsAnim.PushBack({ 222,153,222,152 });
	LightsAnim.PushBack({ 0,305,222,152 });
	LightsAnim.speed = 0.02f;

	//Triangles Animations
	LeftTriAnim.PushBack({ 0,0,21,40 });
	LeftTriAnim.PushBack({ 21,0,21,40 });
	LeftTriAnim.speed = 0.05f;
	RightTriAnim.PushBack({ 42,0,21,40 });
	RightTriAnim.PushBack({ 63,0,21,40 });
	RightTriAnim.speed = 0.05f;
	
			
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
	
	//create circle at the begining of the game if press 1
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && circle == nullptr && ballatcannon == false)
	{
		ballstart();
	}

	//create circle at mouse position
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 7, GROUND, BALL | GROUND));
		circles.getLast()->data->listener = this;
		circles.getLast()->data->listener = App->sensors;
		if (circle == nullptr)
		{
			circle = circles.getFirst()->data;
		}
	}

	//reset player's life
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		playerslife = 0;
		puntuation = 0;
		App->sensors->DiamondCount = 0;
		App->sensors->DomeCounter = 0;
		inmortal = false;		
	}

	//mode inmortal
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		inmortal = !inmortal;
	}

	//throw balls from cannon
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && ballatcannon == true)
	{
		App->audio->PlayFx(launchS);
		cannonanim = true;
		rdytostart = true;
		ballatcannon = false;
	}
	//MAKES PADDLES MOVE
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		App->physics->PaddleMoveL();
		App->audio->PlayFx(paddleS);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		App->physics->PaddleStopL();

	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		App->physics->PaddleMoveR();
		App->audio->PlayFx(paddleS);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		App->physics->PaddleStopR();

	}


	p2List_item<PhysBody*>* c = circlestodelete.getFirst();

	//Delete extra circles that fall down if the cannon is full
	while (c != NULL)
	{
		circles.del(circles.findNode(c->data));
		App->physics->DestroyBodys(*c->data);
		LOG("Destroyed body");
		c = c->next;
	}
	circlestodelete.clear();
	
	// render ground
	App->renderer->Blit(ground, 0, 0, { (256, 432, 0, 0) }, 1.0f);
	App->renderer->Blit(Tri, 57, 316, &(LeftTriAnim.GetCurrentFrame()), 1.0f);
	App->renderer->Blit(Tri, 163, 316, &(RightTriAnim.GetCurrentFrame()), 1.0f);
	App->renderer->Blit(LightsS, 14, 105, &(LightsAnim.GetCurrentFrame()), 1.0f);


	if (App->sensors->DomeCounter >= 1)
	{
		App->renderer->Blit(App->sensors->dome, 95, 226, NULL, 1.0f);
		

	}
	if (App->sensors->DomeCounter >= 2)
	{
		App->renderer->Blit(App->sensors->dome, 112, 230, NULL, 1.0f);
	}
	if (App->sensors->DomeCounter >= 3)
	{
		App->renderer->Blit(App->sensors->dome, 129, 227, NULL, 1.0f);
		App->sensors->AbleSpecial = true;
		//App->sensors->DiamondCount++;
	}
	
	SDL_Rect rect;
	rect.x = 14;
	rect.y = 0;
	rect.w = 14;
	rect.h = 14;
	App->renderer->Blit(diamonds, 113, 342, &rect, 1.0f);
	if (App->sensors->DiamondCount >= 1)
	{
		
		rect.x = 0;
		rect.y = 0;
		rect.w = 14;
		rect.h = 14;
		App->renderer->Blit(diamonds, 99, 336, &rect, 1.0f);
	}
	if (App->sensors->DiamondCount >= 2)
	{
		
		rect.x = 28;
		rect.y = 0;
		rect.w = 14;
		rect.h = 14;
		App->renderer->Blit(diamonds, 105, 321, &rect, 1.0f);


	}
	if (App->sensors->DiamondCount >= 3)
	{
		
		rect.x = 42;
		rect.y = 0;
		rect.w = 14;
		rect.h = 14;
		App->renderer->Blit(diamonds, 121, 321, &rect, 1.0f);


	}
	if (App->sensors->DiamondCount >= 4)
	{
		
		rect.x = 56;
		rect.y = 0;
		rect.w = 14;
		rect.h = 14;
		App->renderer->Blit(diamonds, 127, 336, &rect, 1.0f);


	}

	if (puntuation > bestpuntuation)
	{
		bestpuntuation = puntuation;
	}

	//Blit Left Paddles
	c = paddlesL.getFirst();
	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);

		App->renderer->Blit(paddletexture, x - 1, y, NULL, 1.0f, c->data->GetRotation(), 0, 0);
		c = c->next;
	}
	//Blit Right Paddles
	c = paddlesR.getFirst();
	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);

		App->renderer->Blit(paddle2texture, x, y, NULL, 1.0f, c->data->GetRotation(), 0, 0);
		c = c->next;
	}
	// Render all balls at background
	c = circles.getFirst();
	while (c != NULL)
	{

		int x, y;
		c->data->GetPosition(x, y);
		
		if (c->data->body->GetFixtureList()->GetFilterData().categoryBits == GROUND || c->data->body->GetFixtureList()->GetFilterData().categoryBits == START)
		{
			App->renderer->Blit(circletexture, x, y, NULL, 1.0f);
		}
		c = c->next;
	}
	

	//render lvl 1
	App->renderer->Blit(lvl1, 0, 15, { (256, 432, 0, 0) }, 1.0f);

	//render all balls at lvl 1
	c = circles.getFirst();
	while (c != NULL)
	{

		int x, y;
		c->data->GetPosition(x, y);
		
		if (c->data->body->GetFixtureList()->GetFilterData().categoryBits == LVL1)
		{
			App->renderer->Blit(circletexture, x, y, NULL, 1.0f);
		}
		c = c->next; 
	}

	//render lvl 2
	App->renderer->Blit(lvl2, 0, 0, { (256, 432, 0, 0) }, 1.0f);
	
	//render balls at lvl 2	
	c = circles.getFirst();
	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);

		if (c->data->body->GetFixtureList()->GetFilterData().categoryBits == LVL2)
		{
			App->renderer->Blit(circletexture, x, y, NULL, 1.0f);
		}
		c = c->next;
	}

	//move the ball back to the cannon
	if (playerslife < 3 || inmortal == true)
	{
		if (start == true && circle != nullptr)
		{
			if (circle->body->GetFixtureList()->GetFilterData().categoryBits == GROUND)
			{
				b2Vec2 a(PIXEL_TO_METERS(245), PIXEL_TO_METERS(410));
				circle->body->SetTransform(a, circle->body->GetAngle());
				
			}
			start = false;
			ballatcannon = true;

		}
		
		//throws the ball from cannon
		if (rdytostart == true && circle != nullptr)
		{
			
			if (circle->body->GetFixtureList()->GetFilterData().categoryBits == GROUND)
			{
				circle->body->SetLinearVelocity(b2Vec2(0, 0));
				circle->body->ApplyForceToCenter(b2Vec2(0, -50), true);
			}
			if (circle->body->GetFixtureList()->GetFilterData().categoryBits == START)
			{
				circle->body->SetLinearVelocity(b2Vec2(0, 0));
				circle->body->ApplyForceToCenter(b2Vec2(-50, -50), true);
				rdytostart = false;
			}
		}
	}
	//Blit top texture
	App->renderer->Blit(Top, 0, 0, { (256, 432, 0, 0) }, 1.0f);
	if (App->sensors->DomeCounter < 3)
	{
		App->renderer->Blit(Arrow, 108, 140, &(ArrowAnim.GetCurrentFrame()), 1.0f);
	}
	if (App->sensors->DomeCounter >= 3)
	{
		App->renderer->Blit(Arrow, 230, 170, &(ArrowAnim.GetCurrentFrame()), 1.0f);
	}
	//Blit win situation
	if (App->sensors->DiamondCount >= 4 && App->sensors->DomeCounter >= 1)
	{
		App->renderer->Blit(App->level->Win, 0, 100, NULL, 1.0f);
		App->renderer->Blit(Restart, 0, 170, &RestartAnim.GetCurrentFrame(), 1.0f);
	}
	//Blit cannon
	if (cannonanim == true)
	{
		App->renderer->Blit(cannon, 223, 270, &(CannonAnim.GetCurrentFrame()), 1.0f);
		if (CannonAnim.Finished())
		{
			CannonAnim.Reset();
			cannonanim = false;
		}
	}
	else
	{
		SDL_Rect stpcan;
		stpcan.x = stpcan.y = 0;
		stpcan.w = 31;
		stpcan.h = 36;

		App->renderer->Blit(cannon, 223, 270, &(stpcan), 1.0f);
		if (playerslife == 3 && inmortal == false)
		{
			App->renderer->Blit(Restart, 0, 150, &RestartAnim.GetCurrentFrame(), 1.0f);
		}
		
	}
	
	return UPDATE_CONTINUE;
}


void ModuleLevel::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->body != nullptr && bodyB->body != nullptr)
	{
		if (bodyB->body->GetType() == b2_dynamicBody && bodyA->body->GetFixtureList()->IsSensor() == true)
		{
			
			p2List_item<PhysBody*>* c = circles.getFirst();
			b2Filter balllvl;
			balllvl.categoryBits = bodyB->body->GetFixtureList()->GetFilterData().categoryBits;
			balllvl.maskBits = bodyB->body->GetFixtureList()->GetFilterData().maskBits;
			while (c != NULL)
			{
			if (c->data->body == bodyB->body)
				{
					if (bodyA == lvl1sensor)
					{
						balllvl.categoryBits = LVL1;
						balllvl.maskBits = LVL1 | BALL;
						puntuation += 1000;
					}
					if (bodyA == lvl2sensor0 || bodyA == lvl2sensor1)
					{
						balllvl.categoryBits = LVL2;
						balllvl.maskBits = LVL2 | BALL;
						puntuation += 1000;
					}
					if (bodyA == lvl1growndsensor0 || bodyA == lvl1growndsensor1 || bodyA == lvl2growndsensor0 || bodyA == lvl2growndsensor1 || bodyA == lvl2growndsensor2)
					{
						balllvl.categoryBits = GROUND;
						balllvl.maskBits = GROUND | BALL;
						puntuation += 1000;
					}
					if (bodyA == exitfromcanon)
					{
						balllvl.categoryBits = GROUND;
						balllvl.maskBits = GROUND | BALL;
						start = false;
						rdytostart = false;
						ballatcannon = false;
					}
					if (bodyA == holesensor)
					{
						if ((ballatcannon == false && playerslife < 3)||(ballatcannon == false && inmortal == true))
						{
							start = true;
							circle = c->data;
							playerslife++;
							if (playerslife < 3 || inmortal == true)
							{
								App->audio->PlayFx(ballsafe);
							}
							LOG("CREATED Body")
						}
						else
						{
							circlestodelete.add(c->data);
						}
					}
					if (bodyA == Canonsensor)
					{
						balllvl.categoryBits = START;
						balllvl.maskBits = START;
						circle->body->SetLinearVelocity(b2Vec2(0, 0));
					}
					c->data->body->GetFixtureList()->SetFilterData(balllvl);
				}
				c = c->next;
			}
		}
	}
}

void ModuleLevel::ballstart()
{
	circle = App->physics->CreateCircle(245, 410, 7, GROUND, BALL | GROUND);
	circles.add(circle);
	circle->listener = this;
	circle->listener = App->sensors;
	ballatcannon = true;
}


void ModuleLevel::createchains()
{
	int background[202] = {
		92, 472,
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
		149, 472
	};


	int groundtolvl1[40] = {
		65, 141,
		54, 152,
		52, 151,
		52, 147,
		54, 142,
		53, 131,
		53, 123,
		56, 115,
		65, 112,
		74, 114,
		80, 119,
		92, 132,
		87, 136,
		83, 131,
		78, 129,
		62, 114,
		57, 116,
		54, 123,
		54, 130,
		61, 136
	};

	int groundcircle[30] = {
		70, 59,
		60, 60,
		50, 64,
		41, 72,
		35, 81,
		32, 88,
		31, 96,
		34, 97,
		35, 94,
		37, 86,
		42, 77,
		52, 68,
		62, 63,
		70, 63,
		71, 61
	};

	int groundcircle1[34] = {
		91, 59,
		99, 60,
		107, 62,
		114, 67,
		119, 72,
		122, 79,
		125, 87,
		125, 96,
		122, 96,
		121, 95,
		120, 85,
		118, 77,
		113, 71,
		107, 67,
		98, 64,
		90, 63,
		89, 61
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
		155, 381,
		158, 383,
		160, 387,
		164, 386,
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

	int starttube[16] = {
		236, 271,
		236, 412,
		239, 416,
		249, 416,
		252, 412,
		252, 271,
		249, 264,
		239, 264
	};

	int canon[16] = {
		252, 290,
		248, 295,
		240, 295,
		236, 291,
		155, 218,
		172, 198,
		252, 275,
		252, 284
	};

	int circleboss[36] = {
		210, 101,
		220, 96,
		227, 86,
		228, 74,
		224, 63,
		216, 56,
		204, 54,
		192, 56,
		183, 63,
		179, 73,
		179, 83,
		182, 92,
		187, 96,
		192, 80,
		196, 74,
		205, 73,
		212, 80,
		212, 88
	};

	int leftbarrier[8] = {
		11, 378,
		34, 388,
		33, 392,
		11, 380
	};
	int rightbarrier[8] = {
		228, 382,
		229, 376,
		207, 387,
		208, 394
	};

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
	
	int level2[116] = {
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
		191, 82,
		187, 97,
		208, 100,
		212, 82,
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

	mapchains.add(App->physics->CreateChain(0, 0, background, 202, GROUND, GROUND | BALL));
	mapchains.add(App->physics->CreateChain(0, 16, level1, 132, LVL1, LVL1));
	mapchains.add(App->physics->CreateChain(0, 0, level2, 116, LVL2, LVL2 | BALL));
	mapchains.add(App->physics->CreateChain(0, 0, groundtolvl1, 40, GROUND, GROUND | BALL));
	mapchains.add(App->physics->CreateChain(0, 0, groundcircle, 30, GROUND, GROUND | BALL));
	mapchains.add(App->physics->CreateChain(0, 0, groundcircle1, 34, GROUND, GROUND | BALL));
	mapchains.add(App->physics->CreateChain(0, -1, groundleft, 26, GROUND, GROUND | BALL));
	mapchains.add(App->physics->CreateChain(0, -1, groundright, 24, GROUND, GROUND | BALL));
	mapchains.add(App->physics->CreateChain(0, 4, leftTri, 8, GROUND, GROUND | BALL));
	mapchains.add(App->physics->CreateChain(0, 4, rightTri, 8, GROUND, GROUND | BALL));
	mapchains.add(App->physics->CreateChain(0, 0, starttube, 16, GROUND, GROUND | BALL));
	mapchains.add(App->physics->CreateChain(0, 16, canon, 16, START, START));
	mapchains.add(App->physics->CreateChain(0, 0, circleboss, 36, GROUND, GROUND));
	mapchains.add(App->physics->CreateChain(0, 0, leftbarrier, 8, GROUND, GROUND));
	mapchains.add(App->physics->CreateChain(0, 0, rightbarrier, 8, GROUND, GROUND));
	
	//LEFT PADDLES
	paddlesL.add(App->physics->CreatePaddleL(90, 387, (30 * DEGTORAD), -30 * DEGTORAD, GROUND, GROUND | BALL));
	paddlesL.add(App->physics->CreatePaddleL(32, 185, (70 * DEGTORAD), 35 * DEGTORAD, GROUND, GROUND | BALL));
	//RIGHT PADDLES
	paddlesR.add(App->physics->CreatePaddleR(153, 387, (146 * DEGTORAD), 90 * DEGTORAD, GROUND, GROUND | BALL));
	paddlesR.add(App->physics->CreatePaddleR(222, 258, (100 * DEGTORAD), 40 * DEGTORAD, GROUND, GROUND | BALL));


}

void ModuleLevel::createsensors()
{
	int slvl[12] = {
		69, 139,
		71, 138,
		76, 133,
		77, 131,
		74, 133,
		71, 136
	};

	//rampa esquerra pujada
	lvl2sensor0 = App->physics->CreatePolySensor(-62, 30, slvl, 12, GROUND, GROUND | BALL);
	lvl2sensor0->listener = this;
	//rampa dreta pujada
	lvl2sensor1 = App->physics->CreatePolySensor(168, 30, slvl, 12, GROUND, GROUND | BALL);
	lvl2sensor1->listener = this;

	//rampa metall pujada
	lvl1sensor = App->physics->CreatePolySensor(-5, -5, slvl, 12, GROUND, GROUND | BALL);
	lvl1sensor->listener = this;

	//rampa metall baixada
	lvl1growndsensor0 = App->physics->CreatePolySensor(6, 6, slvl, 12, LVL1, LVL1 | BALL);
	lvl1growndsensor0->listener = this;

	lvl2growndsensor0 = App->physics->CreatePolySensor(-55, 50, slvl, 12, LVL2, LVL2 | BALL);
	lvl2growndsensor0->listener = this;

	lvl2growndsensor1 = App->physics->CreatePolySensor(163, 50, slvl, 12, LVL2, LVL2 | BALL);
	lvl2growndsensor1->listener = this;

	lvl2growndsensor2 = App->physics->CreatePolySensor(127, -45, slvl, 12, LVL2, LVL2 | BALL);
	lvl2growndsensor2->listener = this;

	//reset game sensors
	holesensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 20, 55, 10, GROUND, GROUND | BALL);
	holesensor->listener = this;

	Canonsensor = App->physics->CreateRectangleSensor(245, 285, 10, 10, GROUND, GROUND);
	Canonsensor->listener = this;

	exitfromcanon = App->physics->CreateRectangleSensor(195, 250, 10, 10, START, START);
	exitfromcanon->listener = this;

	int elvl1[12] = {
		40, 311,
		45, 308,
		51, 312,
		51, 310,
		45, 306,
		40, 309
	};
	//rampa metall forat
	lvl1growndsensor1 = App->physics->CreatePolySensor(0, 18, elvl1, 12, LVL1, LVL1 | BALL);
	lvl1growndsensor1->listener = this;
}