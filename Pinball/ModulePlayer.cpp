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

	ball= App->physics->CreateCircle(385, 477, 9.5);
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
	
	if (telepR == true) {

		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(62), PIXEL_TO_METERS(370)), 0);
		telepR = false;
	}
	if (telepL == true) {

		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(335), PIXEL_TO_METERS(238)), 0);
		telepL = false;
	}

	if (stuck == true)
	{
		ball->body->ApplyAngularImpulse(32, true);
		stuck = false;
	}
	/*if (App->player->position.y < 300 && App->player->position.y > 287) {
					App->player->position.x = 327;
					App->player->position.y = 199;
				}
				if (App->player->position.y < 206 && App->player->position.y > 190) {
					App->player->position.x = 48;
					App->player->position.y = 293;
				}*/
	/*c->data->GetPosition(x, y);*/
	App->renderer->Blit(bola, x, y, NULL, 1.0f, ball->GetRotation());
	
	return UPDATE_CONTINUE;
}