#include "j1Player.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "Animation.h"
#include "j1Textures.h"
#include "j1Pathfinding.h"
#include "j1Timer.h"
#include "j1Particles.h"
#include "j1Fade.h"
#include "j1Player.h"
#include "j1EntityManager.h"

#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

#include "Brofiler/Brofiler.h"

j1Player::j1Player(iPoint pos) : j1Entity(ENTITY_TYPE::PLAYER_ENT), player_position(pos) {
  
	name.create("player");
	pugi::xml_parse_result result = PlayerDocument.load_file("PlayerSettings.xml");

	if (result == NULL)
		LOG("The xml file containing the player tileset fails. Pugi error: %s", result.description());

	PlayerSettings = PlayerDocument.child("config");
	
	shoot_pl = App->audio->LoadFx("audio/fx/Shoot.wav");
	button_wav = App->audio->LoadFx("audio/fx/Button.wav");
	ray_touched_wav = App->audio->LoadFx("audio/fx/Ray_touched.wav");
	lose_fx = App->audio->LoadFx("audio/fx/lose.wav");
	win_fx = App->audio->LoadFx("audio/fx/win.wav");
	Shout_Die = App->audio->LoadFx("audio/fx/WilScream.wav");
	life = 70;
	
}

j1Player::~j1Player() {}

void j1Player::CleanUp() { 

	LOG("Cleaning Up Player");
	/*if (player_collider != nullptr)
		player_collider->to_delete = true;*/

	if (current_animation != nullptr)
		current_animation = nullptr;
	
	App->tex->UnLoad(Player_texture);
	active = false;

}


void j1Player::Start() {

	LoadPlayer("Character_tileset.png");

}


void j1Player::FixUpdate(float dt) {

	BROFILER_CATEGORY("Player FixUpdate", Profiler::Color::IndianRed);
	HandleInput(dt);

	if (life > 0) {
		
		//Gun Handle
		if ((!jump || !fall) && state != pl_RUN) {

			Gun_Run.Reset();
			current_animation = &Idle;
			Gun_current_animation = &Gun_Idle;

			//GUN ADJUST
			Adjusting_Gun_position.x = 0;
			Adjusting_Gun_position.y = player_rect.h / 3;

		}

		//SHOOT
		if (Shooting) {

			Gun_current_animation = &Gun_Shot;

			if (Gun_Shot.Finished()) {
				Shooting = false;
				Gun_Shot.Reset();
			}

			Adjusting_Gun_position.x = 2;
			Adjusting_Gun_position.y = player_rect.h / 3.5f;
		}


		if (desaccelerating == true) {

			if (player_velocity.x > 0) {

				player_velocity.x -= acceleration.x*2;

				if (direction_x == pl_RIGHT)
					player_position.x += player_velocity.x;

				else if (direction_x == pl_LEFT)
					player_position.x -= player_velocity.x;
			}
			else
				desaccelerating = false;

		}

		//JUMP
		if (jump || doublejump) {

			if (jump)
				current_animation = &Jump;

			if (player_velocity.y >= 0 && !jump_falling) {

				if (direction_x == pl_RIGHT) {

					if (angle < 0)
						angle *= (-1);

					if (angle < 20)
						angle += 1;

				}
				else if (direction_x == pl_LEFT) {

					if (angle > 0)
						angle *= (-1);

					if (angle > -20)
						angle -= 1;

				}

				fall = false;
				player_position.y -= player_velocity.y;
				player_velocity.y -= acceleration.y;//(acceleration.x * 2); //Original: 0.43f
			}

			else if (player_velocity.y < 0) {

				jump_falling = true;
				fall = true;

			}
		}

		if (!jump)
			doublejump = true;


		//FALL
		if (fall) {

			Jump.Reset();

			current_animation = &Fall;
			if (player_velocity.y < MaxVelocity.y) {

				if (direction_x == pl_RIGHT) {

					if (angle < 0)
						angle *= (-1);

					if (angle > 0)
						angle -= 1;

				}

				else if (direction_x == pl_LEFT) {

					if (angle > 0)
						angle *= (-1);

					if (angle < 0)
						angle += 1;

				}

				player_velocity.y += acceleration.y;
				}

			player_position.y += player_velocity.y;
		}


		//GOD MODE
		if (God) {

			LOG("GOD MODE ACTIVE");
			jump = true;
			fall = false;
			//life += 20;

		}
		else {
			fall = true;
		}
	}

	else if (life <= 0) {

		if (fall == false)
			fall = true;

		else {

		player_velocity.y += acceleration.y;
		player_position.y += player_velocity.y;
		}

		App->audio->PlayFx(Shout_Die);
		App->audio->PlayFx(lose_fx);
		current_animation = &Dead;
		Gun_current_animation = &Gun_None;

		if (Dead.Finished())
			App->entity_manager->DestroyEntity(this);


	}
	LOG("%d", life);

}


