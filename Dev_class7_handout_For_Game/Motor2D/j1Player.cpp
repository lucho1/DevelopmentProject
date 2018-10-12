#include "j1Player.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Collisions.h"
#include "j1Map.h"


j1Player::j1Player() {}

j1Player::~j1Player() {}

bool j1Player::CleanUp() { 
	
	return true;
}

bool j1Player::Awake() {

	LOG("Player Module Loading");
	bool ret = true;
	return ret;
}

bool j1Player::Start() {

	//Load & Start everything here // Remember that gravity is a map parameter pls
	
	//Starting Position & Velocity FOR VEL & POS load them at player config pls
	position.x = App->render->camera.w / 2;
	position.y = App->render->camera.h / 2;
	Xvel = 8;
	Yvel =8;

	//Player Rect
	player_rect.x = position.x;
	player_rect.y = position.y;
	player_rect.w = 32;
	player_rect.h = 64;

	player_collider = App->collisions->AddCollider(player_rect, COLLIDER_PLAYER, this);

	return true;
}

bool j1Player::PreUpdate() {

	return true;
}

bool j1Player::Update(float dt) {

	//X axis Movement
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		position.x += Xvel;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		position.x -= Xvel;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		position.y -= Yvel;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		position.y += Yvel;

	player_rect.x = player_collider->rect.x = position.x;
	player_rect.y = player_collider->rect.y = position.y;

	iPoint vel;
	vel.x = Xvel;
	vel.y = Yvel;
	//COLLISION_POSITION col_pos = NONE;

	/*if (player_collider->PreCollision(vel, (player_rect.y + player_rect.w + height), player_rect, col_pos)) {
		
		
		Yvel = 0;
	}
	else
		Yvel = 1;*/

	App->render->DrawQuad(player_rect, 0, 0, 255, 200);

	return true;
}

bool j1Player::PostUpdate() {

	return true;
}