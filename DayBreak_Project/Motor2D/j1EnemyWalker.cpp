#include "j1EnemyWalker.h"
#include "j1Scene.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "j1Particles.h"
#include "j1EntityManager.h"

#include "Brofiler/Brofiler.h"

j1EnemyWalker::j1EnemyWalker(iPoint pos, const char* path, pugi::xml_document &EnemiesDocument) : j1Enemy(enemy_position, ENEMY_TYPE::FLYER) {

	enemy_position = pos;
	LoadEnemy(path, EnemiesDocument);

	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Idle");
	LoadPushbacks(Animation_node, Idle);
	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Run");
	LoadPushbacks(Animation_node, Run);
	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Shoot");
	LoadPushbacks(Animation_node, Shoot_animation);
	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Dead");
	LoadPushbacks(Animation_node, Dead);

	current_animation = &Idle;
	onGround = false;
	Patrol_Range[2] = {};
	Able_to_Shoot = true;
	
	life = 20;

	Detect_Shoot_Range.x = EnemySettings.child("EnemySettings").child("Detection_Range").attribute("det_shooting_x").as_int();
	Detect_Shoot_Range.y = EnemySettings.child("EnemySettings").child("Detection_Range").attribute("det_shooting_y").as_int();

	Patrol_velocity.x = EnemySettings.child("EnemySettings").child("Velocity").attribute("patrol_x").as_int();
	Patrol_velocity.y = EnemySettings.child("EnemySettings").child("Velocity").attribute(" patrol_y").as_int();

	shoot = App->audio->LoadFx("audio/fx/Shoot2.wav");

}

j1EnemyWalker::~j1EnemyWalker() {}

void j1EnemyWalker::FixUpdate(float dt){

	BROFILER_CATEGORY("EnemyWalker FixUpdate", Profiler::Color::IndianRed);

	if (Detect_Area() && life > 0) {

		current_animation = &Idle;

		iPoint initial_pos = App->map->WorldToMap(enemy_position.x, (enemy_position.y + 30), App->scene->current_pathfinding_map);
		iPoint final_pos = App->map->WorldToMap(App->scene->Player->player_position.x, App->scene->Player->player_position.y, App->scene->current_pathfinding_map);

		if (App->pathfinding->IsWalkable(initial_pos) && App->pathfinding->IsWalkable(final_pos)) {

			enemy_path = App->pathfinding->CreatePath(initial_pos, final_pos);
			Move(*enemy_path, dt);
		}

	}
	else if (!Detect_Area() && life > 0) {

		Patrol(dt);
		if(enemy_path != nullptr) 
			enemy_path->Clear();
	}

	entity_collider->SetPos(enemy_position.x, enemy_position.y);

}

void j1EnemyWalker::Update(float dt) {

	BROFILER_CATEGORY("EnemyWalker Update", Profiler::Color::DarkOrange);
  
	if (life <= 0) {
		current_animation = &Dead;
		if(Dead.Finished())
			App->entity_manager->DestroyEntity(this);

	}

	if (enemy_path != nullptr && App->collisions->debug) {

		for (uint i = 0; i < enemy_path->Count(); ++i) {

			iPoint pos = App->map->MapToWorld(enemy_path->At(i)->x, enemy_path->At(i)->y, App->scene->current_pathfinding_map);
			pathrect.x = pos.x;
			pathrect.y = pos.y + 20;
			pathrect.w = App->scene->current_map.width;
			pathrect.h = App->scene->current_map.height;
			App->render->DrawQuad(pathrect, 255, 0, 255, 100);
		}
	}
	
	Draw(dt);
}

bool j1EnemyWalker::Detect_Area() {

	bool ret = false;	

	if ((App->scene->Player->player_position.x >= enemy_position.x - Detect_Range.x && App->scene->Player->player_position.x <= enemy_position.x + Detect_Range.x)
		&& (App->scene->Player->player_position.y >= enemy_position.y - Detect_Range.y && App->scene->Player->player_position.y <= enemy_position.y + Detect_Range.y)) {
		
		ret = true;
	}
	
	return ret;

}


