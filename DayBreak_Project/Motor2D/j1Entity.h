//#ifndef _J1_ENTITY_
//#define _J1_ENTITY_
//
//#include "p2SString.h"
//#include "p2Point.h"
//#include "PugiXml\src\pugixml.hpp"
//#include "j1App.h"
//
//
//class j1EntityManager;
//struct SDL_Texture;
//struct SDL_Rect;
//
//enum class ENTITY_TYPE {
//
//	ENEMY_ENT,
//	PLAYER_ENT,
//	OBJECT_ENT,
//	UNKNOWN = 3
//
//};
//
//class j1Entity {
//
//public:
//
//	j1Entity(ENTITY_TYPE eType) : type(eType) {}
//
//	~j1Entity(){}
//
//public:
//
//	// Called before render is available
//	virtual void Awake() {}
//
//	// Called before the first frame if it was activated before that
//	virtual void Start() {}
//
//	// Called each loop iteration
//	virtual void FixUpdate(float dt) {}
//
//	// Called each logic iteration
//	virtual void Update(float dt) {}
//
//	// Called before all Updates
//	virtual void PreUpdate() {}
//
//	// Called before all Updates
//	virtual void PostUpdate() {}
//
//	// Called before quitting
//	virtual void CleanUp() {}
//
//	virtual void Destroy(){}
//
//public:
//
//	virtual bool Load(pugi::xml_node&)
//	{
//		return true;
//	}
//
//	virtual bool Save(pugi::xml_node&) const
//	{
//		return true;
//	}
//
//public:
//
//	virtual void OnCollision(Collider* c1, Collider* c2) {}
//
//	virtual void Move(float dt) {}
//	virtual void Draw(float dt) {}
//
//public:
//
//	int life;
//	ENTITY_TYPE type;
//	Collider* entity_collider = nullptr;
//
//};
//
//#endif