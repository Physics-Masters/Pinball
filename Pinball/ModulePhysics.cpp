#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"

#include "ModuleRender.h"
#include "ModuleSensors.h"
#include "ModuleWindow.h"
#include "ModulePhysics.h"
#include "ModuleAudio.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);
		
	return true;
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}
//creates paddles
PhysBody* ModulePhysics::CreatePaddleL(int x, int y, float angd, float angu, uint16 categorybits, uint16 maskbits)
{
	//cirlce
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(4);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);
	
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 50;
	//polygon
	b2BodyDef body2;
	body2.type = b2_dynamicBody;
	body2.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b2 = world->CreateBody(&body2);

	b2PolygonShape shape2;

	int points[16] =
	{
		1, 8,
		1, 4,
		3, 2,
		28, 2,
		28, 4,
		11, 8,
		6, 10,
		2, 9
	};

	b2Vec2* p = new b2Vec2[8];

	for (uint i = 0; i < 8; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape2.Set(p, 8);
	delete[] p;
	b2FixtureDef fixture2;
	fixture2.shape = &shape2;
	fixture2.density = 2.0f;
	
	b2->CreateFixture(&fixture2);
	
	PhysBody* pbody2 = new PhysBody();
	pbody->body = b2;
	b2->SetUserData(pbody2);
	pbody->width = pbody->height = 0;
	//REVOLUTION JOINT
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = b;
	revoluteJointDef.bodyB = b2;
	revoluteJointDef.localAnchorA.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	revoluteJointDef.localAnchorB.Set(PIXEL_TO_METERS(4), PIXEL_TO_METERS(6));
	revoluteJointDef.referenceAngle = 0;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.enableMotor = false;
	revoluteJointDef.maxMotorTorque = 1800;
	revoluteJointDef.motorSpeed = -1500 * DEGTORAD;
	revoluteJointDef.enableLimit = true;
	revoluteJointDef.lowerAngle = angu;
	revoluteJointDef.upperAngle = angd;
	paddleList.add((b2RevoluteJoint*)world->CreateJoint(&revoluteJointDef));
	return pbody;
}
PhysBody* ModulePhysics::CreatePaddleR(int x, int y, float angd, float angu, uint16 categorybits, uint16 maskbits)
{
	//cirlce
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(4);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 50;
	//polygon
	b2BodyDef body2;
	body2.type = b2_dynamicBody;
	body2.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b2 = world->CreateBody(&body2);

	b2PolygonShape shape2;

	int points[14] =
	{
		1, 2,
		27, 2,
		29, 4,
		29, 8,
		27, 10,
		21, 10,
		1, 3
	};

	b2Vec2* p = new b2Vec2[7];

	for (uint i = 0; i < 7; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape2.Set(p, 7);
	delete[] p;
	b2FixtureDef fixture2;
	fixture2.shape = &shape2;
	fixture2.density = 2.0f;

	b2->CreateFixture(&fixture2);

	PhysBody* pbody2 = new PhysBody();
	pbody->body = b2;
	b2->SetUserData(pbody2);
	pbody->width = pbody->height = 0;
	//REVOLUTION JOINT
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = b;
	revoluteJointDef.bodyB = b2;
	revoluteJointDef.localAnchorA.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	revoluteJointDef.localAnchorB.Set(PIXEL_TO_METERS(25), PIXEL_TO_METERS(6));
	revoluteJointDef.referenceAngle = -120 * DEGTORAD;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.enableMotor = false;
	revoluteJointDef.maxMotorTorque = 1800;
	revoluteJointDef.motorSpeed = 1500 * DEGTORAD;
	revoluteJointDef.enableLimit = true;
	revoluteJointDef.lowerAngle = angu;
	revoluteJointDef.upperAngle = angd;
	paddleListR.add((b2RevoluteJoint*)world->CreateJoint(&revoluteJointDef));
	return pbody;
}
//moves paddles
void ModulePhysics::PaddleMoveL()
{
	//p2List_item<PhysBody*>*temp = App->scene_intro->paddles.getFirst()->data->body->GetJointList;
	p2List_item<b2RevoluteJoint*> *temp = paddleList.getFirst();
	while (temp != nullptr)
	{
		temp->data->EnableMotor(true);
		temp = temp->next;
	}
	//paddles->EnableMotor(true);
	
}
void ModulePhysics::PaddleStopL()
{
	p2List_item<b2RevoluteJoint*> *temp = paddleList.getFirst();
	while (temp!= nullptr)
	{
		temp->data->EnableMotor(false);
		temp = temp->next;
	}
}
void ModulePhysics::PaddleMoveR()
{
	//p2List_item<PhysBody*>*temp = App->scene_intro->paddles.getFirst()->data->body->GetJointList;
	p2List_item<b2RevoluteJoint*> *temp = paddleListR.getFirst();
	while (temp != nullptr)
	{
		temp->data->EnableMotor(true);
		temp = temp->next;
	}
	//paddles->EnableMotor(true);

}
void ModulePhysics::PaddleStopR()
{
	p2List_item<b2RevoluteJoint*> *temp = paddleListR.getFirst();
	while (temp != nullptr)
	{
		temp->data->EnableMotor(false);
		temp = temp->next;
	}
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, uint16 categorybits, uint16 maskbits)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.bullet = true;
	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.filter.categoryBits = categorybits;
	fixture.filter.maskBits = maskbits;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;
	
	return pbody;
}
PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, uint16 categorybits, uint16 maskbits)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.filter.categoryBits = categorybits;
	fixture.filter.maskBits = maskbits;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, uint16 categorybits, uint16 maskbits)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;
	fixture.filter.categoryBits = categorybits;
	fixture.filter.maskBits = maskbits;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreatePolySensor(int x, int y, int* points, int size, uint16 categorybits, uint16 maskbits)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape shape2;

	b2Vec2* p = new b2Vec2[size/2];

	for (int i = 0; i < size/2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape2.Set(p, size/2);

	b2FixtureDef fixture;
	fixture.shape = &shape2;
	fixture.density = 1.0f;
	fixture.isSensor = true;
	fixture.filter.categoryBits = categorybits;
	fixture.filter.maskBits = maskbits;
	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);

	return pbody;
}
PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, uint16 categorybits, uint16 maskbits)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(int i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);
	delete[] p;
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.filter.categoryBits = categorybits;
	fixture.filter.maskBits = maskbits;

	b->CreateFixture(&fixture);


	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	b2Body* body_clicked = nullptr;
	b2Vec2 mousepos;
	bool obj_int = false;
	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x*SCREEN_SIZE), METERS_TO_PIXELS(pos.y*SCREEN_SIZE), METERS_TO_PIXELS(shape->m_radius*SCREEN_SIZE), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}
			if (obj_int == false)
			{
				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
				{
					mousepos.x = PIXEL_TO_METERS(App->input->GetMouseX());
					mousepos.y = PIXEL_TO_METERS(App->input->GetMouseY());

					b2Transform trans = b->GetTransform();
					if (f->GetShape()->TestPoint(trans, mousepos) == true)
					{
						LOG("FOUND BODY");
						body_clicked = b;
						obj_int = true;
					}

				}
			}
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property

	if (obj_int == true)
	{
		b2MouseJointDef def;
		def.bodyA = ground;
		def.bodyB = body_clicked->GetFixtureList()->GetBody();
		def.target = mousepos;
		def.dampingRatio = 0.01f;
		def.frequencyHz = 2.0f;
		def.maxForce = 100.0f * body_clicked->GetFixtureList()->GetBody()->GetMass();
		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint)
	{
		mousepos.x = PIXEL_TO_METERS(App->input->GetMouseX());
		mousepos.y = PIXEL_TO_METERS(App->input->GetMouseY());
		mouse_joint->SetTarget(mousepos);
		App->renderer->DrawLine(METERS_TO_PIXELS(mousepos.x), METERS_TO_PIXELS(mousepos.y), METERS_TO_PIXELS(mouse_joint->GetAnchorB().x), METERS_TO_PIXELS(mouse_joint->GetAnchorB().y), 100, 100, 255);
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint)
	{
		obj_int = false;
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;

	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}


void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();
	if (physA == App->sensors->Dome)
	{
		if (App->sensors->DomeCounter < 3)
		{
			App->sensors->DomeCounter++;
			App->audio->PlayFx(App->level->domeS);
		}
		App->level->puntuation += 500;
		
	}
	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}

void ModulePhysics::DestroyBodys(PhysBody &body)
{
	
	if (body.body->GetJointList() != nullptr)
	{
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
	}
	
	world->DestroyBody(body.body);
}