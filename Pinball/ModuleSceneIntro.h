#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void map();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	//classe
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	p2List<PhysBody*> elephs;
	p2List<PhysBody*> walls;
	p2List<PhysBody*> wal1;
	PhysBody* sensor;
	PhysBody* right;
	PhysBody* right_circle;
	PhysBody* left;	
	PhysBody* left_circle;
	PhysBody* bumper;
	
	bool sensed;

	//Muelle/Spring
	PhysBody* muellesito;
	PhysBody* StaticMuelle;

	//Bonus
	PhysBody* Bonus1;
	PhysBody* Bonus2;
	PhysBody* Bonus3;
	p2List<PhysBody*>  SensorBonus1;
	PhysBody* SensorBonus2;
	PhysBody* SensorBonus3;

	SDL_Texture* Bonus;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* background;
	SDL_Texture* elephant;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on=true;

	int score = 0000;

	int scoreFont = -1;
	char scoreText[10] = { "\0" };
};
