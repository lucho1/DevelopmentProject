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

	if (result == NULL)
		LOG("The xml file containing the player tileset fails. Pugi error: %s", result.description());
	
	//Loading Settings
	Animation_node = PlayerDocument.child("config").child("AnimationCoords").child("Idle");
	LoadPushbacks(Animation_node, Idle);
	Animation_node = PlayerDocument.child("config").child("AnimationCoords").child("Run");
	LoadPushbacks(Animation_node, Run);
	Animation_node = PlayerDocument.child("config").child("AnimationCoords").child("Jump");
	LoadPushbacks(Animation_node, Jump);
	PlayerSettings = PlayerDocument.child("config");

}

j1Player::~j1Player() {}

bool j1Player::CleanUp() { 

	LOG("Cleaning Up Player");
	if (player_collider != nullptr)
		player_collider->to_delete = true;

	if (current_animation != nullptr)
		current_animation = nullptr;
	App->tex->UnLoad(Player_texture);

	return true;
}

bool j1Player::Awake() {

	LOG("Player Module Loading");
	bool ret = true;
	return ret;
}

bool j1Player::Start() {

	p2SString tmp = ("maps\\Character_tileset.png");
	Player_texture = App->tex->Load(tmp.GetString());

	//Starting Position acceleration & Velocity
	acceleration.x = PlayerSettings.child("PlayerSettings").child("Acceleration").attribute("a.x").as_float();
	acceleration.y = PlayerSettings.child("PlayerSettings").child("Acceleration").attribute("a.y").as_float();

	initial_vel.x = PlayerSettings.child("PlayerSettings").child("Velocity").attribute("velocity.x").as_int();
	initial_vel.y = PlayerSettings.child("PlayerSettings").child("Velocity").attribute("velocity.y").as_int();
	direction_x = RIGHT;

	if (App->scene->currentLevel == LEVEL1) {
		position.x = PlayerSettings.child("PlayerSettings").child("StartingPose").child("Level1").attribute("position.x").as_int();
		position.y = PlayerSettings.child("PlayerSettings").child("StartingPose").child("Level1").attribute("position.y").as_int();
	}
	else if (App->scene->currentLevel==LEVEL2) {
		position.x = PlayerSettings.child("PlayerSettings").child("StartingPose").child("Level2").attribute("position.x").as_int();
		position.y = PlayerSettings.child("PlayerSettings").child("StartingPose").child("Level2").attribute("position.y").as_int();
	}
	else if (App->scene->currentLevel == MAIN_MENU) 
		position.x = -200;

	fall = true;
	
	//Player Rect
	player_rect.x = position.x;
	player_rect.y = position.y;
	player_rect.w = PlayerSettings.child("image").attribute("w").as_int();
	player_rect.h = PlayerSettings.child("image").attribute("h").as_int();

	player_collider = App->collisions->AddCollider({ player_rect.x + 50, player_rect.y, (player_rect.w - 65), (player_rect.h)-28 }, COLLIDER_PLAYER, this);

	//Rect Pathfinding test
	Enemy = { position.x + player_rect.w + 10, position.y,  (player_rect.w - 65), (player_rect.h) - 28 };
	
	//Once player is created, saving game to have from beginning a save file to load whenever without giving an error and to load if dead
	//App->SaveGame("save_game.xml");

	return true;
}

bool j1Player::PreUpdate() {

	return true;
}

void j1Player::RectPathfindingTest() {

	App->render->DrawQuad(Enemy, 0, 0, 255, 255);

	bool move = true;

	if (move) {

		iPoint initial_pos = iPoint(Enemy.x, Enemy.y + Enemy.h);
		iPoint final_pos = iPoint(position.x, position.y + player_rect.h);



		move = false;
	}

}



