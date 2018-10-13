#ifndef _j1PLAYER_H_
#define _j1PLAYER_H

#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1App.h"

class j1Player : public j1Module
{

public:
	j1Player();
	virtual ~j1Player();

public:

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Load & Save
	bool Load(pugi::xml_node& data);

	// Save Game State
	bool Save(pugi::xml_node& data) const;

public:

	void OnCollision(Collider *c1, Collider *c2);

public:

	SDL_Rect player_rect;
	iPoint position;
	iPoint direction;
	iPoint velocity;

	bool jump;
	bool fall;

	Collider *player_collider = nullptr;

};

#endif