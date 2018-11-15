#include "j1EnemyWalker.h"
#include "j1Scene.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Player.h"

j1EnemyWalker::j1EnemyWalker(const char* path, pugi::xml_document &EnemiesDocument) : j1Enemy(position, ENEMY_TYPE::FLYER) {

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
	

	entity_collider->SetPos(position.x + 40, position.y);

	App->render->Blit(Enemy_tex, position.x, position.y, &current_animation->GetCurrentFrame(), 1, 0, 0, 0, SDL_FLIP_NONE, 0.5);

}

