#include "j1Player.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Scene.h"

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

	min_height = position.y;
	max_height = position.y - 300;
	jumping = false;

	rect.x = position.x;
	rect.y = position.y;
	rect.w = 32;
	rect.h = 64;

	return true;
}

bool j1Player::PreUpdate() {

	return true;
}

bool j1Player::Update(float dt) {


	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

		position.x += Xvel;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

		position.x -= Xvel;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {

		if (!jumping) {
			Yvel = 2.0f;
			bot_reached = false;
			top_reached = false;
			jumping = true;
		}
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
	

	rect.x = position.x;
	rect.y = position.y;

	App->render->DrawQuad(rect, 255, 0, 0, 200);

	//if (Death)
	//	App->LoadGame("save_game.xml");

	return true;
}

bool j1Player::PostUpdate() {

	return true;
}