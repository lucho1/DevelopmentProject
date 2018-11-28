//#include "j1EntityManager.h"
//#include "p2Log.h"
//#include "j1Entity.h"
//#include "j1Scene.h"
//#include "j1Collisions.h"
//
//#include "Brofiler/Brofiler.h"
//
//j1EntityManager::j1EntityManager()
//{
//	name.create("entities");
//}
//
//
//j1EntityManager::~j1EntityManager() {}
//
//bool j1EntityManager::Awake() {
//
//	LOG("AWAKING ENTITY MANAGER");
//	times_per_sec = TIMES_PER_SEC;
//	paths_per_sec = PATHFINDING_PER_SEC;
//
//	update_ms_cycle = 1.0f / (float)times_per_sec;
//	update_enemies = 1.0f / (float)paths_per_sec;
//
//	return true;
//}
//
//bool j1EntityManager::Start() {
//
//	p2List_item<j1Entity*>* item;
//	item = entities_list.start;
//
//	while (item != nullptr) {
//
//		item->data->Start();
//		item = item->next;
//	}
//
//	return true;
//}
//
//bool j1EntityManager::PreUpdate() {
//
//	BROFILER_CATEGORY("Entities PreUpdate", Profiler::Color::GreenYellow);
//	do_logic = false;
//	return true;
//}
//
//bool j1EntityManager::Update(float dt) {
//
//	BROFILER_CATEGORY("Entities Update", Profiler::Color::OrangeRed);
//
//	accumulated_time += dt;
//	accumulated_time_enemies += dt;
//
//	if (accumulated_time >= update_ms_cycle)
//		do_logic = true;
//
//	if (accumulated_time_enemies >= update_enemies)
//		do_enemies_logic = true;
//
//	if (do_logic == true) {
//
//		p2List_item<j1Entity*>*item = entities_list.start;
//		for (; item != nullptr; item = item->next)
//			if(item->data->type != ENTITY_TYPE::ENEMY_ENT)
//				item->data->FixUpdate(dt);
//	
//	}
//
//	if (do_enemies_logic = true) {
//
//		p2List_item<j1Entity*>*item = entities_list.start;
//		for (; item != nullptr; item = item->next)
//			if (item->data->type == ENTITY_TYPE::ENEMY_ENT)
//				item->data->FixUpdate(dt);
//
//	}
//
//	accumulated_time -= update_ms_cycle;
//	accumulated_time_enemies -= update_enemies;
//
//	p2List_item<j1Entity*>*item = entities_list.start;
//	for (; item != nullptr; item = item->next)
//		item->data->Update(dt);
//	
//	return true;
//}
//
//bool j1EntityManager::CleanUp() {
//
//	LOG("Clean Up Entity Manager");
//
//	p2List_item<j1Entity*>* item;
//	item = entities_list.start;
//
//	while (item != nullptr) {
//		if (item->data->entity_collider != nullptr) {
//			item->data->entity_collider->to_delete = true;
//		}
//		RELEASE(item->data);
//		item = item->next;
//	}
//
//	entities_list.clear();
//
//	return true;
//}

//j1Entity *j1EntityManager::CreateEntity(ENTITY_TYPE eType) {
//
//	static_assert(ENTITY_TYPE::UNKNOWN == ENTITY_TYPE(3), "UPDATE ENTITY TYPES");
//	
//	j1Entity* Entity = nullptr;
//
//	switch (eType) {
//
//	case ENTITY_TYPE::ENEMY_ENT:
//		Entity = new j1Entity(ENTITY_TYPE::ENEMY_ENT);
//		break;
//	case ENTITY_TYPE::PLAYER_ENT:
//		Entity = new j1Entity(ENTITY_TYPE::PLAYER_ENT);
//		break;
//	default:
//		break;
//
//	}
//	
//	entities_list.add(Entity);
//	return Entity;
//}


//void j1EntityManager::DestroyEntity(j1Entity *Entity) {
//
//	p2List_item<j1Entity*>*item = entities_list.start;
//
//	while (item != nullptr) {
//
//		if (item->data == Entity) {
//			if(Entity->entity_collider!=nullptr)
//				Entity->entity_collider->to_delete = true;
//			entities_list.del(item);
//			break;
//		}
//
//		item = item->next;
//	}
//}

//void j1EntityManager::DesrtroyEnemies() {
//
//	p2List_item<j1Entity*>*item = App->entity_manager->entities_list.start;
//
//	while (item != nullptr) {
//		if (item->data->type == ENTITY_TYPE::ENEMY_ENT) {
//			if (item->data->entity_collider != nullptr)
//				item->data->entity_collider->to_delete = true;
//			App->entity_manager->entities_list.del(item);
//		}
//		item = item->next;
//	}
//}

//void j1EntityManager::LoadSceneEnemeies(pugi::xml_node &Enemy, ENEMY_TYPE type, const char* path, pugi::xml_document &EnemiesDocument) {
//
//	pugi::xml_node pos;
//
//	for (pos = Enemy.child("Position"); pos; pos = pos.next_sibling("Position")) {
//
//		int x = pos.attribute("x").as_int();
//		int y = pos.attribute("y").as_int();
//
//		j1Enemy* Enemy = nullptr;
//		Enemy->CreateEnemy(iPoint(x, y), type, path, EnemiesDocument);
//	}
//}