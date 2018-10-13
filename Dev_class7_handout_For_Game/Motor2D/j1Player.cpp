#include "j1Player.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Collisions.h"
#include "j1Map.h"


j1Player::j1Player() {
  
	name.create("player");
	pugi::xml_parse_result result = AnimationDocument.load_file("PlayerSettings.xml");

	if (result == NULL) {
		LOG("The xml file containing the player tileset fails. Pugi error: %s", result.description());
	}

	Animation_node = AnimationDocument.child("config").child("AnimationCoords").child("Idle");
	Animation_node = AnimationDocument.child("config").child("AnimationCoords").child("Run");
	Animation_node = AnimationDocument.child("config").child("AnimationCoords").child("Jump");

}

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
	velocity.x = 8;
	velocity.y = 8;
	direction.x = 1;
	direction.y = 0;

	fall = true;

	//Player Rect
	player_rect.x = position.x;
	player_rect.y = position.y;
	player_rect.w = 32;
	player_rect.h = 64;

	player_collider = App->collisions->AddCollider(player_rect, COLLIDER_PLAYER, this);

	//Once player is created, saving game to have from beginning a save file to load whenever without giving an error
	App->SaveGame("save_game.xml");

	return true;
}

bool j1Player::PreUpdate() {

	return true;
}

bool j1Player::Update(float dt) {

	//X axis Movement
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = 8;
		direction.x = 1;
		position.x += velocity.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = 8;
		direction.x = -1;
		position.x -= velocity.x;
	}

	//Y axis Movement
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {

		if (!jump) {
			direction.y = 1;
			collide = false;
			velocity.y = 8;
			jump = true;

		}
	}

	if (jump) {

		fall = false;
		direction.y = 1;
		position.y -= velocity.y;
		velocity.y -= 0.2;

		if (velocity.y < 0) {
			direction.y = -1;
			fall = true;
			jump = false;	
		}
	}
	if (fall && !collide) {
		
		direction.y = -1;

		if(velocity.y < 8)
			velocity.y += 0.2;
		
		position.y += velocity.y;
	}
	if (!collide)
		fall = true;

	

	return true;
}

bool j1Player::PostUpdate() {

	player_rect.x = player_collider->rect.x = position.x;
	player_rect.y = player_collider->rect.y = position.y;
	App->render->DrawQuad(player_rect, 0, 0, 255, 200);
	return true;
}

bool j1Player::Load(pugi::xml_node& data)
{
	position.x = data.child("player").attribute("x").as_int();
	position.y = data.child("player").attribute("y").as_int();

	return true;
}

// Save Game State
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node pl = data.append_child("player");

	pl.append_attribute("x") = position.x;
	pl.append_attribute("y") = position.y;

	return true;
}

void j1Player::OnCollision(Collider *c1, Collider *c2) {

	//Checking collision with walls
	if (c2->type == COLLIDER_STATIC) {

		collide = true;

			if (direction.y == 1){
				
				position.y = c1->rect.y + c2->rect.h - (c1->rect.y - c2->rect.y) + 3;
				velocity.y = 0;

				if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + velocity.x) { //Direction.x = 1

					velocity.x = 0;
					position.x = c1->rect.x - ((c1->rect.x + c1->rect.w) - c2->rect.x) - 1;

				}
				else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x >= c2->rect.x + c2->rect.w - velocity.x) { //Direction.x = -1

					velocity.x = 0;
					//position.x = c1->rect.x - ((c1->rect.x + c1->rect.w) - c2->rect.x); //Put this when colliders rotated
					position.x = c1->rect.x + ((c2->rect.x + c2->rect.w) - c1->rect.x) + 1;

				}
				collide = false;
			}
			else if (direction.y == -1) {

				position.y = c1->rect.y - ((c1->rect.y + c1->rect.h) - c2->rect.y);
				velocity.y = 0;

				if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + velocity.x) { //Direction.x = 1

					velocity.x = 0;
					position.x = c1->rect.x - ((c1->rect.x + c1->rect.w) - c2->rect.x) - 1;
					
				}
				else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x >= c2->rect.x + c2->rect.w - velocity.x) { //Direction.x = -1

					velocity.x = 0;
					//position.x = c1->rect.x - ((c1->rect.x + c1->rect.w) - c2->rect.x); //Put this when colliders rotated
					position.x = c1->rect.x + ((c2->rect.x + c2->rect.w) - c1->rect.x) + 1;
					
				}

				collide = false;
			}

		/*else {

			if (direction.x == 1) {

				if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + velocity.x) {

					velocity.x = 0;
					position.x = c2->rect.x - c1->rect.w - 2;
				}

			}
			else if (direction.x == -1) {
				if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x >= c2->rect.x + c2->rect.w - velocity.x) {

					velocity.x = 0;
					position.x = c2->rect.x + c2->rect.w + 2;
				}
			}
		}*/
	}

	//Checking if falling
	if (c1->type == COLLIDER_FALL || c2->type == COLLIDER_FALL) //When the player's saving/load works, uncomment this as a way of dying --> This mechanic is cool so we force the player to save before each decision
		App->LoadGame("save_game.xml");

}






//if (c1->type == COLLIDER_STATIC) {
//
//	if (direction.y != 0) {
//
//		if (direction.y == 1)
//			return;
//		else if (direction.y == -1 && direction.x == 0) {
//			velocity.y = 0;
//			position.y = c1->rect.y - c2->rect.h - 1;
//			collide = true;
//		}
//		else if (direction.y == -1 && direction.x != 0) {
//
//			velocity.y = 0;
//			position.y = c1->rect.y - c2->rect.h - 1;
//			collide = true;
//
//			if (c2->rect.x + c2->rect.w >= c1->rect.x && c2->rect.x + c2->rect.w <= c1->rect.x + velocity.x) {
//
//				velocity.x = 0;
//				position.x = c1->rect.x - c2->rect.w - 2;
//			}
//			else if (c2->rect.x <= c1->rect.x + c1->rect.w && c2->rect.x >= c1->rect.x + c1->rect.w - velocity.x) {
//
//				velocity.x = 0;
//				position.x = c1->rect.x + c1->rect.w + 2;
//			}
//		}
//	}
//	else {
//
//		if (direction.x == 1) {
//
//			if (c2->rect.x + c2->rect.w >= c1->rect.x && c2->rect.x + c2->rect.w <= c1->rect.x + velocity.x) {
//
//				velocity.x = 0;
//				position.x = c1->rect.x - c2->rect.w - 2;
//			}
//
//		}
//		else if (direction.x == -1) {
//			if (c2->rect.x <= c1->rect.x + c1->rect.w && c2->rect.x >= c1->rect.x + c1->rect.w - velocity.x) {
//
//				velocity.x = 0;
//				position.x = c1->rect.x + c1->rect.w + 2;
//			}
//		}
//	}
//}