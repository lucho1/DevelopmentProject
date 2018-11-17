#include "j1EnemyWalker.h"
#include "j1Scene.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "j1Particles.h"

j1EnemyWalker::j1EnemyWalker(iPoint pos,const char* path, pugi::xml_document &EnemiesDocument) : j1Enemy(enemy_position, ENEMY_TYPE::FLYER) {
	
	enemy_position = pos;
	LoadEnemy(path, EnemiesDocument);

	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Idle");
	LoadPushbacks(Animation_node, Idle);
	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Run");
	LoadPushbacks(Animation_node, Run);
	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Shoot");
	LoadPushbacks(Animation_node, Shoot_animation);

	current_animation = &Idle;
	enemy_velocity = iPoint(3, 4);
	falling = true;
	Detect_Range = iPoint(600, 100);
	Detect_Shoot_Range = iPoint(300, 0);
	Patrol_Range[2] = {};
	Patrol_velocity = iPoint(2, 0);
	Able_to_Shoot = true;

	PERF_START(pathfinding_recalc);
}

j1EnemyWalker::~j1EnemyWalker() {}

void j1EnemyWalker::Update(float dt) {

	current_animation = &Idle;

	if (Detect_Area()) {

     	iPoint initial_pos = App->map->WorldToMap(enemy_position.x, (enemy_position.y + 30), App->scene->current_pathfinding_map);
		iPoint final_pos = App->map->WorldToMap(App->scene->Player->player_position.x, App->scene->Player->player_position.y, App->scene->current_pathfinding_map);

		if (App->pathfinding->IsWalkable(initial_pos) && App->pathfinding->IsWalkable(final_pos)) {

		enemy_path = App->pathfinding->CreatePath(initial_pos, final_pos);
			Move(*enemy_path);
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
	}
	else {
		Patrol();
	}

	entity_collider->SetPos(enemy_position.x, enemy_position.y);

	Draw();
}

bool j1EnemyWalker::Detect_Area() {

	bool ret = false;	

	if ((App->scene->Player->player_position.x >= enemy_position.x - Detect_Range.x && App->scene->Player->player_position.x <= enemy_position.x + Detect_Range.x)
		&& (App->scene->Player->player_position.y >= enemy_position.y - Detect_Range.y && App->scene->Player->player_position.y <= enemy_position.y + Detect_Range.y)) {
		ret = true;
	}
	
	return ret;

}


void j1EnemyWalker::Patrol() {
	current_animation = &Run;


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
			if (enemy_position.x < Patrol_Range[0]) {
				enemy_position.x += Patrol_velocity.x;
			}
			else if (enemy_position.x >= Patrol_Range[0])
				Current_Direction = LEFT;

			break;

		case LEFT:
			if (enemy_position.x > Patrol_Range[1]) {
				enemy_position.x -= Patrol_velocity.x;
			}
			else if (enemy_position.x <=( Patrol_Range[1]))
				Current_Direction = RIGHT;
			break;
		}	
	}


	// south)

};


bool j1EnemyWalker :: Shoot_Area() {
	
	bool ret = false;

	if (App->scene->Player->player_position.x >= enemy_position.x - Detect_Shoot_Range.x && App->scene->Player->player_position.x <= enemy_position.x + Detect_Shoot_Range.x) {
		ret = true;
	}

	return ret; 
};

void j1EnemyWalker::Move(p2DynArray<iPoint>&path) {



	//if (falling == true) {

	//	enemy_velocity.y = 2;
	//	enemy_position.y += enemy_velocity.y;
	//}
	//else if (falling == false)
	//	enemy_velocity.y = 0;
	Run.speed = 0.15f;

	Current_Direction = App->pathfinding->current_Direction(path);
	
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

	//falling = true;

}

void j1EnemyWalker::Shoot() {
	
	current_animation = &Shoot_animation;

	if (Able_to_Shoot == true) {
		switch (Current_Direction) {
		case RIGHT:
			App->particles->AddParticle(App->particles->Enemy_Shoot, enemy_position.x +40, enemy_position.y +36, COLLIDER_ENEMY_BULLET, iPoint(15, 0), 0.8);
			break;
		case LEFT:
			App->particles->AddParticle(App->particles->Enemy_Shoot, enemy_position.x, enemy_position.y +36, COLLIDER_ENEMY_BULLET, iPoint(-15, 0), 0.8, SDL_FLIP_HORIZONTAL);
			break;
		}
		Able_to_Shoot = false;
	}
	if (Shoot_animation.Finished()) {
		Shoot_animation.Reset();
		Able_to_Shoot = true;
	}

}

void j1EnemyWalker::Draw() {

	
			
	if (Current_Direction == RIGHT || Current_Direction == UP_RIGHT || Current_Direction == DOWN_RIGHT || Current_Direction == UP || Current_Direction == DOWN||Current_Direction==NONE)
		App->render->Blit(Enemy_tex, enemy_position.x, enemy_position.y, &current_animation->GetCurrentFrame(), 1, 0, 0, 0, SDL_FLIP_NONE, 0.5);
	else if (Current_Direction == LEFT || Current_Direction == UP_LEFT || Current_Direction == DOWN_RIGHT) {
		App->render->Blit(Enemy_tex, enemy_position.x, enemy_position.y, &current_animation->GetCurrentFrame(), 1, 0, 0, 0, SDL_FLIP_HORIZONTAL, 0.5);
	}
}