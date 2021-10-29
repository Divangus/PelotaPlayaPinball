	#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
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

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	Bonus= App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	background = App->textures->Load("pinball/pinballUltimate.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	map();

	right = App->physics->CreateRectangle(238, 703, 35, 12);
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
	
	left = App->physics->CreateRectangle(138, 703, 35, 12);
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

	//Muelle/Spring
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

	//Bonus en la izquierda
	SensorBonus1.add(App->physics->CreateRectangleSensor(85, 650, 20, 25));


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

	//Bonus Sensor
	p2List_item<PhysBody*>* a = SensorBonus1.getFirst();
	int x, y;
	a->data->GetPosition(x, y);
	App->renderer->Blit(Bonus, x, y, NULL, 1.0f, a->data->GetRotation());
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

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
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

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
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

void ModuleSceneIntro::map()
{
	


	// Pivot 0, 0
	/*int elephant[76] = {
		1488, 3295,
		1606, 3295,
		1611, 937,
		1502, 675,
		1355, 440,
		1182, 277,
		1011, 197,
		835, 168,
		613, 215,
		488, 291,
		320, 477,
		326, 597,
		420, 622,
		511, 604,
		528, 631,
		442, 757,
		466, 902,
		477, 1026,
		420, 1091,
		382, 1304,
		326, 1304,
		377, 1528,
		284, 1160,
		144, 1153,
		266, 1615,
		240, 1573,
		108, 1815,
		100, 2615,
		684, 3095,
		677, 3162,
		591, 3268,
		962, 3271,
		877, 3164,
		833, 3108,
		1442, 2604,
		1437, 1682,
		1491, 1680,
		1482, 3280
	};

	elephs.add(App->physics->CreateChain(0, 0, elephant, 76));*/
	/*int elephant[106] = {
			848, 3102,
			1440, 2602,
			1431, 1680,
			1322, 1514,
			1344, 1304,
			1320, 1194,
			1411, 991,
			1422, 928,
			1382, 765,
			1348, 717,
			1265, 857,
			1225, 831,
			1291, 691,
			1257, 642,
			1271, 600,
			1385, 705,
			1474, 880,
			1485, 1331,
			1502, 1794,
			1504, 3156,
			1608, 3156,
			1616, 912,
			1416, 508,
			1204, 300,
			1024, 196,
			840, 176,
			688, 188,
			508, 272,
			352, 428,
			312, 520,
			344, 612,
			530, 613,
			472, 708,
			446, 826,
			483, 930,
			473, 1043,
			413, 1110,
			383, 1303,
			330, 1310,
			376, 1523,
			280, 1156,
			150, 1153,
			270, 1616,
			236, 1573,
			103, 1826,
			96, 2623,
			676, 3093,
			682, 3137,
			34, 3111,
			28, 51,
			1640, 51,
			1657, 3197,
			871, 3197
	};

	elephs.add(App->physics->CreateChain(0, 0, elephant, 106));*/
int elephant[106] = {
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
			111, 190,
			120, 254,
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

elephs.add(App->physics->CreateChain(0, 0, elephant, 106));

	int wallsup[46] = {
		1434, 1682,
		1360, 1534,
		1323, 1506,
		1348, 1287,
		1325, 1188,
		1339, 1156,
		1369, 1055,
		1402, 1011,
		1411, 935,
		1423, 866,
		1380, 758,
		1346, 720,
		1263, 864,
		1223, 825,
		1296, 684,
		1255, 656,
		1254, 557,
		1333, 650,
		1390, 734,
		1440, 809,
		1473, 928,
		1495, 1222,
		1500, 1677
	};

	walls.add(App->physics->CreateChain(0, 0, wallsup, 46));

	int wallRdw[12] = {
		1336, 1963,
		1340, 2515,
		991, 2813,
		1018, 2853,
		1370, 2545,
		1374, 1961
	};
	walls.add(App->physics->CreateChain(0, 0, wallRdw, 12));

	int wallRup[12] = {
		1219, 2090,
		1220, 2372,
		1059, 2510,
		1075, 2534,
		1249, 2389,
		1249, 2090
	};
	walls.add(App->physics->CreateChain(0, 0, wallRup, 12));

	int trinR[8] = {
		978, 2309,
		1121, 2044,
		1141, 2227,
		1021, 2332
	};
	walls.add(App->physics->CreateChain(0, 0, trinR, 8));

	int trinL[8] = {
		590, 2296,
		448, 2018,
		423, 2203,
		560, 2306
	};
	walls.add(App->physics->CreateChain(0, 0, trinL, 8));

	int wallLdw[12] = {
		214, 1982,
		217, 2576,
		562, 2889,
		600, 2846,
		261, 2545,
		261, 1986
	};
	walls.add(App->physics->CreateChain(0, 0, wallLdw, 12));

	int wallLup[12] = {
		329, 2114,
		329, 2414,
		505, 2565,
		524, 2536,
		361, 2405,
		362, 2118
	};
	walls.add(App->physics->CreateChain(0, 0, wallLup, 12));
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
