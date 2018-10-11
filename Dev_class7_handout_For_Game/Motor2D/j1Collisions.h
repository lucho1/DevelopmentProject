#ifndef _J1COLLISIONS_H
#define _J1COLLISIONS_H

#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1App.h"


#define MAX_COLLIDERS 500

enum COLLIDER_TYPE {

	COLLIDER_NONE = -1,
	COLLIDER_STATIC,
	COLLIDER_UNACTIVE,
	COLLIDER_PLAYER,
	COLLIDER_MAX
};

enum COLLISION_POSITION {

	RIGHT = 0,
	LEFT,
	UP,
	NONE
};

struct Collider {

	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type = COLLIDER_NONE;
	j1Module *callback = nullptr;

	Collider(SDL_Rect rect, COLLIDER_TYPE type, j1Module *callback = nullptr) :
		rect(rect), type(type), callback(callback) {}
	
	void SetPos(int x, int y) { rect.x = x; rect.y = y; }
	bool CheckCollision(const SDL_Rect &r) const;
	void ChangeCollider(COLLIDER_TYPE type_) { type = type_; }
	bool PreCollision(iPoint Vel, int h, const SDL_Rect &r, COLLISION_POSITION cpos);

};

class j1Collisions : public j1Module {


public:

	j1Collisions();
	~j1Collisions();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

public:

	void DebugDraw();
	Collider* AddCollider(SDL_Rect r, COLLIDER_TYPE type, j1Module*callback = nullptr);
	void AssignMapColliders();

private:

	Collider* colliders[MAX_COLLIDERS];
	bool debug = true;
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];

	
};

#endif