#include <math.h>
#include "p2Defs.h"
#include "j1App.h"
#include "j1Fade.h"
#include "j1Render.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "j1Player.h"
#include "j1Window.h"

j1Fade::j1Fade()
{
	
	//screen = { 0, 0, App->win->GetWindowWidth(),App->win->GetWindowHeight() };

}

j1Fade::~j1Fade()
{}

// Load assets
bool j1Fade::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool j1Fade::Update()
{
	bool ret = true;

	if (current_step == fade_step::none)
		return ret = false;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{

			//moduleOn->Enable();
			//moduleOff->Disable();

			// TODO 3: enable / disable the modules received when FadeToBlacks() gets called

			// ---
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;

		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = fade_step::none;
	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return ret;
}

// Fade to black. At mid point deactivate one module, then activate the other
//bool j1Fade::FadeToBlack(Module* module_off, Module* module_on, float time)
//{
//	bool ret = false;
//	if (App->player->IsEnabled()) {
//		App->player->Disable();
//	}
//	if (App->player2->IsEnabled()) {
//		App->player2->Disable();
//	}
//	moduleOff = module_off;
//	moduleOn = module_on;
//
//	if (current_step == fade_step::none)
//	{
//		current_step = fade_step::fade_to_black;
//		start_time = SDL_GetTicks();
//		total_time = (Uint32)(time * 0.5f * 1000.0f);
//		ret = true;
//	}
//
//	return ret;
//}