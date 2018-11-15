#ifndef _J1_ENEMY_
#define _J1_ENEMY_

#include "j1Entity.h"
#include "p2DynArray.h"
#include "Animation.h"


enum ENEMY_TYPE {

	NON = -1,
	FLYER,
	WALKER,
	UNKNOWN = 2
};

class j1Enemy : public j1Entity
{

public:

	j1Enemy(ENEMY_TYPE type_);
	~j1Enemy() {}

	virtual void CleanUp();

	virtual void Update(float dt) {}
	bool LoadEnemy(const char*file_name, pugi::xml_document &EnemiesDocument);

	//Pushbacks loading
	void LoadPushbacks(pugi::xml_node node, Animation &animation);
	p2DynArray<iPoint>* FindPath(iPoint f_pos, p2DynArray<iPoint>*path);

public:

	pugi::xml_node		EnemySettings;
	pugi::xml_node		Animation_node;

	ENEMY_TYPE type;

	iPoint position;
	fPoint velocity;
	iPoint initial_velocity;

	Collider* enemy_collider;

	SDL_Texture *Enemy_tex = nullptr;
	SDL_Rect enemy_rect;

};

#endif