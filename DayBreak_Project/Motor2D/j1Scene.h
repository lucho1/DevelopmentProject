#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;


enum LEVELS {

	LEVEL1 = 0,
	LEVEL2,
	MAIN_MENU,
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

	//Used to iterate between Levels
	void IterateLevel();

	//Used to change the actual level
	void ChangeLevel();

	bool Level1 = false;
	bool Level2 = true;
	bool Main_Menu = true;
	
	int LevelIterator = 0;

	LEVELS currentLevel = NO_LEVEL;


private:
	



};

#endif // __j1SCENE_H__