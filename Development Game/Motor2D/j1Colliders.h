#ifndef _J1COLLIDERS_
#define _J1COLLIDERS_
#include "j1Module.h"
#define MAX_COLLIDERS 700


enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_STATIC,
	COLLIDER_PLAYER,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module *callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;

	void changeCollider(COLLIDER_TYPE type_) {
		type = type_;
	}
};

class j1Colliders:public j1Module
{
public:
	j1Colliders();
	~j1Colliders();

	// Called before render is available
	//bool Awake(pugi::xml_node&);

	// Called before the first frame
	//bool Start();

	// Called each loop iteration
	bool PreUpdate();

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	void DebugDraw();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);

private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug=true;
};

#endif // !1