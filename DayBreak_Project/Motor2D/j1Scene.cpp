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
	
	App->map->Load("Level1.tmx", Level1_map);
	App->map->Load("Level2.tmx", Level2_map);

	if (Main_Menu == true) {

		App->map->Load("Main_Menu.tmx", Intro_map);
		current_map = Intro_map;
		currentLevel = MAIN_MENU;

		pathfinding = false;
	}

	if (Level1 == true) {
	
		if (Player == nullptr)
			Player = Player->CreatePlayer(iPoint(200, 1080));

		App->map->Load("Level1_WalkabilityMap.tmx", Level1_pathfinding_map);

		current_pathfinding_map = Level1_pathfinding_map;
		current_map = Level1_map;
		currentLevel = LEVEL1;

		pathfinding = true;

		if(Player->active)
			Player->Start();

		if(!App->collisions->active)
			App->collisions->Start();
	}

	else if (Level2 == true) {

		if (Player == nullptr)
			Player = Player->CreatePlayer(iPoint(580, 1400));

		//App->map->Load("Level2_WalkabilityMap.tmx", Level2_pathfinding_map);

		current_map = Level2_map;
		//current_pathfinding_map = Level2_pathfinding_map;
		currentLevel = LEVEL2;

		pathfinding = false;

		if (Player->active)
			Player->Start();

		if (!App->collisions->active)
			App->collisions->Start();
	}
  
	App->render->ResetCamera();

	if (pathfinding) {
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data, current_pathfinding_map))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	pugi::xml_parse_result result = SceneDocument.load_file("config.xml");
	music_node = SceneDocument.child("config").child("music");

	if (result == NULL)
		LOG("The xml file containing the music fails. Pugi error: %s", result.description());

	//Need to play more than one track (or merge them). Just comment/uncomment to change music
	//App->audio->PlayMusic(music_node.attribute("level1_mus").as_string());
	//App->audio->PlayMusic(music_node.attribute("back_music").as_string());

	
	pugi::xml_parse_result result3 = EnemiesDocument.load_file("Enemy2_Settings.xml");

	if (result3 == NULL)
		LOG("The xml file containing the player tileset fails. Pugi error: %s", result.description());


	Enemy1 = Enemy1->CreateEnemy(iPoint(800, 1430), FLYER, "Enemy2_Tileset.png", EnemiesDocument);
	Enemy2 = Enemy2->CreateEnemy(iPoint(300, 1430), FLYER, "Enemy2_Tileset.png", EnemiesDocument);

	pugi::xml_parse_result result4 = EnemiesDocument.load_file("Enemy1_Settings.xml");
	if (result4 == NULL)
		LOG("The xml file containing the player tileset fails. Pugi error: %s", result.description());

	Enemy3 = Enemy3->CreateEnemy(iPoint(600, 1350), WALKER, "Enemy1_Tileset.png", EnemiesDocument);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
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

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN&&!changing_same_Level) {

		//App->fade->Fade(2.0f);
		App->map->TriggerActive = false;
		changing_same_Level = true;
		//ChangeLevel(currentLevel);
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN&&!Change_Level) {

		Change_Level = true;
		App->map->TriggerActive = false;
	}

	if (currentLevel == MAIN_MENU && App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN&&!Change_Level)
		Change_Level = true;
	
	App->map->Draw(current_map);
	App->map->Draw(current_pathfinding_map);

	//int x, y;
	//App->input->GetMousePosition(x, y);

	//iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y, current_pathfinding_map);
	//iPoint mc2 = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y, current_map);

	//iPoint mapatW = App->map->MapToWorld(mc2.x, mc2.y, current_map);
	//iPoint pl_pos = App->map->WorldToMap(App->player->position.x, App->player->position.y, current_map);
	//iPoint en_pos = App->map->WorldToMap(App->player->Enemy.x, App->player->Enemy.y, current_map);

	//
	//iPoint pathtoworld = App->map->MapToWorld(map_coordinates.x, map_coordinates.y, current_pathfinding_map);
	////iPoint pathtomap = App->map->WorldToMap(pathtoworld.x, pathtoworld.y, current_map);
	////iPoint pathftomap = App->map->MapToWorld(pathtomap.x, pathtomap.y, current_map);
	////iPoint pathftomap = App->map->WorldToMap(pathtoworld.x, pathtoworld.y, current_map);

	//p2SString title("PFMapC: %d,%d (%d,%d) MapC: %d,%d(%d,%d) PlayerPos: %d,%d (%d,%d) Enemy: %d,%d (%d,%d)", map_coordinates.x, map_coordinates.y, pathtoworld.x, pathtoworld.y,
	//	mc2.x, mc2.y, mapatW.x, mapatW.y, App->player->position.x, App->player->position.y, pl_pos.x, pl_pos.y, App->player->Enemy.x, App->player->Enemy.y, en_pos.x, en_pos.y);

	///*p2SString title("%s v0.1 Info: Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d", App->GetTitle(),
	//	current_pathfinding_map.width, current_pathfinding_map.height,
	//	current_pathfinding_map.tile_width, current_pathfinding_map.tile_height,
	//	current_pathfinding_map.tilesets.count(),
	//	map_coordinates.x, map_coordinates.y);*/

	//App->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
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
	
	LEVELS aux = currentLevel;
	if (level_change != NO_CHANGE) {
		App->collisions->CleanUp();
		Player->CleanUp();
		IterateLevel(level_change);
	}

	if (currentLevel == LEVEL1) {
		App->LoadGame("Level_1_settings.xml");
	
	}
	if (currentLevel == LEVEL2) {
		App->LoadGame("Level_2_settings.xml");
	}

	if (currentLevel!=aux) {
		Player->Start();
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
		
	}

	else if (LevelIterator == 1) {

			Main_Menu = false;
			Level2 = false;
			Level1 = true;
			currentLevel = LEVEL1;
			current_map = Level1_map;
	}
	
	else if(LevelIterator == 2) {

			Main_Menu = false;
			Level1 = false;
			Level2 = true;
			currentLevel = LEVEL2;
			current_map = Level2_map;
	}
}