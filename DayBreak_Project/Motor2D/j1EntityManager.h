//#ifndef _J1_ENTITY_MANAGER
//#define _J1_ENTITY_MANAGER
//
//#include "j1Module.h"
//#include "j1Entity.h"
//#include "p2List.h"
//#include "j1Enemy.h"
//#include "PugiXml\src\pugixml.hpp"
//
//#define TIMES_PER_SEC 5
//#define PATHFINDING_PER_SEC 3
//
//class j1EntityManager : public j1Module
//{
//public:
//
//	j1EntityManager();
//	~j1EntityManager();
//
//public:
//
//	bool Awake();
//	bool Start();
//	bool PreUpdate();
//	bool Update(float dt);
//	bool CleanUp();
//
//
//public:
//
////	j1Entity *CreateEntity(ENTITY_TYPE eType);
////	void DestroyEntity(j1Entity *Entity);
////	void DesrtroyEnemies();
//
////	void LoadSceneEnemeies(pugi::xml_node &Enemy, ENEMY_TYPE type, const char* path, pugi::xml_document &EnemiesDocument);
//
//
//public:
//
////	p2List<j1Entity*> entities_list;
//
//	float accumulated_time;
//	float update_ms_cycle;
//	int times_per_sec;
//
//	float accumulated_time_enemies;
//	int paths_per_sec;
//	float update_enemies;
//
//	bool do_logic;
//	bool do_enemies_logic;
//
//};
//
//#endif