void j1Player::Update(float dt) {

	BROFILER_CATEGORY("Player Update", Profiler::Color::DarkOrange);
	Draw(dt);

}

void j1Player::Draw(float dt) {


	if (direction_x == pl_RIGHT) {

		entity_collider->SetPos(player_position.x, player_position.y);
		App->render->Blit(Player_texture, player_position.x, player_position.y, &(current_animation->GetCurrentFrame(dt)), 1, 0, 0, 0, SDL_FLIP_NONE, 0.4f);
		App->render->Blit(Player_texture, player_position.x + Adjusting_Gun_position.x, player_position.y + Adjusting_Gun_position.y, &(Gun_current_animation->GetCurrentFrame(dt)), 1, angle, 0, 0, SDL_FLIP_NONE, 0.4f);
	}

	if (direction_x == pl_LEFT) {

		entity_collider->SetPos(player_position.x, player_position.y);
		App->render->Blit(Player_texture, player_position.x, player_position.y, &(current_animation->GetCurrentFrame(dt)), 1, 0, 0, 0, SDL_FLIP_HORIZONTAL, 0.4f);
		App->render->Blit(Player_texture, player_position.x + Adjusting_Gun_position.x - 30, player_position.y + Adjusting_Gun_position.y, &(Gun_current_animation->GetCurrentFrame(dt)), 1, angle, 60, 0, SDL_FLIP_HORIZONTAL, 0.4f);
	}

}

void j1Player::HandleInput(float dt) {
		
	//player_velocity *= dt;
	//acceleration *= dt;
	//initial_vel *= dt;
	

	//God mode
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		God = !God;

	//X axis Movement
	//RIGHT
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

		state = State::pl_RUN;

		desaccelerating = false;
		//player_velocity.x = player_velocity.x; //??

		direction_x = pl_RIGHT;

		player_position.x += player_velocity.x ;

		if (player_velocity.x <= MaxVelocity.x)
			player_velocity.x +=acceleration.x;// acceleration.x; //?? WATCHOUT HERE 0.2f was the original


		Gun_current_animation = &Gun_Run;
		current_animation = &Run;

		//GUN ADJUST
		Adjusting_Gun_position.x = 0;
		Adjusting_Gun_position.y = player_rect.h / 3.5f;

		if (!jump)
			angle = 0;
	}

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP && !jump && !desaccelerating)
		desaccelerating = true;

	else
		state = State::pl_IDLE;

	//LEFT
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

		state = State::pl_RUN;

		desaccelerating = false;
		//player_velocity.x = player_velocity.x; //??

		direction_x = pl_LEFT;

		player_position.x -= player_velocity.x + acceleration.x;

		if (player_velocity.x <= MaxVelocity.x)
			player_velocity.x += acceleration.x;// acceleration.x; //?? SAME THAN UPWARDS, original: 0.2f

		Gun_current_animation = &Gun_Run;
		current_animation = &Run;

		//GUN ADJUST
		Adjusting_Gun_position.x = -5;
		Adjusting_Gun_position.y = player_rect.h / 3.5f;

		if (!jump)
			angle = 0;

	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP && !jump && !desaccelerating)
		desaccelerating = true;

	if (!God) {

		//Y axis Movement (UP)
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && !jump && !God && !fall) {

			jump = true;
			jump_falling = false;

			auxY = player_position.y;
			player_velocity.y = initial_vel.y;
		}

		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && jump&& doublejump && !God) {

			doublejump = false;
			jump_falling = false;

			auxY = player_position.y;
			player_velocity.y = initial_vel.y;
		}
	}
	else {

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {

			player_velocity.y = initial_vel.y;
			player_position.y -= player_velocity.y;
		}

		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {

			player_velocity.y = initial_vel.y;
			player_position.y += player_velocity.y;
		}

	}

	//SHOOTING
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !Shooting) {

		angle = 0;
		App->audio->PlayFx(shoot_pl);

		if (direction_x == pl_RIGHT) {

			App->particles->AddParticle(App->particles->Player_Shoot, player_position.x + Adjusting_Gun_position.x + 20, player_position.y + Adjusting_Gun_position.y - 14, COLLIDER_PLAYER_BULLET, fPoint(1000, 0), 0.8f); //Original speed 16, 0
			App->particles->AddParticle(App->particles->Player_Shoot_Beam, player_position.x + Adjusting_Gun_position.x + 20, player_position.y - 18, COLLIDER_NONE, fPoint( player_velocity.x+200, 0), 1.1f);
		}

		else if (direction_x == pl_LEFT) {

			App->particles->AddParticle(App->particles->Player_Shoot, player_position.x + Adjusting_Gun_position.x - 80, player_position.y + Adjusting_Gun_position.y - 14, COLLIDER_PLAYER_BULLET, fPoint(-1000, 0), 0.8f, SDL_FLIP_HORIZONTAL); //Original speed 16, 0
			App->particles->AddParticle(App->particles->Player_Shoot_Beam, player_position.x + Adjusting_Gun_position.x - 80, player_position.y - 18, COLLIDER_NONE, fPoint( player_velocity.x-200, 0), 1.1f);
		}

		Shooting = true;
	}
}

