#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1App.h"

struct SDL_Texture;

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

private:

	int Xvel;
	float Yvel;
	bool jumping;

	int max_height;
	int min_height;
	bool top_reached = false;
	bool bot_reached = false;

	SDL_Rect rect;
	iPoint position;

	//bool Death = false; //Uncomment this and the line in .cpp in order to make it work

};
#endif