#ifndef _J1_ENEMY_FLYER_
#define _J1_ENEMY_FLYER_

#include "j1Enemy.h"
#include "Animation.h"

class j1EnemyFlyer : public j1Enemy
{

public:

	j1EnemyFlyer(iPoint pos, const char* path, pugi::xml_document &EnemiesDocument);
	~j1EnemyFlyer();

	void Update(float dt) override;

public:

	p2DynArray<iPoint>* enemy_path;
	
	SDL_Rect pathrect;
	j1PerfTimer pathfinding_recalc;

	

private:

	Animation* current_animation = nullptr;
	Animation Idle;
	Animation Run;
	Animation Dead;

};

#endif