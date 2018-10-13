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
	velocity.x = 1;
	velocity.y = 1;
	direction.x = 0;
	direction.y = 0;
	jump = false;
	fall = true;

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

	fall = true;

	//X axis Movement
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = 1;
		direction.x = 1;
		position.x += velocity.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = 1;
		direction.x = -1;
		position.x -= velocity.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		velocity.y = 2;
		direction.y = 1;
		position.y -= velocity.y;
	}
	if (fall) {
		velocity.y = 1;
		direction.y = -1;
		position.y += velocity.y;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE && !fall) 
		direction.y = 0;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
		direction.x = 0;

	player_rect.x = player_collider->rect.x = position.x;
	player_rect.y = player_collider->rect.y = position.y;

	//App->render->DrawQuad(player_rect, 0, 0, 255, 200);

	return true;
}

bool j1Player::PostUpdate() {

	App->render->DrawQuad(player_rect, 0, 0, 255, 200);
	return true;
}

void j1Player::OnCollision(Collider *c1, Collider *c2) {

	if (c1->type == COLLIDER_STATIC || c2->type == COLLIDER_STATIC) {

		if (c1->type == COLLIDER_STATIC) {

			if (direction.y == 1) {
				velocity.y = 0;
				position.y = c1->rect.y - c2->rect.h + 1;
			}

			if (direction.y == -1) {
				velocity.y = 0;
				position.y = c1->rect.y - c2->rect.h - 1;
				fall = false;
			}

			if (direction.x == 1) {

				if (c2->rect.x + c2->rect.w >= c1->rect.x && c2->rect.x + c2->rect.w <= c1->rect.x + velocity.x) {

					velocity.x = 0;
					position.x = c1->rect.x - c2->rect.w - 1;
				}

			}
			else if (direction.x == -1) {
				if (c2->rect.x <= c1->rect.x + c1->rect.w && c2->rect.x >= c1->rect.x + c1->rect.w - velocity.x) {

					velocity.x = 0;
					position.x = c1->rect.x + c1->rect.w + 1;
				}
			}
		}
		else if (c2->type == COLLIDER_STATIC) {

			if (direction.y == 1) {
				velocity.y = 0;
				position.y = c2->rect.y - c1->rect.h + 1;
			}

			if (direction.y == -1) {
				velocity.y = 0;
				position.y = c2->rect.y - c1->rect.h - 1;
				fall = false;
			}

			if (direction.x == 1) {

				if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + velocity.x) {

					velocity.x = 0;
					position.x = c2->rect.x - c1->rect.w - 1;
				}

			}
			else if (direction.x == -1) {
				if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x >= c2->rect.x + c2->rect.w - velocity.x) {

					velocity.x = 0;
					position.x = c2->rect.x + c2->rect.w + 1;
				}
			}

		}
	}

	/*if (c1->type == COLLIDER_FALL || c2->type == COLLIDER_FALL) { //When the player's saving/load works, uncomment this as a way of dying --> This mechanic is cool so we force the player to save before each decision

		App->LoadGame("save_game.xml");
	}*/
}