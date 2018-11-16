#include "j1EnemyFlyer.h"
#include "j1Scene.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Pathfinding.h"
#include "j1Player.h"


j1EnemyFlyer::j1EnemyFlyer(iPoint pos, const char* path, pugi::xml_document &EnemiesDocument) : j1Enemy(enemy_position, ENEMY_TYPE::FLYER) {

	enemy_position = pos;
	LoadEnemy(path, EnemiesDocument);

	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Idle");
	LoadPushbacks(Animation_node, Idle);
	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Run");
	LoadPushbacks(Animation_node, Run);
	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Dead");
	LoadPushbacks(Animation_node, Dead);

	current_animation = &Idle;
	enemy_velocity.y = 2;
	falling = true;

	PERF_START(pathfinding_recalc);
}

j1EnemyFlyer::~j1EnemyFlyer() {}

void j1EnemyFlyer::Update(float dt) {

	falling = true;
	
	if (falling == true) {

		enemy_velocity.y = 2;
		enemy_position.y += enemy_velocity.y;
	}
	else if (falling == false)
		enemy_velocity.y = 0;
	

	iPoint initial_pos = App->map->WorldToMap(enemy_position.x, (enemy_position.y + 30), App->scene->current_pathfinding_map);
	iPoint final_pos = App->map->WorldToMap(App->scene->Player->player_position.x, App->scene->Player->player_position.y, App->scene->current_pathfinding_map);

	//if (pathfinding_recalc.ReadMs() > 3000) {

		if (App->pathfinding->IsWalkable(initial_pos) && App->pathfinding->IsWalkable(final_pos)) {

			LOG("--PATHFIND----------------------------------------------");
			LOG("Starting Pos: %d,%d Ending Pos %d,%d", initial_pos.x, initial_pos.y, final_pos.x, final_pos.y);
			LOG("   Steps--");
			enemy_path = App->pathfinding->CreatePath(initial_pos, final_pos);
			last_enemy_path = enemy_path;
		}

		//PERF_START(pathfinding_recalc);
	//}
			
	if ((!App->pathfinding->IsWalkable(initial_pos) || !App->pathfinding->IsWalkable(final_pos)) && enemy_path != nullptr) 
		enemy_path->Clear();

	//for (uint i = 0; i < EnemyPath->Count(); ++i) {

//	x = EnemyPath->At(i)->x;
//	y = EnemyPath->At(i)->y;

//	iPoint path_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.x, App->scene->current_pathfinding_map);
//	iPoint path_coord_to_world = App->map->MapToWorld(path_coordinates.x, path_coordinates.y, App->scene->current_pathfinding_map);

//	iPoint path_coord_to_curr_map = App->map->WorldToMap(path_coord_to_world.x, path_coord_to_world.y, App->scene->current_map);

//	while (Enemy.x != path_coord_to_curr_map.x/* && Enemy.y != path_coord_to_curr_map.y*/) {

//		if (Enemy.x < path_coord_to_curr_map.x)
//			Enemy.x += 0.1f;
//		else if (Enemy.x > path_coord_to_curr_map.x)
//			Enemy.x -= 0.1f;

//		/*if (Enemy.y < path_coord_to_curr_map.y)
//			Enemy.y++;
//		else if (Enemy.y > path_coord_to_curr_map.y)
//			Enemy.y--;*/

//	}
//}


	if (enemy_path != nullptr) {

		for (uint i = 0; i < enemy_path->Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(enemy_path->At(i)->x, enemy_path->At(i)->y, App->scene->current_pathfinding_map);
			//App->render->Blit(debug_tex, pos.x, pos.y);
			pathrect.x = pos.x;
			pathrect.y = pos.y + 20;
			pathrect.w = App->scene->current_map.width;
			pathrect.h = App->scene->current_map.height;
			App->render->DrawQuad(pathrect, 255, 0, 0, 50);
		}
	}

	entity_collider->SetPos(enemy_position.x+40, enemy_position.y);
	App->render->Blit(Enemy_tex, enemy_position.x, enemy_position.y, &current_animation->GetCurrentFrame(), 1, 0, 0, 0, SDL_FLIP_NONE, 0.5);
}