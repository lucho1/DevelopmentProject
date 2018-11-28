//#ifndef _J1OBJECTS_
//#define _J1OBJECTS_
//
//#include "j1Entity.h"
//#include "Animation.h"
//
//enum OBJECT_TYPE {
//
//	PUSHOFF,
//	PUSHON,
//	WIN,
//	BLINK_BLOCK
//	//OBJECT_UNKNOWN = 2
//
//};
//
//class j1Objects : public j1Entity
//{
//
//public:
//
//	j1Objects(iPoint pos, OBJECT_TYPE objType);
//	~j1Objects();
//	
//
//public:
//
//	//Create an object
//	j1Objects* CreateObject(iPoint pos, OBJECT_TYPE objType);
//
//	//Pushbacks loading
//	//void LoadPushbacks(pugi::xml_node node, Animation &animation);
//
//
//public:
//
//	OBJECT_TYPE type;
//	SDL_Texture *Obj_tex = nullptr;
//
//};
//
//#endif