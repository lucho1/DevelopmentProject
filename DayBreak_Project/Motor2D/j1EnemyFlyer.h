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
	void Draw();
	void reset_Velocity() {
		enemy_velocity=iPoint(3, 3);
	}

	bool Detect_Area() override;
	bool Exploding_Area();
	bool Start_exploding = false;

	//void Patrol() override;

public:

	iPoint Detect_Exploding_Range;

	p2DynArray<iPoint>* enemy_path;
	p2DynArray<iPoint>* last_enemy_path;
	
	SDL_Rect pathrect;
	void Move(p2DynArray<iPoint>&path);

	j1Timer Explosion_Time;


private:

	Animation* current_animation = nullptr;
	Animation Idle;
	Animation Exploding;
	Animation Dead;

};

#endif