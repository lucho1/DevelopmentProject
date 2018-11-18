#ifndef _J1_ENEMY_FLYER_
#define _J1_ENEMY_FLYER_

#include "j1Enemy.h"
#include "Animation.h"
#include "j1Audio.h"

class j1EnemyFlyer : public j1Enemy
{

public:

	j1EnemyFlyer(iPoint pos, const char* path, pugi::xml_document &EnemiesDocument);
	~j1EnemyFlyer();

	void Update(float dt) override;
	void FixUpdate(float dt) override;
	void Draw(float dt);

	bool Detect_Area() override;
	bool Exploding_Area();
	bool Start_exploding = false;

	void Move(p2DynArray<iPoint>&path, float dt);

public:

	p2DynArray<iPoint>* enemy_path;
	
	SDL_Rect pathrect;

	iPoint Detect_Exploding_Range;
	j1Timer Explosion_Time;


private:

	Animation* current_animation = nullptr;
	Animation Idle;
	Animation Exploding;
	Animation Dead;
	Mix_Chunk *explosion;

};

#endif