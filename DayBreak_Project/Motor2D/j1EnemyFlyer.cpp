#include "j1EnemyFlyer.h"
#include "j1Scene.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Pathfinding.h"


j1EnemyFlyer::j1EnemyFlyer(iPoint pos, const char* path, pugi::xml_document &EnemiesDocument) : j1Enemy(position, ENEMY_TYPE::FLYER) {

	position = pos;
	LoadEnemy(path, EnemiesDocument);

	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Idle");
	LoadPushbacks(Animation_node, Idle);
	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Run");
	LoadPushbacks(Animation_node, Run);
	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Dead");
	LoadPushbacks(Animation_node, Dead);

	current_animation = &Idle;
	velocity.y = 2;
	falling = true;

	PERF_START(pathfinding_recalc);
}

j1EnemyFlyer::~j1EnemyFlyer() {}

void j1EnemyFlyer::Update(float dt) {

	/*if (position.x < App->player->position.x) {
		position.x += 1;
	}
	else if (position.x > App->player->position.x) {
		position.x -= 1;
	}*/

	falling = true;
	
	if (falling == true) {
		velocity.y = 2;
		position.y += velocity.y; 
	}
	else if (falling == false)
		velocity.y = 0;
	

	iPoint initial_pos = App->map->WorldToMap(position.x, (position.y + 30), App->scene->current_pathfinding_map);
	iPoint final_pos = App->map->WorldToMap(App->player->position.x, App->player->position.y, App->scene->current_pathfinding_map);

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
			App->render->Blit(App->player->debug_tex, pos.x, pos.y);
			/*pathrect.x = pos.x;
			pathrect.y = pos.y + 50;
			pathrect.w = App->scene->current_map.width;
			pathrect.h = App->scene->current_map.height;
			App->render->DrawQuad(pathrect, 255, 0, 0, 50);*/
		}
	}

	entity_collider->SetPos(position.x+40, position.y);
	App->render->Blit(Enemy_tex, position.x, position.y, &current_animation->GetCurrentFrame(), 1, 0, 0, 0, SDL_FLIP_NONE, 0.5);
}