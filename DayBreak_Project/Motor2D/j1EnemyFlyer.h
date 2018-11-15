#ifndef _J1_ENEMY_FLYER_
#define _J1_ENEMY_FLYER_

#include "j1Enemy.h"
#include "Animation.h"

class j1EnemyFlyer : public j1Enemy
{

public:

	j1EnemyFlyer(const char* path, pugi::xml_document &EnemiesDocument);
	~j1EnemyFlyer();

	void Update(float dt) override;

private:

	Animation* current_animation = nullptr;
	Animation Idle;
	Animation Run;
	Animation Dead;

};

#endif