
#ifndef __j1GUI_H__
#define __j1GUI_H__
#define _CRT_SECURE_NO_WARNINGS


#include "j1Module.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Fonts.h"
#include "j1Audio.h"


#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

enum UI_Type {

	BUTTON,
	LABEL,
	PANEL,
	HANDLEINPUT,
	NON_INTERACTIVE,
	NONE_TYPE
};
enum Button_Logic {
	QUIT,
	LIFE,
	RETURN_MAIN_MENU,
	RESUME_GAME,
	LIFE_ALERT,
	SAVE_REMEMBER,
	LOAD,
	COINS,
	TIME,
	PLAY,
	SCORE,
	CONTINUE,
	DRAG,
	WRITE,
	ACTIVEWIN,
	CLOSEWIN,
	PUSH,
	WEB,
	CLICKTOWRITE,
	DRAGVOLUME,
	NONE_LOGIC
};

class UI_Element {

public:

	UI_Element() {};
	UI_Element(UI_Type type, iPoint position, SDL_Rect Image_Rect, Button_Logic logic, UI_Element* Parent = nullptr)
		:type(type), UI_Rect(Image_Rect), Position(position), initialPosition(position), Logic(logic), Parent(Parent) {
		if (this->isParent()) {
			position.x = Parent->Position.x + initialPosition.x;
			position.y = Parent->Position.y + initialPosition.y;
		}
		CurrentRect = &UI_Rect;
	};


	UI_Element(UI_Type type, iPoint position, Button_Logic logic, const char* Text = nullptr, UI_Element* Parent = nullptr)
		:type(type), Position(position), initialPosition(position), Logic(logic), Parent(Parent) {
		if (this->isParent()) {
			Position.x = Parent->Position.x + initialPosition.x;
			Position.y = Parent->Position.y + initialPosition.y;
		}
	};

public:

	bool isParent() {
		if (Parent == nullptr)
			return false;
		else
			return true;
	}

	void Score(int score) {
	char score_Text[10];
	sprintf_s(score_Text, "%d", score); //warning: deprecated
	App->font->CalcSize(score_Text, UI_Rect.w, UI_Rect.h);
	texture = App->font->Print(score_Text);
	}
	void Time(int time) {
		char score_Text[10];
		sprintf_s(score_Text, "%02i", time); //warning: deprecated
		App->font->CalcSize(score_Text, UI_Rect.w, UI_Rect.h);
		texture = App->font->Print(score_Text);
	}
	virtual void Drag() {
		if (Clicked()) {
			int x = 0;
			int y = 0;
			App->input->GetMouseMotion(x, y);
			if (isParent()) {
				initialPosition.x += x;
				initialPosition.y += y;
			}
			else
			{
				Position.x += x;
				Position.y += y;
			}
		}
	}

	virtual void Update() {
		if (isParent()) {
			Position.x = Parent->Position.x + initialPosition.x;
			Position.y = Parent->Position.y + initialPosition.y;
		}
		if (type == BUTTON) {
			if (onTop()) {
				if (Clicked() == false) {
					CurrentRect = &UI_Rect_Active;
				}
				else
					CurrentRect = &UI_Rect_Pushed;
			}
			else if ((Logic == LOAD || Logic == CONTINUE) && App->GetSaves("save_game.xml") == false) {
				CurrentRect = &UI_Rect_Pushed;
			}
			else {
				CurrentRect = &UI_Rect;
			}
		}
		if (Child_List.count() > 0) {
			p2List_item<UI_Element*>* Item = Child_List.start;
			for (; Item != nullptr; Item = Item->next) {
				if (isActive)
					Item->data->isActive = true;
				else
					Item->data->isActive = false;
			}
		}
	}
	void Alert(SDL_Texture* texture,int& alpha) {
		if (alpha <= 255&&alphaReach) {
			alpha -= 7;
		}
		else if (alpha >= 90) {
			alphaReach = false;
			alpha += 2;
			if (alpha >= 255) {
				alphaReach = true;
			}
		}
		//SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(texture, alpha);
	}
	virtual void Active(UI_Element* toActive) {
		toActive->isActive = true;
	}
	virtual void Deactive(UI_Element* toDeactive) {
		toDeactive->isActive = false;
	}

	void Draw();

	void goWeb(const char* web) {

		ShellExecuteA(NULL, "open", web, NULL, NULL, SW_SHOWNORMAL);

	}
	void VolumeControl(iPoint newMousePos, iPoint lastMousePos) {

		if (Position.x >= Parent->Position.x&&Position.x + UI_Rect.w*scale < (Parent->Position.x + Parent->UI_Rect.w*scale))
			initialPosition.x += newMousePos.x - lastMousePos.x;
		else if (Position.x < Parent->Position.x)
			initialPosition.x = 0;
		else
			initialPosition.x = Parent->UI_Rect.w*scale - UI_Rect.w*scale - 1;
	}

	bool onTop();

	bool Clicked();


public:


	iPoint initialPosition;
	iPoint Position;

	UI_Type type;
	Button_Logic Logic;

	const char* text = nullptr;
	SDL_Rect UI_Rect;
	SDL_Rect UI_Rect_Active;
	SDL_Rect UI_Rect_Pushed;
	SDL_Rect* CurrentRect;
	SDL_Texture* texture;

	p2List<UI_Element*> Child_List;
	UI_Element* Parent = nullptr;
	float scale=1.0f;
	float angle = 0.0f;
	bool isClicked = false;;
	bool isActive = true;
	bool toDesactive = false;
	bool toActive = false;
	bool sound = true;

	bool alphaReach = true;
	int alpha=255;


};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update();
	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	SDL_Texture* GetAtlas() const;


	iPoint lastMousePos = { 0,0 };
	iPoint newMousePos = { 0,0 };

	UI_Element* Add_UIElement(UI_Type type, iPoint position, SDL_Rect Image_Rect, Button_Logic = NONE_LOGIC, SDL_Rect Image_Rect_Active = NULL_RECT, SDL_Rect ButtonPush = NULL_RECT,float scale=1.0f, SDL_Color Color = { 255,255,255,255 }, UI_Element* Parent = nullptr, const char* Text = nullptr);

	//UI_Element* Add_UIElement(UI_Type type, iPoint position, const char* Text = nullptr, UI_Element* Parent = nullptr);

	p2List<UI_Element*> UI_Elements_List;
	UI_Element*last;

	uint32 onTopSFX;
	uint32 clickSFX;

	bool Debug = false;
private:

	SDL_Rect a = { 0, 0, 219, 57 };

	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__