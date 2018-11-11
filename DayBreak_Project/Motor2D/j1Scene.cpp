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

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
	Main_Menu = true;
	Level1 = false;
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
	if (Main_Menu == true) {
		App->map->Load("Main_Menu.tmx");
		currentLevel = MAIN_MENU;
		App->render->ResetCamera();
	}
	if (Level1 == true) {

		App->map->Load("Level1.tmx");
		currentLevel = LEVEL1;
		App->render->ResetCamera();
		App->player->Start();
		App->collisions->Start();
	}
	else if (Level2 == true) {
		App->map->Load("Level2.tmx");
		currentLevel = LEVEL2;
		App->render->ResetCamera();
		App->player->Start();
		App->collisions->Start();
	}

	if (Level1==true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
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

	debug_tex = App->tex->Load("maps/path2.png");
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		//App->LoadGame("save_level1.xml");
		App->map->TriggerActive = false;
		ChangeLevel();
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		App->collisions->CleanUp();
		App->player->CleanUp();
		App->map->CleanUp();
		Start();
	}

	if (currentLevel == MAIN_MENU && App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		ChangeLevel();
	
	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);

	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("%s v0.1 Info: Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d", App->GetTitle(),
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());

	// Debug pathfinding ------------------------------
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

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

void j1Scene::ChangeLevel() {
	IterateLevel();
	App->collisions->CleanUp();
	App->player->CleanUp();
	App->map->CleanUp();
	Start();
}

void j1Scene::IterateLevel() {

	LevelIterator++;
	if (LevelIterator > 2) {
		LevelIterator = 0;
	}

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
	}
	
	else if(LevelIterator == 2) {
			Main_Menu = false;
			Level1 = false;
			Level2 = true;
			currentLevel = LEVEL2;
	}



}