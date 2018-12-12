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
#include "j1EntityManager.h"
#include "j1EnemyWalker.h"
#include "j1Particles.h"

j1Enemy::j1Enemy(iPoint pos, ENEMY_TYPE type_) : j1Entity(ENTITY_TYPE::ENEMY_ENT), enemy_position(pos)
{
}

bool j1Enemy::LoadEnemy(const char*file_name, pugi::xml_document &EnemiesDocument) {

	EnemySettings = EnemiesDocument.child("config");

	//Starting Position & Velocities
	initial_velocity.x = EnemySettings.child("EnemySettings").child("Velocity").attribute("initial_velocity.x").as_int();
	initial_velocity.y = EnemySettings.child("EnemySettings").child("Velocity").attribute("initial_velocity.y").as_int();

	enemy_velocity.x = EnemySettings.child("EnemySettings").child("Velocity").attribute("velocity.x").as_int();
	enemy_velocity.y = EnemySettings.child("EnemySettings").child("Velocity").attribute("velocity.y").as_int();

	//Detection Range
	Detect_Range.x = EnemySettings.child("EnemySettings").child("Detection_Range").attribute("det_x").as_int();
	Detect_Range.y = EnemySettings.child("EnemySettings").child("Detection_Range").attribute("det_y").as_int();


	enemy_rect.x = 0;
	enemy_rect.y = 0;
	enemy_rect.w = EnemySettings.child("image").attribute("w").as_int();
	enemy_rect.h = EnemySettings.child("image").attribute("h").as_int();

	enemy_Collider_rect.w= EnemySettings.child("EnemySettings").child("Collider").attribute("w").as_int();
	enemy_Collider_rect.h = EnemySettings.child("EnemySettings").child("Collider").attribute("h").as_int();

	entity_collider = App->collisions->AddColliderEntity({ enemy_Collider_rect.x, enemy_rect.y,enemy_Collider_rect.w ,enemy_Collider_rect.h }, COLLIDER_ENEMY, this);

	Enemy_tex = App->tex->Load(file_name);

	return true;
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

	p2List_item<j1Entity*>* item = App->entity_manager->entities_list.start;

	for (; item != nullptr; item = item->next) {

		if (c1->type == COLLIDER_ENEMY && c2->type == COLLIDER_PLAYER_BULLET) {

			if (Last_collided == c2) 
				return;
			
			life -= 5;
			Last_collided = c2;
			App->particles->AddParticle(App->particles->Blood, enemy_position.x, enemy_position.y, COLLIDER_NONE, fPoint(0, 0), 1.5f, SDL_FLIP_NONE);	
		}
		if (item->data->type == ENTITY_TYPE::ENEMY_ENT ) {

			if (c2->type == COLLIDER_STATIC || (c2->type == COLLIDER_BLINKING && App->map->TriggerActive == true)) {

				//Calculating an error margin of collision to avoid problems with colliders corners
				int error_margin = 0;

				if (Current_Direction == RIGHT)
					error_margin = (c1->rect.x + c1->rect.w) - c2->rect.x;
				else if (Current_Direction == LEFT)
					error_margin = (c2->rect.x + c2->rect.w) - c1->rect.x;

				//If the enemy falls less than a pixel over a collider, it falls (and it looks ok)
				//Checking Y Axis Collisions
				if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y >= c2->rect.y + c2->rect.h - enemy_velocity.y) {

					enemy_velocity.y = 0;
					enemy_position.y = c1->rect.y + c2->rect.h - (c1->rect.y - c2->rect.y) + 3;
				}
				else if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h <= c2->rect.y + enemy_velocity.y) {
					onGround = true;
					enemy_velocity.y = 0;
					enemy_position.y = c1->rect.y - ((c1->rect.y + c1->rect.h) - c2->rect.y);

				}
			
				//Checking X Axis Collisions
				if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + enemy_velocity.x) { //Colliding Left (going right)

					enemy_velocity.x = 0;
					enemy_position.x -= (c1->rect.x + c1->rect.w) - c2->rect.x + 4;
					CollidingX = true;
					Y_Collider_Pos = c2->rect.y;

				}
				else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x >= c2->rect.x + c2->rect.w - enemy_velocity.x) { //Colliding Right (going left)

					enemy_velocity.x = 0;
					enemy_position.x += (c2->rect.x + c2->rect.w) - c1->rect.x + 4;
					CollidingX = true;
					Y_Collider_Pos = c2->rect.y;

				}
			}
		}
	}
}

j1Enemy* j1Enemy::CreateEnemy(iPoint pos, ENEMY_TYPE enemyType, const char* path, pugi::xml_document &EnemiesDocument) {

	p2SString tmp("maps\\%s", path);

	static_assert(ENEMY_TYPE::UNKNOWN == ENEMY_TYPE(2), "UPDATE ENEMY TYPES");
	j1Enemy* Enemy = nullptr;
	switch (enemyType) {

	case ENEMY_TYPE::FLYER:
		Enemy = new j1EnemyFlyer(pos, tmp.GetString(), EnemiesDocument);
		break;
	case ENEMY_TYPE::WALKER:
		Enemy = new j1EnemyWalker(pos, tmp.GetString(), EnemiesDocument);
		break;
	default:
		break;
	}

	App->entity_manager->entities_list.add(Enemy);

	return Enemy;
}


//void j1Enemy::CleanUp() {
//
//	p2List_item<j1Entity*>*item = App->entity_manager->entities_list.start;
//
//	while (item != nullptr) {
//		if (item->data->type == ENTITY_TYPE::ENEMY_ENT){
//			App->entity_manager->entities_list.del(item);
//			break;
//		}
//		item = item->next;
//	}
//
//}