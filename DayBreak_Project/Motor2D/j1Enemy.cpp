#include "j1Enemy.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Collisions.h"
#include "j1EnemyFlyer.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1Scene.h"

j1Enemy::j1Enemy(ENEMY_TYPE type_) : j1Entity(ENTITY_TYPE::ENEMY_ENT) {}

void j1Enemy::CleanUp() {

	App->tex->UnLoad(Enemy_tex);
	if (Enemy_tex != nullptr)
		RELEASE(Enemy_tex);
}

bool j1Enemy::LoadEnemy(const char*file_name, pugi::xml_document &EnemiesDocument) {

	Enemy_tex = App->tex->Load((file_name));

	EnemySettings = EnemiesDocument.child("config");

	//Starting Position & Velocity
	initial_velocity.x = EnemySettings.child("EnemySettings").child("Velocity").attribute("velocity.x").as_int();
	initial_velocity.y = EnemySettings.child("EnemySettings").child("Velocity").attribute("velocity.y").as_int();

	enemy_rect.x = 0;
	enemy_rect.y = 0;
	enemy_rect.w = EnemySettings.child("image").attribute("w").as_int();
	enemy_rect.h = EnemySettings.child("image").attribute("h").as_int();

	enemy_collider = App->collisions->AddColliderEntity({ enemy_rect.x + 50, enemy_rect.y, (enemy_rect.w - 65), (enemy_rect.h) - 28 }, COLLIDER_ENEMY, this);

	return true;
}


p2DynArray<iPoint>* j1Enemy::FindPath(iPoint f_pos, p2DynArray<iPoint>*path) {

	iPoint Enemy_WorldtoPFMap = App->map->WorldToMap(position.x, position.y, App->scene->current_pathfinding_map);
	iPoint Dest_WorldtoPFMap = App->map->WorldToMap(f_pos.x, f_pos.y, App->scene->current_pathfinding_map);

	iPoint initial_pos = App->map->WorldToMap(position.x, position.y, App->scene->current_pathfinding_map);
	iPoint final_pos = App->map->WorldToMap(f_pos.x, f_pos.y, App->scene->current_pathfinding_map);

	App->pathfinding->CreatePath(initial_pos, final_pos);

	path = new p2DynArray<iPoint>(App->pathfinding->GetLastPath());

	return path;
}

void j1Enemy::LoadPushbacks(pugi::xml_node node, Animation &animation) {

	animation.loop = node.attribute("loop").as_bool();
	animation.speed = node.attribute("speed").as_float();
	SDL_Rect rect;

	for (node = node.child("Pushback"); node; node = node.next_sibling("Pushback")) {

		rect.x = node.attribute("x").as_int();
		rect.y = node.attribute("y").as_int();
		rect.w = node.attribute("w").as_int();
		rect.h = node.attribute("h").as_int();
		animation.PushBack({ rect });
	}
}