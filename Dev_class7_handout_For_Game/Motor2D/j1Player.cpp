#include "j1Player.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "Animation.h"
#include "j1Textures.h"

j1Player::j1Player() {
  
	name.create("player");
	pugi::xml_parse_result result = PlayerDocument.load_file("PlayerSettings.xml");

	if (result == NULL) {
		LOG("The xml file containing the player tileset fails. Pugi error: %s", result.description());
	}

	//Loading Settings
	Animation_node = PlayerDocument.child("config").child("AnimationCoords").child("Idle");
	LoadPushbacks(Animation_node, Idle);
	Animation_node = PlayerDocument.child("config").child("AnimationCoords").child("Run");
	LoadPushbacks(Animation_node, Run);
	Animation_node = PlayerDocument.child("config").child("AnimationCoords").child("Jump");
	LoadPushbacks(Animation_node, Jump);
	PlayerSettings = PlayerDocument.child("config").child("image");

	//Loading textures 
	
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

	//const char* path = PlayerSettings.child("image").attribute("source").as_string();
	p2SString tmp = ("maps\\Character_tileset.png");
	Player_texture = App->tex->Load(tmp.GetString());

	//Starting Position & Velocity FOR VEL & POS load them at player config
 	position.x = App->render->camera.w / 2;
	position.y = App->render->camera.h / 2;
	velocity.x = 8;
	velocity.y = 10;
	direction.x = 1;
	direction.y = -1;


	player_collider = App->collisions->AddCollider({ position.x +50, position.y, PlayerSettings.attribute("w").as_int() -50, PlayerSettings.attribute("h").as_int() }, COLLIDER_PLAYER, this);

	//Once player is created, saving game to have from beginning a save file to load whenever without giving an error
	App->SaveGame("save_game.xml");

	return true;
}

bool j1Player::PreUpdate() {

	return true;
}

bool j1Player::Update(float dt) {

	current_animation = &Idle;
	//X axis Movement
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = 8;
		direction.x = 1;
		position.x += velocity.x;
		current_animation = &Run;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = 8;
		direction.x = -1;
		position.x -= velocity.x;
		current_animation = &Run;
	}

	

	//Y axis Movement
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && !jump) {

		jump = true;
		jump_falling = false;
		auxY = position.y;
		velocity.y = 10;

	}
	//if (!double_jump && jump_counter == 0/*double_jump*/ && App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {

	//	double_jump = true;
	//	doblejump_falling = false;
	//	velocity.y = 8;
	//	direction.y = 0;
	//}

	//JUMP
	if (jump) {

		//double_jump = false;

		if (velocity.y >= 0 && !jump_falling) {

			
			fall = false;
			position.y -= velocity.y;
			velocity.y -= 0.2;
			direction.y = 1;

		/*	if (position.y <= auxY) 
				double_jump = false;*/
			
				//double_jump = true;
		}

		else if (velocity.y < 0) {
		
			jump_falling = true;
			fall = true;
			
			/*if (position.y >= auxY)
				double_jump = true;*/
				//double_jump = false;
			
		}
		
		/*if (position.y >= auxY) {
			jump = false;
			jump_falling = false;*/
			/*double_jump = true;
			jump_counter = 0;*/
		//}
	}

	//Doble Jump
	/*if (double_jump) {

		jump_counter++;
		velocity.y = 8;
		double_jump = true;

	}*/
	
	

	//FALL
	if (fall) {

		direction.y = -1;

		if (velocity.y < 10)
			velocity.y += 0.2;

		position.y += velocity.y;
	}

	//COLLIDE
	if (collide) {
		velocity.y = 0;
		fall = false;
	}
	else
		fall = true;



	//BLIT PLAYER
	if (direction.x == 1 || direction.x == 0) {
		player_collider->SetPos(position.x, position.y);
		App->render->Blit(Player_texture, position.x, position.y, &(current_animation->GetCurrentFrame()), 1, 0, 0, 0, SDL_FLIP_HORIZONTAL);
	}
	if (direction.x == -1) {
		player_collider->SetPos(position.x, position.y);
		App->render->Blit(Player_texture, position.x - 46, position.y, &(current_animation->GetCurrentFrame()));
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
		direction.x = 0;

	return true;
}

bool j1Player::PostUpdate() {

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

		if (direction.y != 0) {

			//Checking Y Axis Collisions
			if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y >= c2->rect.y + c2->rect.y - velocity.y) { //direction.y == 1
				velocity.y = 0;
				position.y = c1->rect.y + c2->rect.h - (c1->rect.y - c2->rect.y) + 3;
			}
			else if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h <= c2->rect.y + velocity.y) { /*direction.y == -1*/
				jump = false;
				velocity.y = 0;
				position.y = c1->rect.y - ((c1->rect.y + c1->rect.h) - c2->rect.y);
			}
		}
		if (direction.x != 0) {

			//Checking X Axis Collisions
			if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + velocity.x) { //Direction.x = 1

				velocity.x = 0;
				position.x -= (c1->rect.x + c1->rect.w) - c2->rect.x + 7;
			}
			else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x >= c2->rect.x + c2->rect.w - velocity.x) { //Direction.x = -1

				velocity.x = 0;
				position.x += (c2->rect.x + c2->rect.w) - c1->rect.x + 7;
			}

		}
	}


	//Checking if falling
	if (c1->type == COLLIDER_FALL || c2->type == COLLIDER_FALL) //This mechanic is cool so we force the player to save before each decision
		App->LoadGame("save_game.xml");

}


void j1Player::LoadPushbacks(pugi::xml_node node, Animation& animation) {

	animation.loop = node.attribute("loop").as_bool();
	animation.speed = node.attribute("speed").as_float();
	SDL_Rect rect;
	
	for (node = node.child("Pushback"); node; node = node.next_sibling("Pushback")) {

		rect.x = node.attribute("x").as_int();
		rect.y = node.attribute("y").as_int();
		rect.w = node.attribute("w").as_int();
		rect.h = node.attribute("h").as_int();
		animation.PushBack({ rect });
	}

} 