#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"

struct SDL_Texture;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	p2List<PhysBody*> circles;
	PhysBody* ball;
	iPoint position;
	SDL_Texture* bola;
	PhysBody* Door;

	//booleanos de sensores
	bool death;
	bool stuck;
	bool closeDoor;
	bool telepR = false;
	bool telepL = false;
	bool muell ;
	bool door;


};