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
	
	muell = true;
	death = false;
	door = false;
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

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) { //reinicia la posicion del jugador
		death = true;
		
	}
	if (death == true)
	{
		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(385), PIXEL_TO_METERS(477)), 0);

		/*muell = true;*/
		muell = true;
		door = false;
		death = false;
		App->physics->world->DestroyBody(Door->body);
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
	if (closeDoor == true )
	{
		Door = App->physics->CreateRectangle(319, 122, 6, 50);
		
		Door->body->SetType(b2_staticBody);
		closeDoor = false;
		door = true;
		muell = false;
	}
	if (door==true)
	{
		App->renderer->DrawQuad({ 316,95,6,50 }, 0, 150, 150);

	}
	
	App->renderer->Blit(bola, x, y, NULL, 1.0f, ball->GetRotation());
	
	return UPDATE_CONTINUE;
}