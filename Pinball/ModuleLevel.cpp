#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleLevel.h"
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

	App->renderer->camera.x = App->renderer->camera.y = 0;

	ground = App->textures->Load("pinball/wheel.png");
	lvl1 = App->textures->Load("pinball/crate.png");
	lvl2 = App->textures->Load("pinball/rick_head.png");

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

	App->renderer->Blit(ground, 0, 0, {(256, 432, 0, 0)}, 1.0f);
	
	/*if ()
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		circles.getLast()->data->listener = this;
	}

	if ()
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}*/

	if (atground == true)
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

		groundchain.add(App->physics->CreateChain(0, 0, background, 206));
		atground = false;
	}

	// Prepare for raycast ------------------------------------------------------

	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	//p2List_item<PhysBody*>* c = circles.getFirst();
	/*
	//while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if (c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(ground, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(lvl1, x, y, NULL, 1.0f, c->data->GetRotation());
		if (ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if (hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(lvl2, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if (ray_on == true)
	{
		fVector destination(mouse.x - ray.x, mouse.y - ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if (normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}
	*/
	return UPDATE_CONTINUE;
}
