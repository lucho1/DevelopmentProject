#ifndef _J1_ENEMY_WALKER_
#define _J1_ENEMY_WALKER_

#include "j1Enemy.h"
#include "Animation.h"
#include "j1Pathfinding.h"


class j1EnemyWalker : public j1Enemy
{

public:

	j1EnemyWalker(iPoint pos, const char* path, pugi::xml_document &EnemiesDocument);
	~j1EnemyWalker();

	void Draw();
	void Move(p2DynArray<iPoint>&path);
	bool Detect_Area() override;

	p2DynArray<iPoint> patho;

	SDL_Rect pathrect;
	j1PerfTimer pathfinding_recalc;

	void Update(float dt) override;

	p2DynArray<iPoint>* enemy_path = nullptr;
	p2DynArray<iPoint> last_enemy_path;

	//Direction current_Direction;


private:

	Animation* current_animation = nullptr;
	Animation Idle;
	Animation Run;
	Animation Shoot;

};

#endif