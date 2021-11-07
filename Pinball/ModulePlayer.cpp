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
	/*circles.add(App->physics->CreateCircle(px, py, 9.5));
	circles*/

	ball= App->physics->CreateCircle(px, py, 9.5);
	ball->listener = this;
	

	death = false;
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
	/*p2List_item<PhysBody*>* c = circles.getFirst();*/

	/*PhysBody* b=ball.;*/
	
	if (death == true)
	{
		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(385), PIXEL_TO_METERS(477)), 0);
		death = false;
	}
	int x, y;
	ball->GetPosition(x, y);
	

	/*c->data->GetPosition(x, y);*/
	App->renderer->Blit(bola, x, y, NULL, 1.0f, ball->GetRotation());
	
	return UPDATE_CONTINUE;
}