bool j1Player::Load(pugi::xml_node& data)
{
	player_position.x = data.child("player").attribute("x").as_int();
	player_position.y = data.child("player").attribute("y").as_int();
	life = data.child("player").attribute("life").as_int();
	App->entity_manager->score = data.child("player").attribute("score").as_int();
	App->entity_manager->coins = data.child("player").attribute("coins").as_int();

	return true;
}

// Save Game State
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node pl = data.append_child("player");

	pl.append_attribute("x") = player_position.x;
	pl.append_attribute("y") = player_position.y;
	pl.append_attribute("life") = life;
	pl.append_attribute("score") = App->entity_manager->score;
	pl.append_attribute("coins") = App->entity_manager->coins;

	return true;
}

void j1Player::OnCollision(Collider *c1, Collider *c2) {

	if (c2->type == COLLIDER_ENEMY_BULLET) {

		if (Last_collided == c2) 
			return;
		
		if (App->render->DoCameraShake == false) {
			App->render->DoCameraShake = true;
			App->render->power = 3.0f;
			App->render->Time_Doing_Shake = 0.3f;
			PERF_START(App->render->CameraShake_Time);
		}

		life -= 5;
		Last_collided = c2;
		App->particles->AddParticle(App->particles->Blood, player_position.x-35, player_position.y-30, COLLIDER_NONE, fPoint(0, 0), 1.5f, SDL_FLIP_NONE);

	}
	//Checking collision with walls
	if (c2->type == COLLIDER_STATIC || (c2->type == COLLIDER_BLINKING && App->map->TriggerActive == true)) {
		
		//Calculating an error margin of collision to avoid problems with colliders corners
		int error_margin = 0;

		if (direction_x == pl_RIGHT)
			error_margin = (c1->rect.x + c1->rect.w) - c2->rect.x;
		else if (direction_x == pl_LEFT)
			error_margin = (c2->rect.x + c2->rect.w) - c1->rect.x;

		//If the player falls less than a pixel over a collider, it falls (and it looks ok)
		if (error_margin > 1) {

			//Checking Y Axis Collisions
			if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y >= c2->rect.y + c2->rect.h - player_velocity.y - acceleration.y) { //Colliding down (jumping)
			
				fall = false;
				player_velocity.y = 0.0f;
				
				player_position.y = c1->rect.y + c2->rect.h - (c1->rect.y - c2->rect.y) + 3;
			}
			else if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h <= c2->rect.y + player_velocity.y + acceleration.y) { //Colliding Up (falling)
				
				fall = false;
				jump = false;
				player_velocity.y = 0.0f;
				
				player_position.y = c1->rect.y - ((c1->rect.y + c1->rect.h) - c2->rect.y);
			}
		}

		//Checking X Axis Collisions
		if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + player_velocity.x + acceleration.x) { //Colliding Left (going right)
			
			desaccelerating = false;
			player_velocity.x = 0.0f;
			player_position.x -= (c1->rect.x + c1->rect.w) - c2->rect.x + 1;

		}
		else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x >= c2->rect.x + c2->rect.w - player_velocity.x-acceleration.x) { //Colliding Right (going left)
			
			desaccelerating = false;
			player_velocity.x = 0.0f;
			player_position.x += (c2->rect.x + c2->rect.w) - c1->rect.x + 1;

		}
	}

	if (!God) {

		//Checking if falling
		if (c1->type == COLLIDER_FALL || c2->type == COLLIDER_FALL) //This mechanic is cool so we force the player to save before each decision
			life = 0;
	}

	//Check if touched button or end level door
	if (c1->type == TRIGGER_PUSH || c2->type == TRIGGER_PUSH) { //Trigger push = button
		
		App->map->TriggerActive = true;
		App->audio->PlayFx(button_wav);
	}

	if (c1->type == TRIGGER_PUSHOFF || c2->type == TRIGGER_PUSHOFF) { //Ray that makes blinking platforms unactive
	
		App->map->TriggerActive = false;
		App->audio->PlayFx(ray_touched_wav);
	}

	if (c1->type == TRIGGER_WIN || c2->type == TRIGGER_WIN) {

		App->scene->Change_Level = true;
		App->map->TriggerActive = false;

		App->audio->PlayFx(win_fx);

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


j1Player* j1Player::CreatePlayer(iPoint pos/*, const char* path, pugi::xml_document &PlayerDocument*/) {

	j1Player* Player = nullptr;
	Player = new j1Player(pos);

	App->entity_manager->entities_list.add(Player);
	return Player;
}

void j1Player::LoadPlayer(const char *file_name) {
	
	//Loading Settings
	Animation_node = PlayerDocument.child("config").child("AnimationCoords").child("Idle");
	LoadPushbacks(Animation_node, Idle);
	Animation_node = PlayerDocument.child("config").child("AnimationCoords").child("Run");
	LoadPushbacks(Animation_node, Run);
	Animation_node = PlayerDocument.child("config").child("AnimationCoords").child("Jump");
	LoadPushbacks(Animation_node, Jump);
	Animation_node = PlayerDocument.child("config").child("AnimationCoords").child("Fall");
	LoadPushbacks(Animation_node, Fall);
	Animation_node = PlayerDocument.child("config").child("AnimationCoords").child("Dead");
	LoadPushbacks(Animation_node, Dead);
	Animation_node = PlayerDocument.child("config").child("AnimationCoords").child("Gun").child("None");
	LoadPushbacks(Animation_node, Gun_None);
	Animation_node = PlayerDocument.child("config").child("AnimationCoords").child("Gun").child("Idle");
	LoadPushbacks(Animation_node, Gun_Idle);
	Animation_node = PlayerDocument.child("config").child("AnimationCoords").child("Gun").child("Run");
	LoadPushbacks(Animation_node, Gun_Run);
	Animation_node = PlayerDocument.child("config").child("AnimationCoords").child("Gun").child("Shoot");
	LoadPushbacks(Animation_node, Gun_Shot);


	PlayerSettings = PlayerDocument.child("config");

	life = PlayerSettings.child("PlayerSettings").child("Life").attribute("life").as_int();

	p2SString tmp("maps\\%s", file_name);
	Player_texture = App->tex->Load(tmp.GetString());

	//Starting Position acceleration & Velocity
	acceleration.x = PlayerSettings.child("PlayerSettings").child("Acceleration").attribute("a.x").as_float();
	acceleration.y = PlayerSettings.child("PlayerSettings").child("Acceleration").attribute("a.y").as_float();

	MaxVelocity.x = initial_vel.x = PlayerSettings.child("PlayerSettings").child("MaxVelocity").attribute("velocity.x").as_float();
	MaxVelocity.y= PlayerSettings.child("PlayerSettings").child("MaxVelocity").attribute("velocity.y").as_float();
	initial_vel.x = PlayerSettings.child("PlayerSettings").child("Velocity").attribute("velocity.x").as_float();
	initial_vel.y = PlayerSettings.child("PlayerSettings").child("Velocity").attribute("velocity.y").as_float();
	direction_x = pl_RIGHT;

	if (App->scene->currentLevel == LEVEL1) {

		player_position.x = PlayerSettings.child("PlayerSettings").child("StartingPose").child("Level1").attribute("position.x").as_int();
		player_position.y = PlayerSettings.child("PlayerSettings").child("StartingPose").child("Level1").attribute("position.y").as_int();

		Gun_position = player_position;
	}

	else if (App->scene->currentLevel == LEVEL2) {

		player_position.x = PlayerSettings.child("PlayerSettings").child("StartingPose").child("Level2").attribute("position.x").as_int();
		player_position.y = PlayerSettings.child("PlayerSettings").child("StartingPose").child("Level2").attribute("position.y").as_int();
	}

	else if (App->scene->currentLevel == MAIN_MENU)
		player_position.x = -200;

	fall = true;

	//Player Rect
	player_rect.x = player_position.x;
	player_rect.y = player_position.y;
	player_rect.w = PlayerSettings.child("image").attribute("w").as_int();
	player_rect.h = PlayerSettings.child("image").attribute("h").as_int();

	entity_collider = App->collisions->AddColliderEntity({ player_rect.x + 50, player_rect.y, (player_rect.w - 65), (player_rect.h) - 28 }, COLLIDER_PLAYER, this);

	state = State::pl_IDLE;

}