#include "j1EnemyWalker.h"
#include "j1Scene.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Pathfinding.h"

j1EnemyWalker::j1EnemyWalker(iPoint pos,const char* path, pugi::xml_document &EnemiesDocument) : j1Enemy(enemy_position, ENEMY_TYPE::FLYER) {
	
	enemy_position = pos;
	LoadEnemy(path, EnemiesDocument);

	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Idle");
	LoadPushbacks(Animation_node, Idle);
	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Run");
	LoadPushbacks(Animation_node, Run);
	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Dead");
	LoadPushbacks(Animation_node, Shoot);

	current_animation = &Idle;
	enemy_velocity.y = 2;
	falling = true;

	PERF_START(pathfinding_recalc);
}

j1EnemyWalker::~j1EnemyWalker() {}

void j1EnemyWalker::Update(float dt) {

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


	if (enemy_path != nullptr) {

		for (uint i = 0; i < enemy_path->Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(enemy_path->At(i)->x, enemy_path->At(i)->y, App->scene->current_pathfinding_map);
			//App->render->Blit(debug_tex, pos.x, pos.y);
			pathrect.x = pos.x;
			pathrect.y = pos.y + 50;
			pathrect.w = App->scene->current_map.width;
			pathrect.h = App->scene->current_map.height;
			App->render->DrawQuad(pathrect, 0, 255, 0, 50);
		}
	}
	

	entity_collider->SetPos(enemy_position.x, enemy_position.y);

	App->render->Blit(Enemy_tex, enemy_position.x, enemy_position.y, &current_animation->GetCurrentFrame(), 1, 0, 0, 0, SDL_FLIP_NONE, 0.5);

}