void j1EnemyWalker::Patrol(float dt) {

	current_animation = &Run;

	Patrol_velocity *= dt;

	if (!Path_Found) {

		iPoint cell;
		Run.speed = 0.08;

		// South-Right
		if (!Limit_Right_Reached) {
			
			cell=(App->map->WorldToMap(enemy_position.x, (enemy_position.y + 30), App->scene->current_pathfinding_map));
			cell.x += 1;
			cell.y += 1;

			if (App->pathfinding->IsWalkable(cell)) {
				
				Limit_Right_Reached = true;
				Patrol_Range[0] = enemy_position.x;
				Current_Direction = LEFT;
			}
			else if (!App->pathfinding->IsWalkable(iPoint(cell.x,cell.y-1))) {
				
				Limit_Right_Reached = true;
				Patrol_Range[0] = enemy_position.x;
				Current_Direction = LEFT;
			}
			else
				enemy_position.x += Patrol_velocity.x;
		}

		else {
			
			if (!Limit_Left_Reached) {
				
				//South-Left
				cell = (App->map->WorldToMap(enemy_position.x, (enemy_position.y + 30), App->scene->current_pathfinding_map));
				cell.x -= 1;
				cell.y += 1;;
				
				if (App->pathfinding->IsWalkable(cell)) {
					
					Limit_Left_Reached = true;
					(Patrol_Range[1] = enemy_position.x);
					Path_Found = true;
					Current_Direction = RIGHT;
				}
				else if (!App->pathfinding->IsWalkable(iPoint(cell.x, cell.y - 1))) {
				
					Limit_Right_Reached = true;
					Patrol_Range[1] = enemy_position.x-60;
					Current_Direction = RIGHT;
					Path_Found = true;
				}
				else
					enemy_position.x -= Patrol_velocity.x;
				
			}
		}
	}
	else {

		switch (Current_Direction) {
		case RIGHT:

			if (enemy_position.x < Patrol_Range[0])
				enemy_position.x += Patrol_velocity.x;
			
			else if (enemy_position.x >= Patrol_Range[0])
				Current_Direction = LEFT;

			break;

		case LEFT:

			if (enemy_position.x > Patrol_Range[1]) 
				enemy_position.x -= Patrol_velocity.x;
		
			else if (enemy_position.x <=( Patrol_Range[1]))
				Current_Direction = RIGHT;

			break;
		}	
	}
};

void j1EnemyWalker::Move(p2DynArray<iPoint>&path, float dt) {

	//if (falling == true) {

	//	enemy_velocity.y = 2;
	//	enemy_position.y += enemy_velocity.y;
	//}
	//else if (falling == false)
	//	enemy_velocity.y = 0;

	if (onGround == false) {
		enemy_velocity.y = 4;
		enemy_position.y += enemy_velocity.y;
	}

	//enemy_velocity *= dt;
	Run.speed = 0.15f;

	Current_Direction = App->pathfinding->current_Direction(path);
	enemy_velocity.x = 5.5;
	switch (Current_Direction) {

	case RIGHT:
		if (!Shoot_Area()) {

			enemy_position.x += enemy_velocity.x;
			current_animation = &Run;
		}
		else
			Shoot();

		break;

	case LEFT:
		if (!Shoot_Area()) {

			enemy_position.x -= enemy_velocity.x;
			current_animation = &Run;
		}
		else
			Shoot();
		break;
	}

	onGround = false;
	//falling = true;

}

void j1EnemyWalker::Shoot() {
	
	current_animation = &Shoot_animation;

	if (Able_to_Shoot == true) {

		switch (Current_Direction) {
		case RIGHT:
			App->particles->AddParticle(App->particles->Enemy_Shoot, enemy_position.x +40, enemy_position.y +36, COLLIDER_ENEMY_BULLET, fPoint(937.5, 0), 0.8, SDL_FLIP_NONE); //Original speed = 15, 0
			break;
		case LEFT:
			App->particles->AddParticle(App->particles->Enemy_Shoot, enemy_position.x, enemy_position.y +36, COLLIDER_ENEMY_BULLET, fPoint(-937.5, 0), 0.8, SDL_FLIP_HORIZONTAL); //Same 15, 0
			break;
		}

		App->audio->PlayFx(shoot);
		Able_to_Shoot = false;
	}
	if (Shoot_animation.Finished()) {

		Shoot_animation.Reset();
		Able_to_Shoot = true;
	}
}

void j1EnemyWalker::Draw(float dt) {

	if (Current_Direction == RIGHT || Current_Direction == UP_RIGHT || Current_Direction == DOWN_RIGHT || Current_Direction == UP || Current_Direction == DOWN||Current_Direction==NONE)
		App->render->Blit(Enemy_tex, enemy_position.x, enemy_position.y, &current_animation->GetCurrentFrame(dt), 1, 0, 0, 0, SDL_FLIP_NONE, 0.5);
	
	else if (Current_Direction == LEFT || Current_Direction == UP_LEFT || Current_Direction == DOWN_RIGHT) 
		App->render->Blit(Enemy_tex, enemy_position.x, enemy_position.y, &current_animation->GetCurrentFrame(dt), 1, 0, 0, 0, SDL_FLIP_HORIZONTAL, 0.5);
	
}

bool j1EnemyWalker::Shoot_Area() {

	if (App->scene->Player->player_position.x >= enemy_position.x - Detect_Shoot_Range.x && App->scene->Player->player_position.x <= enemy_position.x + Detect_Shoot_Range.x) {

		return true;
	}

	return false;
};
