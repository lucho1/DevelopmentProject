#ifndef _J1_ENEMY_
#define _J1_ENEMY_

#include "j1Entity.h"
#include "p2DynArray.h"
#include "Animation.h"
#include "p2List.h"
#include "j1Pathfinding.h"

enum ENEMY_TYPE {

	NON = -1,
	FLYER,
	WALKER,
	UNKNOWN = 2
};

class j1Enemy : public j1Entity
{

public:

	j1Enemy(iPoint pos, ENEMY_TYPE type_);
	~j1Enemy() {}

	void j1Enemy::OnCollision(Collider *c1, Collider *c2) override;
	
public:

	//Create/Destroy an Enemy
	j1Enemy* CreateEnemy(iPoint pos, ENEMY_TYPE enemyType, const char* path, pugi::xml_document &EnemiesDocument); //Remember to put maps/ at path
	void DestroyEnemy(j1Enemy *Enemy);

	//Load Enemy
	bool LoadEnemy(const char*file_name, pugi::xml_document &EnemiesDocument);
	//Pushbacks loading
	void LoadPushbacks(pugi::xml_node node, Animation &animation);

	virtual bool Shoot_Area() { return true; };

	virtual bool Detect_Area() { return true; };

	virtual void Patrol() {};

	virtual void Shoot() {};

public:

	bool falling = false;;
	bool Path_Found = false;

	int Patrol_Range[2];
	ENEMY_TYPE type;
	Direction Current_Direction;

	iPoint Detect_Range;
	iPoint Detect_Shoot_Range;
	iPoint enemy_position;
	iPoint enemy_velocity;
	iPoint initial_velocity;
	iPoint Patrol_velocity;


	bool firstiteration;

	SDL_Texture *Enemy_tex = nullptr;
	SDL_Rect enemy_rect;
	SDL_Rect enemy_Collider_rect;

	//Path texture
	SDL_Texture *debug_tex;

	bool CollidingX = false;
	int Y_Collider_Pos;

public:

	pugi::xml_node		EnemySettings;
	pugi::xml_node		Animation_node;
};

#endif