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

	void Draw(float dt);
	void Move(p2DynArray<iPoint>&path, float dt);
	void Update(float dt) override;
	void FixUpdate(float dt) override;

public:

	bool Detect_Area() override;
	bool Shoot_Area() override;
	bool Able_to_Shoot;

	void Patrol(float dt) override;
	void Shoot() override;

	bool Limit_Right_Reached = false;
	bool Limit_Left_Reached = false;

public:

	SDL_Rect pathrect;

	p2DynArray<iPoint>* enemy_path = nullptr;

	//Direction current_Direction;
	iPoint Detect_Shoot_Range;

	iPoint Patrol_velocity;
	int Patrol_Range[2];

private:

	Animation* current_animation = nullptr;
	Animation Idle;
	Animation Run;
	Animation Shoot_animation;
	Animation Dead;
};

#endif