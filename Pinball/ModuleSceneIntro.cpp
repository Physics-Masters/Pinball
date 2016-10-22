#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleLevel.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/ball.png");
	paddle = App->textures->Load("pinball/paddle.png");
	paddle2 = App->textures->Load("pinball/paddle2.png");
	box = App->textures->Load("pinball/crate.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	
	
	//LEFT PADDLES
	paddlesL.add(App->physics->CreatePaddleL(90, 387, (40 * DEGTORAD), -30 * DEGTORAD, GROUND, GROUND|BALL));
	paddlesL.add(App->physics->CreatePaddleL(32, 185, (70 * DEGTORAD), 35 * DEGTORAD, GROUND, GROUND | BALL));
	paddlesR.add(App->physics->CreatePaddleR(150, 387, (146 * DEGTORAD), 78 * DEGTORAD, GROUND, GROUND | BALL));
	paddlesR.add(App->physics->CreatePaddleR(222, 258, (100 * DEGTORAD), 40 * DEGTORAD, GROUND, GROUND | BALL));
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}
	//MAKES PADDLES MOVE
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		App->physics->PaddleMoveL();
	}
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_UP)
	{
		App->physics->PaddleStopL();

	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		App->physics->PaddleMoveR();
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
	{
		App->physics->PaddleStopR();

	}
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 7, GROUND, BALL|GROUND));
		circles.getLast()->data->listener = App->level;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		//boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50, BOX, BOX|BALL|GROUND));
	}
		
	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = paddlesL.getFirst();
	/*while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		
		App->renderer->Blit(paddle, x-1, y, NULL, 1.0f, c->data->GetRotation(),0,0);
		c = c->next;
	}*/
	 c = paddlesR.getFirst();
	 while (c != NULL)
	 {
		 int x, y;
		 c->data->GetPosition(x, y);

		 App->renderer->Blit(paddle2, x, y, NULL, 1.0f, c->data->GetRotation(), 0, 0);
		 c = c->next;
	 }
	/* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		//if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f );
		c = c->next;
	}
	*/
	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

}
