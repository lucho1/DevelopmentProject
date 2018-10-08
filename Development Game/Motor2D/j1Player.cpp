#include "j1Player.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Colliders.h"

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

	//Load and start everything here
	position.x = position.y = 10;
	Xvel = 1;
	Yvel = 0.0f;
	gravity = 1;
	min_height = position.y;
	max_height = position.y - 300;
	jumping = false;

	Player.x = position.x;
	Player.y = position.y;
	Player.w = 32;
	Player.h = 64;

	PlayerCollider=App->colliders->AddCollider(Player, COLLIDER_PLAYER,this);

	return true;
}

bool j1Player::PreUpdate() {

	return true;
}

bool j1Player::Update(float dt) {

	position.y += gravity;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

		position.x += Xvel;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

		position.x -= Xvel;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		
		//if (!jumping&&!falling) {
		//	top = position.y - 80.0;
		//	jumping = true;
		//	Yvel = 2.0;
		//	}
		//}
		//if (jumping) {
		//	Yvel += 0.5*gravity/2;
		//	position.y -= Yvel;
		//	if (position.y <= top) {
		//		jumping = false;
		//		falling = true;
		//}
		//if (falling) {
		//		position.y += Yvel;
		//}
		//

		if (!jumping) {
			Yvel = 2.0f;
			bot_reached = false;
			top_reached = false;
			jumping = true;
		}
	

	if (jumping) {

		if (!top_reached) {

			position.y -= Yvel;

			if (position.y <= max_height)
				top_reached = true;
		}
		else if (!bot_reached) {

			position.y += Yvel;

			if (position.y >= min_height) {

				Yvel = 0.0f;
				min_height = position.y;
				max_height = position.y - 300;
				jumping = false;
				bot_reached = true;
				}	
			}
		}
	}
	

	Player.x = position.x;
	Player.y = position.y;

	App->render->DrawQuad(Player, 255, 0, 0, 200);
	PlayerCollider->SetPos(position.x, position.y);
	//if (Death)
	//	App->LoadGame("save_game.xml");

	return true;
}

bool j1Player::PostUpdate() {

	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {

	if ((c1->type == COLLIDER_TYPE::COLLIDER_STATIC) && (c2->type == COLLIDER_TYPE::COLLIDER_PLAYER) || (c2->type == COLLIDER_TYPE::COLLIDER_STATIC) && (c1->type == COLLIDER_TYPE::COLLIDER_PLAYER)) {
		gravity = 0;
	}

}