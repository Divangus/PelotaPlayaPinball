#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

struct Sensores {
	enum col{
		COINS,
		DEAD,
		TUNEL,
	};
	PhysBody* Sensor;
	col tipo;
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
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;

public:
	//classe
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	p2List<PhysBody*> TheMap;
	p2List<PhysBody*> walls;
	p2List<PhysBody*> wal1;
	p2List<PhysBody*> bumper;
	
	PhysBody* sensor;
	PhysBody* right;
	PhysBody* right_circle;
	PhysBody* left;	
	PhysBody* left_circle;


	//Sensores
	PhysBody* Bon1;
	PhysBody* Bon2;
	PhysBody* Bon3;
	PhysBody* Death;
	PhysBody* TeleportR;
	PhysBody* TeleportL;
	PhysBody* LilBumperSensor;
	PhysBody* BumperSensors1;
	PhysBody* BumperSensors2;
	PhysBody* BumperSensors3;
	PhysBody* BumperSensors4;
	PhysBody* springSensor;
	bool sensed;
	SDL_Texture* BonusTex1;
	SDL_Texture* BonusTex2;
	SDL_Texture* BonusTex3;
	SDL_Texture* BonusUsedTex;

	//Muelle/Spring
	PhysBody* muellesito;
	PhysBody* StaticMuelle;
	SDL_Texture* springTex;

	//Bonus
	bool Bonus1;
	bool Bonus2;
	bool Bonus3;
	uint headshotFx;

	//Static Circles at the Top
	PhysBody* Static1;
	PhysBody* Static2;

	//Swirl
	SDL_Texture* swirlTex;

	//Rectangulo negro del Score
	SDL_Texture* BlackRectangle;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* flipperR;
	SDL_Texture* flipperL;
	SDL_Texture* background;
	SDL_Texture* elephant;
	uint music;
	p2Point<int> ray;
	bool ray_on=true;

	int score = 0000;
	int Bbonus = 0;
	int scoreFont = -1;
	char scoreText[10] = { "\0" };
};
