#ifndef _J1_ENEMY_WALKER_
#define _J1_ENEMY_WALKER_

#include "j1Enemy.h"
#include "Animation.h"

class j1EnemyWalker : public j1Enemy
{

public:

	j1EnemyWalker(const char* path, pugi::xml_document &EnemiesDocument);
	~j1EnemyWalker();

	void Update(float dt) override;

private:

	Animation* current_animation = nullptr;
	Animation Idle;
	Animation Run;
	Animation Shoot;

};

#endif