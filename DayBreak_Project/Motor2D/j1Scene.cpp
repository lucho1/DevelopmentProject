#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Collisions.h"
#include "j1PathFinding.h"
#include "j1Fade.h"
#include "j1EntityManager.h"
#include "j1Enemy.h"
#include "j1Player.h"
#include "j1Objects.h"

#include "Brofiler/Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
	Main_Menu = false;
	Level1 = true;
	Level2 = false;
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{

	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
  
	pugi::xml_parse_result load_scenedoc_res = SceneDocument.load_file("config.xml");
	music_node = SceneDocument.child("config").child("music");

	if (load_scenedoc_res == NULL)
		LOG("The xml file containing the music fails. Pugi error: %s", load_scenedoc_res.description());
	
	App->map->Load("Main_Menu.tmx", Intro_map);
	App->map->Load("Level1.tmx", Level1_map);
	App->map->Load("Level2.tmx", Level2_map);
	App->map->Load("Level1_WalkabilityMap.tmx", Level1_pathfinding_map);
	App->map->Load("Level2_WalkabilityMap.tmx", Level2_pathfinding_map);


	if (Main_Menu == true) {

		current_map = Intro_map;
		currentLevel = MAIN_MENU;

	//	App->audio->PlayMusic(music_node.attribute("intro2").as_string());

		pathfinding = false;
	}

	if (Level1 == true) {

		App->collisions->AssignMapColliders("Level1.tmx");
		LoadObjects("Level1.tmx");
	
		if (Player == nullptr)
			//Player = Player->CreatePlayer(iPoint(200, 1080));

		current_map = Level1_map;
		currentLevel = LEVEL1;

		pathfinding = true;

		/*if(Player->active)
			Player->Start();*/

		if(!App->collisions->active)
			App->collisions->Start();

	}

	else if (Level2 == true) {

		App->collisions->AssignMapColliders("Level2.tmx");
		LoadObjects("Level2.tmx");

		/*if (Player == nullptr)
			Player = Player->CreatePlayer(iPoint(580, 1400));*/

		current_map = Level2_map;
		//current_pathfinding_map = Level2_pathfinding_map;
		currentLevel = LEVEL2;

		pathfinding = false;

		/*if (Player->active)
			Player->Start();*/

		if (!App->collisions->active)
			App->collisions->Start();

	}

	App->audio->ControlVolume(10);
	App->render->ResetCamera();

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("Scene PreUpdate", Profiler::Color::GreenYellow);
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	BROFILER_CATEGORY("Scene Update", Profiler::Color::PaleVioletRed);

	if (Change_Level) {

		App->fade->Fade(2.0f);

		if (App->fade->current_step == App->fade->fade_from_black) {
			ChangeLevel(currentLevel);
			Change_Level = false;
		}
	}

	else if (changing_same_Level) {

		App->fade->Fade(2.0f);

		if (App->fade->current_step == App->fade->fade_from_black) {

			ChangeLevel(NO_CHANGE);
			changing_same_Level = false;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) 
		App->LoadGame("save_game.xml");
	

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && !changing_same_Level) {
		//App->fade->Fade(2.0f);
		App->map->TriggerActive = false;
		changing_same_Level = true;
		//ChangeLevel(currentLevel);
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && !Change_Level) {

		Change_Level = true;
		App->map->TriggerActive = false;
	}

	if (currentLevel == MAIN_MENU && App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && !Change_Level)
		Change_Level = true;
	
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {

		App->cap = !App->cap;

		if(App->cap)
			App->frame_cap = 30;
		else 
			App->frame_cap = 60;
		
		App->capped_ms = 1000 / App->frame_cap;
	}

	App->map->Draw(current_map);
	if (Player != nullptr) {

		/*if (Player->life <= 0) {
			if(Player->Dead.Finished())
			ChangeLevel(LEVEL2+1);
		}*/
	}
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{

	BROFILER_CATEGORY("Scene PostUpdate", Profiler::Color::YellowGreen);
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{

	LOG("Freeing scene");

	return true;
}

void j1Scene::ChangeLevel(int level_change) {
	
//	App->entity_manager->DesrtroyEnemies();

	/*if(Player!=nullptr)
		Player->entity_collider->to_delete = true;

	App->entity_manager->DestroyEntity(Player);
	RELEASE(Player);*/
	LEVELS aux = currentLevel;


	if (level_change != NO_CHANGE) {
		App->collisions->CleanUp();
		IterateLevel(level_change);
	}

	if (pathfinding) {
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data, current_pathfinding_map))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	if (currentLevel == LEVEL1) {

		//App->audio->PlayMusic(music_node.attribute("level").as_string());
		App->render->camera.x = -7;
		App->render->camera.y = -1242;
		/*Player = Player->CreatePlayer(iPoint(580, 1400));
		Player->LoadPlayer("Character_tileset.png");*/


		//LOAD SCENE ENEMIES
		//Load Enemy Flyers
		pugi::xml_parse_result Enemies_doc_res = EnemiesDocument.load_file("Enemy2_Settings.xml");
		if (Enemies_doc_res == NULL)
			LOG("The xml file containing the player tileset fails. Pugi error: %s", Enemies_doc_res.description());

		pugi::xml_parse_result enemies_pos_result = EnemiesPositions.load_file("Enemy_Positions.xml");
		en_pos = EnemiesPositions.child("config").child("Level1").child("Enemy2");

		//App->entity_manager->LoadSceneEnemeies(en_pos, FLYER, "Enemy2_Tileset.png", EnemiesDocument);

		//Load enemy Walkers
		pugi::xml_parse_result Enemies_doc_res2 = EnemiesDocument.load_file("Enemy1_Settings.xml");
		if (Enemies_doc_res2 == NULL)
			LOG("The xml file containing the player tileset fails. Pugi error: %s", Enemies_doc_res2.description());

		en_pos = EnemiesPositions.child("config").child("Level1").child("Enemy1");

		//App->entity_manager->LoadSceneEnemeies(en_pos, WALKER, "Enemy1_Tileset.png", EnemiesDocument);

	
	}
	if (currentLevel == LEVEL2) {

		//App->audio->PlayMusic(music_node.attribute("level").as_string());
		App->render->camera.x = -7;
		App->render->camera.y = -903;
		/*Player = Player->CreatePlayer(iPoint(200, 1116));
		Player->LoadPlayer("Character_tileset.png");*/

		//LOAD SCENE ENEMIES
		//Load Enemy Flyers
		pugi::xml_parse_result Enemies_doc_res = EnemiesDocument.load_file("Enemy2_Settings.xml");
		if (Enemies_doc_res == NULL)
			LOG("The xml file containing the player tileset fails. Pugi error: %s", Enemies_doc_res.description());

		pugi::xml_parse_result enemies_pos_result = EnemiesPositions.load_file("Enemy_Positions.xml");
		en_pos = EnemiesPositions.child("config").child("Level2").child("Enemy2");

		//App->entity_manager->LoadSceneEnemeies(en_pos, FLYER, "Enemy2_Tileset.png", EnemiesDocument);

		//Load enemy Walkers
		pugi::xml_parse_result Enemies_doc_res2 = EnemiesDocument.load_file("Enemy1_Settings.xml");
		if (Enemies_doc_res2 == NULL)
			LOG("The xml file containing the player tileset fails. Pugi error: %s", Enemies_doc_res2.description());

		en_pos = EnemiesPositions.child("config").child("Level2").child("Enemy1");

		//App->entity_manager->LoadSceneEnemeies(en_pos, WALKER, "Enemy1_Tileset.png", EnemiesDocument);

	}
	if (currentLevel == MAIN_MENU) {
		App->collisions->CleanUp();
	}
	if (currentLevel!=aux) {
		LoadObjects(current_map.Filename.GetString());
		App->collisions->AssignMapColliders(current_map.Filename.GetString());
	}
}

void j1Scene::IterateLevel(int level_change) {

	level_change++;
	LevelIterator = level_change;
	

	if (LevelIterator > 2)
		LevelIterator = 0;
	

	if (LevelIterator == 0) {

		Main_Menu = true;
		Level2 = false;
		Level1 = false;
		currentLevel = MAIN_MENU;
		current_map = Intro_map;
		pathfinding = false;
		
	}

	else if (LevelIterator == 1) {

			Main_Menu = false;
			Level2 = false;
			Level1 = true;
			currentLevel = LEVEL1;
			current_map = Level1_map;
			current_pathfinding_map = Level1_pathfinding_map;
			pathfinding = true;
	}
	
	else if(LevelIterator == 2) {

			Main_Menu = false;
			Level1 = false;
			Level2 = true;
			currentLevel = LEVEL2;
			current_map = Level2_map;
			current_pathfinding_map = Level2_pathfinding_map;
			pathfinding = true;
	}
}


bool j1Scene::LoadObjects(const char*file_name) {

	p2SString tmp("%s%s", App->map->folder.GetString(), file_name);

	pugi::xml_parse_result result = App->map->map_file.load_file(tmp.GetString());

	if (result == NULL) {

		LOG("Could not load xml file %s. pugi error: %s", file_name, result.description());
		return true;
	}

	pugi::xml_node collider;
	pugi::xml_node type;
	const char* collidertype;

	for (type = App->map->map_file.child("map").child("objectgroup"); type && result; type = type.next_sibling("objectgroup")) {

		collidertype = type.attribute("name").as_string();

		for (collider = type.child("object"); collider&&result; collider = collider.next_sibling("object")) {

			if (strcmp(collidertype, "Push_Triggers") == 0) {

				int x = collider.attribute("x").as_int();
				int y = collider.attribute("y").as_int();

				App->collisions->AddCollider({ x, y, collider.attribute("width").as_int(),collider.attribute("height").as_int() }, COLLIDER_TYPE::TRIGGER_PUSH);
			//	j1Objects* object = nullptr;
				//object = object->CreateObject(iPoint(x, y), OBJECT_TYPE::PUSHON);


			}

			if (strcmp(collidertype, "PushOff_Triggers") == 0) {

				int x = collider.attribute("x").as_int();
				int y = collider.attribute("y").as_int();

				App->collisions->AddCollider({ x, y, collider.attribute("width").as_int(),collider.attribute("height").as_int() }, COLLIDER_TYPE::TRIGGER_PUSHOFF);
				/*j1Objects* object = nullptr;
				object = object->CreateObject(iPoint(x, y), OBJECT_TYPE::PUSHOFF);*/

			}

			if (strcmp(collidertype, "Win_Trigger") == 0) {
				
				int x = collider.attribute("x").as_int();
				int y = collider.attribute("y").as_int();

				App->collisions->AddCollider({ x, y, collider.attribute("width").as_int(),collider.attribute("height").as_int() }, COLLIDER_TYPE::TRIGGER_WIN);
				/*j1Objects* object = nullptr;
				object = object->CreateObject(iPoint(x, y), OBJECT_TYPE::WIN);*/
			}

			if (strcmp(collidertype, "Bloc_Colliders") == 0) {

				int x = collider.attribute("x").as_int();
				int y = collider.attribute("y").as_int();

				App->collisions->AddCollider({ x, y, collider.attribute("width").as_int(),collider.attribute("height").as_int() }, COLLIDER_TYPE::COLLIDER_BLINKING);
				/*j1Objects* object = nullptr;
				object = object->CreateObject(iPoint(x, y), OBJECT_TYPE::BLINK_BLOCK);*/

			}
		}
	}


	return true;
}