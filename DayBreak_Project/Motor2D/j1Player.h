#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1Entity.h"
#include "j1App.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "j1Audio.h"


enum direction {

	pl_RIGHT,
	pl_LEFT,
};
enum State {

	pl_IDLE,
	pl_RUN,
	pl_JUMP,
	pl_FALL
};


class j1Player : public j1Entity
{

public:

	j1Player(iPoint pos);
	virtual ~j1Player();

public:

	// Called before the first frame
	void Start() override;

	// Called each loop iteration
	void Update(float dt) override;

	// Called before quitting
	void CleanUp() override;

	void FixUpdate(float dt) override;

public:

	//Pushbacks loading
	void LoadPushbacks(pugi::xml_node node, Animation &animation);

	//Load & Save
	bool Load(pugi::xml_node& data);

	//Save Game State
	bool Save(pugi::xml_node& data) const;

	//Load player stats
	void LoadPlayer(const char *file_name);

	void Draw(float dt);
	void HandleInput(float dt);


public:

	void OnCollision(Collider *c1, Collider *c2);
	j1Player* CreatePlayer(iPoint pos/*, const char* path, pugi::xml_document &PlayerDocument*/); //Remember to put maps/ at path

public:

	//Player & Gun states
	int angle;
	State state;
	bool Shooting;
	bool jump = false;
	bool doublejump = true;
	bool jump_falling = false;
	int auxY;


	iPoint player_position;
	iPoint Gun_position;
	iPoint Adjusting_Gun_position;

	int direction_x;

	fPoint player_velocity;
	fPoint MaxVelocity;
	fPoint initial_vel;

	fPoint acceleration;
	bool desaccelerating;
	

	bool active;
	bool fall = true;
	bool God = false;

	j1Timer desac;

private:

	Mix_Chunk *shoot_pl;


public:

	//Animations Gun & Player
	Animation*		current_animation = nullptr;
	Animation*		Gun_current_animation = nullptr;
	Animation		Idle;
	Animation		Run;
	Animation		Jump;
	Animation		Fall;
	Animation		Gun_Idle;
	Animation		Gun_Run;
	Animation		Gun_Shot;
	Animation		Gun_None;
	Animation		Dead;

	//Textures/Rects Gun & Player
	SDL_Texture*	Player_texture = nullptr;
	SDL_Texture*	Gun_Texture = nullptr;
	
	SDL_Rect player_rect;
	SDL_Rect Gun_Rect;

	//Player Collider
	Collider *player_collider = nullptr;
	Collider *Last_collided = nullptr;
	

private :

	pugi::xml_document	PlayerDocument;
	pugi::xml_node		Animation_node;
	pugi::xml_node		PlayerSettings;

};

#endif