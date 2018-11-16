#ifndef _J1_ENEMY_
#define _J1_ENEMY_

#include "j1Entity.h"
#include "p2DynArray.h"
#include "Animation.h"
#include "p2List.h"

enum ENEMY_TYPE {

	NON = -1,
	FLYER,
	WALKER,
	UNKNOWN = 2
};

enum Enemy_direction {

	EN_RIGHT,
	EN_LEFT
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

public:

	bool falling = false;;

	ENEMY_TYPE type;
	Enemy_direction Direction= EN_RIGHT;

	iPoint enemy_position;
	fPoint enemy_velocity;
	iPoint initial_velocity;

	SDL_Texture *Enemy_tex = nullptr;
	SDL_Rect enemy_rect;
	SDL_Rect enemy_Collider_rect;

	//Path texture
	SDL_Texture *debug_tex;

public:

	pugi::xml_node		EnemySettings;
	pugi::xml_node		Animation_node;
};

#endif