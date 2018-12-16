#ifndef __MODULEFADE_H__
#define __MODULEFADE_H__

#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1App.h"

class UI_Element;
struct SDL_Rect;

class j1Fade : public j1Module
{
public:
	j1Fade();
	~j1Fade();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool Fade(float time = 2.0f);



	//Module* moduleOff = nullptr;
	//Module* moduleOn = nullptr;

	SDL_Texture* loadingTex;

	SDL_Rect LoadingRect = { 724,903,277,41 };

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;
private:

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
};

#endif //__MODULEFADETOBLACK_