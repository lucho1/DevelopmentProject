#include "j1EnemyFlyer.h"
#include "j1Scene.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Pathfinding.h"
#include "j1Player.h"
#include "j1Particles.h"
#include "j1EntityManager.h"


j1EnemyFlyer::j1EnemyFlyer(iPoint pos, const char* path, pugi::xml_document &EnemiesDocument) : j1Enemy(enemy_position, ENEMY_TYPE::FLYER) {

	enemy_position = pos;
	LoadEnemy(path, EnemiesDocument);

	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Idle");
	LoadPushbacks(Animation_node, Idle);

	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Exploding");
	LoadPushbacks(Animation_node, Exploding);

	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Dead");
	LoadPushbacks(Animation_node, Dead);

	current_animation = &Idle;
	enemy_velocity.y = 3;
	enemy_velocity.x = 3;
	falling = true;
	Current_Direction = LEFT;

	Detect_Range = iPoint(600, 400);
	
	Detect_Exploding_Range = iPoint(200,200);
	Start_exploding = false;
	life = 20;

	//PERF_START(pathfinding_recalc);
}

j1EnemyFlyer::~j1EnemyFlyer() {}

void j1EnemyFlyer::Update(float dt) {

	//CollidingX = false;
	if(life>0){
	if (Detect_Area()) {
		//reset_Velocity();
		iPoint initial_pos = App->map->WorldToMap(enemy_position.x, (enemy_position.y + 15), App->scene->current_pathfinding_map);
		iPoint final_pos = App->map->WorldToMap(App->scene->Player->player_position.x, App->scene->Player->player_position.y, App->scene->current_pathfinding_map);

		//if (pathfinding_recalc.ReadMs() > 3000) {

		if (App->pathfinding->IsWalkable(initial_pos) && App->pathfinding->IsWalkable(final_pos)) {

			enemy_path = App->pathfinding->CreatePath(initial_pos, final_pos);
			Move(*enemy_path);
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
				pathrect.y = pos.y;
				pathrect.w = App->scene->current_map.width;
				pathrect.h = App->scene->current_map.height;
				App->render->DrawQuad(pathrect, 255, 0, 0, 50);
			}
		}
	  }
	entity_collider->SetPos(enemy_position.x + 40, enemy_position.y);
	Draw();
	}
	if (life <= 0) {
		App->entity_manager->DestroyEntity(this);
	}
	

}

bool j1EnemyFlyer::Detect_Area() {

	bool ret = false;

	if ((App->scene->Player->player_position.x >= enemy_position.x - Detect_Range.x && App->scene->Player->player_position.x <= enemy_position.x + Detect_Range.x)
		&& (App->scene->Player->player_position.y >= enemy_position.y - Detect_Range.y && App->scene->Player->player_position.y <= enemy_position.y + Detect_Range.y)) {
		ret = true;
	}

	return ret;

}


void j1EnemyFlyer::Move(p2DynArray<iPoint>&path) {

	Current_Direction = App->pathfinding->current_Direction(path);

	if (CollidingX && Detect_Area() && (RIGHT || LEFT || UP_RIGHT || UP_LEFT)) {

		if (enemy_position.y + enemy_Collider_rect.h >= Y_Collider_Pos) 
				Current_Direction = UP;
			CollidingX = false;
	}

	switch (Current_Direction) {
	case UP_RIGHT:
		enemy_position.x += enemy_velocity.x;
		enemy_position.y -= enemy_velocity.y;	
		break;
	case UP_LEFT:
		enemy_position.x -= enemy_velocity.x;
		enemy_position.y -= enemy_velocity.y;
		break;
	case DOWN_RIGHT:
		enemy_position.x += enemy_velocity.x;
		enemy_position.y += enemy_velocity.y;	
		break;
	case DOWN_LEFT:
		enemy_position.x -= enemy_velocity.x;
		enemy_position.y += enemy_velocity.y;
		break;
	case RIGHT:
		enemy_position.x += enemy_velocity.x;
		break;
	case LEFT:
		enemy_position.x -= enemy_velocity.x;
		break;
	case UP:
		enemy_position.y -= enemy_velocity.y;
		break;
	case DOWN:
		enemy_position.y += enemy_velocity.y;
		break;
	}

	if (!Exploding_Area()) {
		current_animation = &Idle;
		enemy_velocity = iPoint(3, 3);
	}
	else {
		Start_exploding = false;
		current_animation = &Exploding;
		enemy_velocity = iPoint(2, 2);

		if (Explosion_Time.ReadSec() >= 0.5f) {
			current_animation = &Dead;
		}
		if (Explosion_Time.ReadSec() > 1) {
			life = 0;
			App->particles->AddParticle(App->particles->Plasma_Explosion,enemy_position.x-90,enemy_position.y-70,COLLIDER_NONE,iPoint(0,0),1.5f);
			App->particles->AddParticle(App->particles->Plasma_Explosion, enemy_position.x - 150, enemy_position.y - 160, COLLIDER_NONE, iPoint(0, 0), 1.9f, SDL_FLIP_NONE, 350.0f);
			App->particles->AddParticle(App->particles->Plasma_Explosion, enemy_position.x - 70, enemy_position.y - 200, COLLIDER_NONE, iPoint(0, 0), 1.9f,SDL_FLIP_NONE,200.0f);
		}
	}

	if (Start_exploding == true) {
		PERF_START(Explosion_Time);
	}
}

void j1EnemyFlyer:: Draw() {

	if( Current_Direction == RIGHT || Current_Direction == UP_RIGHT||Current_Direction==DOWN_RIGHT|| Current_Direction== UP|| Current_Direction==DOWN|| Current_Direction==NONE)
		App->render->Blit(Enemy_tex, enemy_position.x, enemy_position.y, &current_animation->GetCurrentFrame(), 1, 0, 0, 0, SDL_FLIP_NONE, 0.5);
	else if (Current_Direction ==LEFT||Current_Direction ==UP_LEFT || Current_Direction==DOWN_RIGHT) {
		App->render->Blit(Enemy_tex, enemy_position.x, enemy_position.y, &current_animation->GetCurrentFrame(), 1, 0, 0, 0, SDL_FLIP_HORIZONTAL, 0.5);
	}

}


bool  j1EnemyFlyer::Exploding_Area() {

	bool ret = false;

	if ((App->scene->Player->player_position.x >= enemy_position.x - Detect_Exploding_Range.x && App->scene->Player->player_position.x <= enemy_position.x + Detect_Exploding_Range.x)
		&& (App->scene->Player->player_position.y >= enemy_position.y - Detect_Exploding_Range.y && App->scene->Player->player_position.y <= enemy_position.y + Detect_Exploding_Range.y)) {
		ret = true;
	}

	Start_exploding = true;

	return ret;

	
}