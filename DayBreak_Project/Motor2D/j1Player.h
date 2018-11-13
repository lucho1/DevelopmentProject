#ifndef _j1PLAYER_H_
#define _j1PLAYER_H

#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1App.h"
#include "Animation.h"
#include "p2DynArray.h"


enum direction {

	RIGHT,
	LEFT
};


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

	//Pushbacks loading
	void LoadPushbacks(pugi::xml_node node, Animation &animation);

	//Load & Save
	bool Load(pugi::xml_node& data);

	//Save Game State
	bool Save(pugi::xml_node& data) const;

	void RectPathfindingTest();
	SDL_Rect Enemy; //Rect to test
	p2DynArray<iPoint>* EnemyPath = nullptr;
	j1Timer recalc_path;
	SDL_Texture *debug_tex;


public:

	void OnCollision(Collider *c1, Collider *c2/* bool collX, bool collY*/);

public:

	iPoint position;
	int direction_x;
	fPoint velocity;
	fPoint MaxVelocity;
	fPoint initial_vel;
	fPoint acceleration;
	bool desaccelerating;
	bool God = false;
	
	bool jump = false;
	bool jump_falling = false;
	int auxY;
	
	bool fall = true;

	Animation*		current_animation = nullptr;
	Animation		Idle;
	Animation		Run;
	Animation		Jump;
	SDL_Texture*	Player_texture = nullptr;
	SDL_Rect player_rect;

	Collider *player_collider = nullptr;

	j1Timer desac;

private :

	pugi::xml_document	PlayerDocument;
	pugi::xml_node		Animation_node;
	pugi::xml_node		PlayerSettings;

};

#endif