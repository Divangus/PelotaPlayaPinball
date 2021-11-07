	#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"
#include "p2List.h"


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

	//Textures
	box = App->textures->Load("pinball/crate.png");
	flipperR = App->textures->Load("pinball/flipperR.png");
	flipperL = App->textures->Load("pinball/flipperL.png");
	Bonus= App->textures->Load("pinball/Bonus.png");
	background = App->textures->Load("pinball/pinballUltimate.png");//background
	BlackRectangle = App->textures->Load("pinball/BlackRectangle.png");
	springTex = App->textures->Load("pinball/spring.png");

	//music
	music = App->audio->PlayMusic("pinball/Audio/Chocobo.ogg");

	map();

	//Score
	char lookupTable[] = { "0123456789 0123456789" };
	scoreFont = App->fonts->Load("pinball/fonts/font1.png", lookupTable, 2);


	//flippers
	//right flippers
	right = App->physics->CreateRectangle(238, 703, 36, 12);
	right_circle = App->physics->CreateCircleStatic(238, 703, 6);

	b2RevoluteJointDef rightRevJoint;
	rightRevJoint.bodyA = right->body;
	rightRevJoint.bodyB = right_circle->body;
	rightRevJoint.localAnchorA.Set(PIXEL_TO_METERS(25), 0);
	rightRevJoint.localAnchorB.Set(0, 0);
	rightRevJoint.referenceAngle = 0 * DEGTORAD;
	rightRevJoint.enableLimit = true;
	rightRevJoint.lowerAngle = -45 * DEGTORAD;
	rightRevJoint.upperAngle = 45 * DEGTORAD;

	b2RevoluteJoint* joint_right = (b2RevoluteJoint*)App->physics->world->CreateJoint(&rightRevJoint);
	
	//left flippers
	left = App->physics->CreateRectangle(138, 703, 36, 12);
	left_circle = App->physics->CreateCircleStatic(138, 703, 6);

	b2RevoluteJointDef leftRevJoint;
	leftRevJoint.bodyA = left->body;
	leftRevJoint.bodyB = left_circle->body;
	leftRevJoint.localAnchorA.Set(PIXEL_TO_METERS(-25), 0);
	leftRevJoint.localAnchorB.Set(0, 0);
	leftRevJoint.referenceAngle = 0 * DEGTORAD;
	leftRevJoint.enableLimit = true;
	leftRevJoint.lowerAngle = -45 * DEGTORAD;
	leftRevJoint.upperAngle = 45 * DEGTORAD;

	b2RevoluteJoint* joint_left = (b2RevoluteJoint*)App->physics->world->CreateJoint(&leftRevJoint);

	//Muelle
	muellesito = App->physics->CreateRectangle(385, 664, 30, 20);
	StaticMuelle = App->physics->CreateRectangle(385, 744, 30, 20);
	StaticMuelle->body->SetType(b2_staticBody);

	//Joint del muelle
	b2DistanceJointDef MuelleJointDef;
	MuelleJointDef.bodyA = muellesito->body;
	MuelleJointDef.bodyB = StaticMuelle->body;
	MuelleJointDef.localAnchorA.Set(0, 0);
	MuelleJointDef.localAnchorB.Set(0, 0);
	MuelleJointDef.length = 2;
	MuelleJointDef.collideConnected = true;
	MuelleJointDef.frequencyHz = 4.0f;
	MuelleJointDef.dampingRatio = 0.5f;
	b2PrismaticJoint* MuelleJoint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&MuelleJointDef);

	//Sensor Teleport
	TeleportR = App->physics->CreateRectangleSensor(335, 200, 15, 9);
	TeleportR->listener = this;
	TeleportL = App->physics->CreateRectangleSensor(62, 320, 27, 9);
	TeleportL->listener = this;

	//Sensor de muerte
	Death = App->physics->CreateRectangleSensor(185, 837, 120, 15);
	Death->listener = this;

	//Sensor Bonus rojo en la izquierda
	Bon1= App->physics->CreateRectangleSensor(58, 415, 9, 15);
	Bon2 = App->physics->CreateRectangleSensor(49, 430, 9, 15);
	Bon3 = App->physics->CreateRectangleSensor(41, 445, 9, 15);

	Bon1->listener = this;
	Bon2->listener = this;
	Bon3->listener = this;

	//Sensor Bumper (in case ball gets stuck in the left bumper)
	BumperSensor=App->physics->CreateRectangleSensor(132, 220, 5, 5);
	BumperSensor->listener = this;

	//Spring sensor
	springSensor = App->physics->CreateRectangleSensor(310, 120, 6, 50);
	springSensor->listener = this;

	//Static Circles at the top
	Static1 = App->physics->CreateCircleStatic(186, 133, 24);
	Static2 = App->physics->CreateCircleStatic(264, 132, 24);

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
	//Draw Bonus
	App->renderer->Blit(Bonus, 58, 415, NULL, 1.0f);
	App->renderer->Blit(Bonus, 49, 430, NULL, 1.0f);
	App->renderer->Blit(Bonus, 41, 445, NULL, 1.0f);



	//Muelle/Spring
	
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		muellesito->body->ApplyForceToCenter(b2Vec2(0, 250), 1);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		right->body->ApplyForceToCenter(b2Vec2(0, -300), 1);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		left->body->ApplyForceToCenter(b2Vec2(0, -300), 1);
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		circles.getLast()->data->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	

	//background
	App->renderer->Blit(background, 0, 0, NULL, 1.0f);
	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

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

	//Black Rectangle
	App->renderer->Blit(BlackRectangle, 19, 52, NULL, 1.0f);

	App->renderer->Blit(springTex, 370, 635, NULL,1.0f);

	App->renderer->Blit(flipperR, 190, 700, NULL, 1.0f, right->GetRotation());

	App->renderer->Blit(flipperL, 93, 700, NULL, 1.0f, left->GetRotation());

	sprintf_s(scoreText, 10, "%8d", score);
	App->fonts->BlitText(20, 50, scoreFont, scoreText);

	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		LOG("add 100 score");
		score += 100;
	}

	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::map()
{
	
int mapchain[106] = {
		401, 753,
		401, 227,
		368, 149,
		322, 89,
		262, 51,
		210, 43,
		176, 45,
		132, 63,
		96, 97,
		80, 123,
		86, 150,
		118, 157,
		130, 152,
		133, 158,
		117, 197,
		130, 215,
		105, 279,
		97, 328,
		87, 344,
		97, 383,
		92, 386,
		69, 289,
		39, 289,
		68, 405,
		64, 406,
		60, 393,
		30, 452,
		30, 659,
		168, 779,
		168, 790,
		11, 789,
		16, 27,
		409, 27,
		409, 782,
		209, 789,
		213, 774,
		351, 663,
		351, 438,
		335, 380,
		339, 327,
		332, 298,
		352, 249,
		356, 218,
		338, 181,
		317, 215,
		305, 209,
		325, 173,
		313, 164,
		314, 141,
		347, 178,
		363, 213,
		369, 284,
		369, 753
};

TheMap.add(App->physics->CreateChain(0, 0, mapchain, 106));


	int wallRdw[12] = {
		233, 700,
		242, 710,
		325, 636,
		325, 503,
		314, 521,
		314, 633
	};
	walls.add(App->physics->CreateChain(0, 0, wallRdw, 12));

	int wallRup[12] = {
		249, 631,
		286, 600,
		286, 546,
		294, 533,
		294, 603,
		253, 637
	};
	walls.add(App->physics->CreateChain(0, 0, wallRup, 12));

	int trinR[6] = {
		233, 581,
		261, 530,
		263, 565
	};
	walls.add(App->physics->CreateReboundChain(0, 0, trinR, 6));

	int trinL[6] = {
		155, 576,
		126, 524,
		124, 559
	};
	walls.add(App->physics->CreateReboundChain(0, 0, trinL, 6));

	int wallLdw[12] = {
		144, 702,
		67, 636,
		67, 517,
		56, 504,
		56, 640,
		135, 711
	};
	walls.add(App->physics->CreateChain(0, 0, wallLdw, 12));

	int wallLup[12] = {
		134, 621,
		97, 590,
		97, 535,
		90, 525,
		90, 592,
		129, 626
	};
	walls.add(App->physics->CreateChain(0, 0, wallLup, 12));

	bumper.add(App->physics->CreateBumper(109, 126, 24));
	bumper.add(App->physics->CreateBumper(148, 245, 24));
	bumper.add(App->physics->CreateBumper(231, 394, 24));
	bumper.add(App->physics->CreateBumper(287, 279, 24));
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//int x, y;

	//p2List_item<Sensores*>* Sensores = TiposSensores.getFirst();

	//while (Sensores != NULL) {
	//	if (bodyA == Sensores->data->Sensor && bodyB->listener == (Module*)App->player) {
	//		if (Sensores->data->tipo == Sensores::COINS) {
	//			/*App->audio->PlayFx(bonus_fx);*/
	//		}
	//		if (Sensores->data->tipo == Sensores::DEAD) {
	//			if (App->player->position.y < 837) {
	//			}
	//		}
	//		if (Sensores->data->tipo == Sensores::TUNEL) {
	//			
	//		}
	//	}
	//}
	
}
