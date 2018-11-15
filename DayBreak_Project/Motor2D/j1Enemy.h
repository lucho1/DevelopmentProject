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
	bool LoadEnemy(const char*file_name, pugi::xml_document &EnemiesDocument);

	j1Enemy* CreateEnemy(iPoint position, ENEMY_TYPE enemyType, const char* path, pugi::xml_document &EnemiesDocument); //Remember to put maps/ at path
	void j1Enemy::DestroyEnemy(j1Enemy *Enemy);

	//Pushbacks loading
	void LoadPushbacks(pugi::xml_node node, Animation &animation);

	//Pathfinding
	p2DynArray<iPoint>* FindPath(iPoint f_pos, p2DynArray<iPoint>*path);

public:

	bool falling = false;;

	pugi::xml_node		EnemySettings;
	pugi::xml_node		Animation_node;

	ENEMY_TYPE type;

	Enemy_direction Direction= EN_RIGHT;

	iPoint position;
	fPoint velocity;
	iPoint initial_velocity;

	SDL_Texture *Enemy_tex = nullptr;
	SDL_Rect enemy_rect;
	SDL_Rect enemy_Collider_rect;
};

#endif