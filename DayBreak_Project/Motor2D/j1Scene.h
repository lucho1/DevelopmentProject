#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;


enum LEVELS {

	MAIN_MENU = 0,
	LEVEL1,
	LEVEL2,
	NO_LEVEL

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

	//Load & Save
	bool Load(pugi::xml_node& data);

	//Save Game State
	bool Save(pugi::xml_node& data) const;

	//Used to iterate between Levels
	void IterateLevel(int level);

	//Used to change the actual level
	void ChangeLevel(int level);

	bool Level1 = false;
	bool Level2 = true;
	bool Main_Menu = true;
	
	int LevelIterator = 2;

	LEVELS currentLevel = NO_LEVEL;


private:
	
	SDL_Texture* debug_tex;

	pugi::xml_document	SceneDocument;
	pugi::xml_node		music_node;

};

#endif // __j1SCENE_H__