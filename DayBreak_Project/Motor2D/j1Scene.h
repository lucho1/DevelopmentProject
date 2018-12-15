#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Enemy.h"


struct SDL_Texture;
struct MapData;
class j1Player;
class UI_Element;

enum LEVELS {

	MAIN_MENU = 0,
	LEVEL1,
	LEVEL2,
	NO_LEVEL,
	NO_CHANGE

};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Used to iterate between Levels
	void IterateLevel(int level_change);

	//Used to change the actual level
	void ChangeLevel(int level_change);

	//Load Map Objects
	bool LoadObjects(const char*file_name);

public:

	bool Level1 = false;
	bool Level2 = true;
	bool Main_Menu = true;
	bool changing_same_Level = false;
	bool Change_Level = false;
	
	int LevelIterator = 0;

	LEVELS currentLevel = NO_LEVEL;

	MapData current_map;
	MapData current_pathfinding_map;

	//Player
	j1Player *Player = nullptr;

	//UI_Element* gameName=nullptr;

	//UI_Element* quitButton = nullptr;
	//UI_Element* labelQuitButton = nullptr;

	//UI_Element* playButton = nullptr;
	//UI_Element* labelPlayButton = nullptr;

	//UI_Element* continueButton = nullptr;
	//UI_Element* labelContinueButton = nullptr;

	//UI_Element* webPageButton = nullptr;
	//UI_Element* iconWebPageButton = nullptr;

	//UI_Element* settingsButton = nullptr;
	//UI_Element* labelSettingsButton = nullptr;

	//UI_Element* settingsPanel = nullptr;
	//UI_Element* labelSettingsPanel = nullptr;

	//UI_Element* closeWinButon = nullptr;
	//UI_Element* iconCloseWinButton = nullptr;

	//UI_Element* creditButton = nullptr;
	//UI_Element* labelcreditButton = nullptr;

	//UI_Element* slideMusic = nullptr;
	//UI_Element* slideSFX = nullptr;

	//UI_Element* thumbMusic = nullptr;
	//UI_Element* thumbSFX = nullptr;


	//UI_Element* labelMusic = nullptr;
	//UI_Element* labelSFX = nullptr;

	//p2List<UI_Element*> UI_Elements_List;

	//iPoint lastMousePos = { 0,0 };
	//iPoint newMousePos = { 0,0 };

private:
	SDL_Color None = { 255,255,255,255 };
	MapData Intro_map;
	MapData Level1_map;
	MapData Level2_map;

	MapData Level1_pathfinding_map;
	MapData Level2_pathfinding_map;

	bool pathfinding = false;

	pugi::xml_document	EnemiesDocument;
	pugi::xml_document	SceneDocument;
	pugi::xml_node		music_node;

	pugi::xml_document EnemiesPositions;
	pugi::xml_node en_pos;
	

};

#endif // __j1SCENE_H__