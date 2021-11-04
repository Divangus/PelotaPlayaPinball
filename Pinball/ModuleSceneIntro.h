#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

struct Sensores {
	PhysBody* Sensor;
};

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
	p2List<PhysBody*> BonusClass;
	SDL_Texture* Bonus;

	//Rectangulo negro del Score
	SDL_Texture* BlackRectangle;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* flipperR;
	SDL_Texture* flipperL;
	SDL_Texture* rick;
	SDL_Texture* background;
	SDL_Texture* elephant;
	uint bonus_fx;
	uint music;
	p2Point<int> ray;
	bool ray_on=true;

	int score = 0000;
	int Bbonus = 0;
	int scoreFont = -1;
	char scoreText[10] = { "\0" };
};
