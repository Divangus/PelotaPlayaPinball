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
	circles.add(App->physics->CreateCircle(385, 477, 10));
	circles.getLast()->data->listener = this;

	
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
	
	return UPDATE_CONTINUE;
}