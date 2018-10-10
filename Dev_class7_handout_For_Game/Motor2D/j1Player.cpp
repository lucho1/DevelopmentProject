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

	//Load & Start everything here // Remember that gravity is a map parameter pls
	
	//Starting Position & Velocity FOR VEL & POS load them at player config pls
	position.x = 200;
	position.y = 0;
	Xvel = 1;

	//Player Rect
	player_rect.x = position.x;
	player_rect.y = position.y;
	player_rect.w = 32;
	player_rect.h = 64;

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

	player_rect.x = position.x;
	player_rect.y = position.y;

	App->render->DrawQuad(player_rect, 0, 255, 0, 200);

	return true;
}

bool j1Player::PostUpdate() {

	return true;
}