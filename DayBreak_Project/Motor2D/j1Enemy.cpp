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
	enemy_Collider_rect.x= EnemySettings.child("EnemySettings").child("Collider").attribute("x").as_int();
	enemy_Collider_rect.w= EnemySettings.child("EnemySettings").child("Collider").attribute("w").as_int();
	enemy_Collider_rect.h = EnemySettings.child("EnemySettings").child("Collider").attribute("h").as_int();

	enemy_collider = App->collisions->AddColliderEntity({ enemy_Collider_rect.x, enemy_rect.y,enemy_Collider_rect.w ,enemy_Collider_rect.h }, COLLIDER_ENEMY, this);

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

void j1Enemy::OnCollision(Collider *c1, Collider *c2) {

	if (c2->type == COLLIDER_STATIC || (c2->type == COLLIDER_BLINKING && App->map->TriggerActive == true)) {

		//Calculating an error margin of collision to avoid problems with colliders corners
		int error_margin = 0;

		if (Direction == EN_RIGHT)
			error_margin = (c1->rect.x + c1->rect.w) - c2->rect.x;
		else if (Direction == EN_LEFT)
			error_margin = (c2->rect.x + c2->rect.w) - c1->rect.x;

		//If the enemy falls less than a pixel over a collider, it falls (and it looks ok)
		if (error_margin > 1) {

			//Checking Y Axis Collisions
			if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y >= c2->rect.y + c2->rect.h - velocity.y) { 

				velocity.y = 0;
				position.y = c1->rect.y + c2->rect.h - (c1->rect.y - c2->rect.y) + 3;
			}
			else if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h <= c2->rect.y + velocity.y) { 
				falling = false;
				velocity.y = 0;
				position.y = c1->rect.y - ((c1->rect.y + c1->rect.h) - c2->rect.y);
			}
		}

		//Checking X Axis Collisions
		if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + velocity.x) { //Colliding Left (going right)

			velocity.x = 0;
			position.x -= (c1->rect.x + c1->rect.w) - c2->rect.x + 4;

		}
		else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x >= c2->rect.x + c2->rect.w - velocity.x) { //Colliding Right (going left)

			velocity.x = 0;
			position.x += (c2->rect.x + c2->rect.w) - c1->rect.x + 4;

		}
	}

}