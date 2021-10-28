#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	//Textures
	bola = App->textures->Load("pinball/bola.png");
	//Rectangles and Circle
	circles.add(App->physics->CreateCircle(515, 826, 10));
	circles.getLast()->data->listener = this;
	muellesito=App->physics->CreateRectangle(515, 850, 30, 20);
	StaticMuelle = App->physics->CreateRectangle(515, 930, 30, 20);
	StaticMuelle->body->SetType(b2_staticBody);


	//Nacimiento del muelle
	b2DistanceJointDef MuelleJointDef;

	MuelleJointDef.bodyA = muellesito->body;

	MuelleJointDef.bodyB = StaticMuelle->body;

	MuelleJointDef.localAnchorA.Set(0, 0);

	MuelleJointDef.localAnchorB.Set(0, 0);

	MuelleJointDef.length = 2;

	MuelleJointDef.collideConnected = true;

	MuelleJointDef.frequencyHz =4.0f;
	MuelleJointDef.dampingRatio = 0.5f;
		
	
	b2PrismaticJoint* MuelleJoint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&MuelleJointDef);
	
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	p2List_item<PhysBody*>* c = circles.getFirst();
	int x, y;
	c->data->GetPosition(x, y);
	App->renderer->Blit(bola, x, y, NULL, 1.0f, c->data->GetRotation());

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		
		muellesito->body->ApplyForceToCenter(b2Vec2(0, 250), 1);
		
		
		
	}
	
	/*b2DistanceJointDef MuelleJoint;
	MuelleJoint.bodyA = muellesito->body;
	MuelleJoint.bodyB = StacticMuelle->body;*/

	return UPDATE_CONTINUE;
}