bool j1Player::Update(float dt) {

	current_animation = &Idle;

	//X axis Movement
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

		velocity.x = initial_vel.x;
		direction_x = RIGHT;
		position.x += velocity.x;
		current_animation = &Run;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

		velocity.x = initial_vel.x;
		direction_x = LEFT;
		position.x -= velocity.x;
		current_animation = &Run;
	}
	//Y axis Movement
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && !jump && !God) {

		jump = true;
		jump_falling = false;
		auxY = position.y;
		velocity.y = initial_vel.y;
	}

	//JUMP
	if (jump) {

		if (velocity.y >= 0 && !jump_falling) {
			
			fall = false;
			position.y -= velocity.y;
			velocity.y -= acceleration.y;
		}

		else if (velocity.y < 0) {
		
			jump_falling = true;
			fall = true;
		}
	}
	
	//FALL
	if (fall) {

		if (velocity.y < initial_vel.y)
			velocity.y += acceleration.y;

		position.y += velocity.y;
	}

	//God mode
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		God = !God;

	if (God) {

		LOG("GOD MODE ACTIVE");
		jump = true;
		fall = false;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {

			velocity.y = initial_vel.y;
			position.y -= velocity.y;
		}

		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {

			velocity.y = initial_vel.y;
			position.y += velocity.y;
		}

	}
	else
		fall = true;

	//BLIT PLAYER
	if (direction_x == RIGHT) {

		player_collider->SetPos(position.x, position.y);
		App->render->Blit(Player_texture, position.x , position.y, &(current_animation->GetCurrentFrame()), 1, 0, 0, 0, SDL_FLIP_NONE, 0.4);
	}
	if (direction_x == LEFT) {

		player_collider->SetPos(position.x, position.y);
		App->render->Blit(Player_texture, position.x, position.y, &(current_animation->GetCurrentFrame()), 1, 0, 0, 0, SDL_FLIP_HORIZONTAL, 0.4);
	}

	//Testing pathfinding
	RectPathfindingTest();

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
	if (c2->type == COLLIDER_STATIC || (c2->type == COLLIDER_BLINKING && App->map->TriggerActive == true)) {

		//Calculating an error margin of collision to avoid problems with colliders corners
		int error_margin = 0;

		if (direction_x == RIGHT)
			error_margin = (c1->rect.x + c1->rect.w) - c2->rect.x;
		else if (direction_x == LEFT)
			error_margin = (c2->rect.x + c2->rect.w) - c1->rect.x;

		//If the player falls less than a pixel over a collider, it falls (and it looks ok)
		if (error_margin > 1) {

			//Checking Y Axis Collisions
			if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y >= c2->rect.y + c2->rect.h - velocity.y) { //Colliding down (jumping)
				
				velocity.y = 0;
				position.y = c1->rect.y + c2->rect.h - (c1->rect.y - c2->rect.y) + 3;
			}
			else if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h <= c2->rect.y + velocity.y) { //Colliding Up (falling)
				
				jump = false;
				velocity.y = 0;
				position.y = c1->rect.y - ((c1->rect.y + c1->rect.h) - c2->rect.y);
			}
		}

		//Checking X Axis Collisions
		if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + velocity.x) { //Colliding Left (going right)
			
			velocity.x = 0;
			position.x -= (c1->rect.x + c1->rect.w) - c2->rect.x + 4;

		}
		else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x >= c2->rect.x + c2->rect.w - velocity.x) { //Colliding Right (going left)
			
			velocity.x = 0;
			position.x += (c2->rect.x + c2->rect.w) - c1->rect.x + 4;

		}
	}

	if (!God) {

		//Checking if falling
		if (c1->type == COLLIDER_FALL || c2->type == COLLIDER_FALL) //This mechanic is cool so we force the player to save before each decision
			App->LoadGame("save_game.xml");
	}

	//Check if touched button or end level door
	if (c1->type == TRIGGER_PUSH || c2->type == TRIGGER_PUSH) //Trigger push = button
		App->map->TriggerActive = true;

	if (c1->type == TRIGGER_PUSHOFF || c2->type == TRIGGER_PUSHOFF) //Ray that makes blinking platforms unactive
		App->map->TriggerActive = false;

	if (c1->type == TRIGGER_WIN || c2->type == TRIGGER_WIN) {

		int level_switch = App->scene->currentLevel + 1;
		App->scene->ChangeLevel(level_switch);
	}
}

void j1Player::LoadPushbacks(pugi::xml_node node, Animation &animation) {

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
