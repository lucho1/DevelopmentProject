#include "j1EnemyWalker.h"
#include "j1Scene.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Player.h"

j1EnemyWalker::j1EnemyWalker(iPoint pos,const char* path, pugi::xml_document &EnemiesDocument) : j1Enemy(position, ENEMY_TYPE::FLYER) {
	
	position = pos;
	LoadEnemy(path, EnemiesDocument);

	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Idle");
	LoadPushbacks(Animation_node, Idle);
	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Run");
	LoadPushbacks(Animation_node, Run);
	Animation_node = EnemiesDocument.child("config").child("AnimationCoords").child("Dead");
	LoadPushbacks(Animation_node, Shoot);

	current_animation = &Idle;
	velocity.y = 2;
	falling = true;

	PERF_START(pathfinding_recalc);
}

j1EnemyWalker::~j1EnemyWalker() {}

void j1EnemyWalker::Update(float dt) {

	falling = true;

	if (falling == true) {
		velocity.y = 2;
		position.y += velocity.y;
	}
	else if (falling == false)
		velocity.y = 0;
	

	if (pathfinding_recalc.ReadMs() > 3000) {

		//patho = FindPath(App->player->position, patho);
		PERF_START(pathfinding_recalc);
	}

	for (uint i = 0; i < patho.Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(patho.At(i)->x, patho.At(i)->y, App->scene->current_pathfinding_map);
		//App->render->Blit(App->player->debug_tex, pos.x, pos.y);
		pathrect.x = pos.x;
		pathrect.y = pos.y;
		pathrect.w = App->scene->current_map.width;
		pathrect.h = App->scene->current_map.height;
		App->render->DrawQuad(pathrect, 0, 0, 255, 100);
	}
	

	entity_collider->SetPos(position.x, position.y);

	App->render->Blit(Enemy_tex, position.x, position.y, &current_animation->GetCurrentFrame(), 1, 0, 0, 0, SDL_FLIP_NONE, 0.5);

}

