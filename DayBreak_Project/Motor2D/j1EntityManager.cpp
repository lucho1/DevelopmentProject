#include "j1EntityManager.h"
#include "p2Log.h"
#include "j1EnemyFlyer.h"
#include "j1Entity.h"

j1EntityManager::j1EntityManager()
{
	name.create("entities");
}


j1EntityManager::~j1EntityManager() {}

bool j1EntityManager::Awake() {

	LOG("AWAKING ENTITY MANAGER");
	times_per_sec = TIMES_PER_SEC; //Read this with an XML
	update_ms_cycle = 1.0f / (float)times_per_sec;

	return true;
}

bool j1EntityManager::PreUpdate() {

	do_logic = false;
	return true;
}

bool j1EntityManager::Update(float dt) {

	/*accumulated_time += dt;

	if (accumulated_time >= update_ms_cycle)
		do_logic = true;

	if (do_logic == true) {*/
		p2List_item<j1Entity*>*item = entities_list.start;
		for (; item != nullptr; item = item->next)
			item->data->Update(dt);


		p2List_item<j1Enemy*>*item2 = enemies_list.start;
		for (; item2 != nullptr; item2 = item2->next)
			item2->data->Update(dt);

		accumulated_time = 0.0f;
	//}
	
	return true;
}

bool j1EntityManager::CleanUp() {

	LOG("Clean Up Entity Manager");

	p2List_item<j1Entity*>* item2;
	item2 = entities_list.start;

	while (item2 != nullptr) {

		RELEASE(item2->data);
		item2 = item2->next;
	}

	p2List_item<j1Entity*>* item;
	item = entities_list.start;

	while (item != nullptr) {

		RELEASE(item->data);
		item = item->next;
	}

	enemies_list.clear();
	entities_list.clear();

	return true;
}

j1Entity *j1EntityManager::CreateEntity(ENTITY_TYPE eType, ENEMY_TYPE enemyType) {

	static_assert(ENTITY_TYPE::UNKNOWN == ENTITY_TYPE(2), "UPDATE ENTITY TYPES");
	
	j1Entity* Entity = nullptr;

	switch (eType) {

	case ENTITY_TYPE::ENEMY_ENT:
		LOG("ENEMIES ARE NOT CREATED HERE");
		Entity = nullptr;
		break;
	case ENTITY_TYPE::PLAYER_ENT:
		Entity = new j1Entity(ENTITY_TYPE::PLAYER_ENT);
		break;
	default:
		break;

	}
	
	entities_list.add(Entity);
	return Entity;
}

j1Enemy* j1EntityManager::CreateEnemy(iPoint ePos, ENEMY_TYPE enemyType, const char* path, const char* document_path) {

	static_assert(ENEMY_TYPE::UNKNOWN == ENEMY_TYPE(2), "UPDATE ENEMY TYPES");

	pugi::xml_parse_result result = EnemiesDocument.load_file(document_path);

	if (result == NULL)
		LOG("The xml file containing the player tileset fails. Pugi error: %s", result.description());

	j1Enemy* Enemy = nullptr;

	switch (enemyType) {

	case ENEMY_TYPE::FLYER:
		Enemy = new j1EnemyFlyer(ePos, path, EnemiesDocument);
		break;
	case ENEMY_TYPE::WALKER:
		//Enemy = new j1EnemyWalker();
		break;
	default:
		break;

	}

	enemies_list.add(Enemy);
	return Enemy;
}

void j1EntityManager::DestroyEntity(j1Entity *Entity) {

	p2List_item<j1Entity*>*item = entities_list.start;

	while (item != nullptr) {

		if (item->data == Entity) {

			entities_list.del(item);
			RELEASE(item->data);
			break;
		}

		item = item->next;
	}
}