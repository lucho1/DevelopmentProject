#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{

	onTopSFX = App->audio->LoadFx("audio/fx/button_hover_sfx.wav");
	clickSFX = App->audio->LoadFx("audio/fx/button_clicked_sfx.wav");
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{

	return true;
}
bool j1Gui::Update() {

	return true;
}
// Called after all Updates
bool j1Gui::PostUpdate()
{
	p2List_item<UI_Element*>* Item = UI_Elements_List.start;
	for (; Item != nullptr; Item = Item->next) {

		Item->data->Update();
		if (Item->data->isActive) {
			Item->data->onTop();
			if (Item->data->Clicked() == false) {
				
			}
			if (Item->data->onTop() == false) {
				Item->data->sound = true;
				Item->data->isClicked = false;
			}
			Item->data->Draw();
		}
	}

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

UI_Element* j1Gui::Add_UIElement(UI_Type type, iPoint position, SDL_Rect Image_Rect,
	Button_Logic logic, SDL_Rect Image_Rect_Active, SDL_Rect ButtonPush, float scale, SDL_Color Color,
	UI_Element* Parent, const char* Text) {

	UI_Element* ret = nullptr;

	if (type == BUTTON || type == PANEL || type == HANDLEINPUT) {
		ret = new UI_Element(type, position, Image_Rect, logic, Parent);
		ret->texture = GetAtlas();
		UI_Elements_List.add(ret);
		ret->UI_Rect_Active = Image_Rect_Active;
		ret->UI_Rect_Pushed = ButtonPush;
		if (Parent != nullptr) {
			ret->Parent = Parent;
			Parent->Child_List.add(ret);
		}
	}
	if (type == NON_INTERACTIVE) {

		ret = new UI_Element(type, position, Image_Rect, logic, Parent);
		ret->texture = GetAtlas();
		UI_Elements_List.add(ret);
		if (Parent != nullptr) {
			ret->Parent = Parent;
			Parent->Child_List.add(ret);
		}
	}
	else if (type == LABEL) {

		ret = new UI_Element(type, position, logic, Text, Parent);
		ret->texture = App->font->Print(Text, Color);

		ret->UI_Rect.x = 0;
		ret->UI_Rect.y = 0;
		App->font->CalcSize(Text, ret->UI_Rect.w, ret->UI_Rect.h);
		UI_Elements_List.add(ret);
		ret->CurrentRect = &ret->UI_Rect;
		if (Parent != nullptr) {
			ret->Parent = Parent;
			Parent->Child_List.add(ret);
		}
	}
	ret->scale = scale;
	ret->isActive = true;
	return ret;

}

void UI_Element::Draw() {
	if(Logic!=SAVE_REMEMBER)
		App->render->Blit(texture, Position.x, Position.y, CurrentRect, false, angle, 0, 0, SDL_FLIP_NONE, scale);
	else
		App->render->Blit(texture, Position.x, Position.y, CurrentRect, 1.0f, angle, 0, 0, SDL_FLIP_NONE, scale);
	if (App->gui->Debug == true)
		App->render->DrawQuad({ Position.x,Position.y,UI_Rect.w,UI_Rect.h }, 255, 0, 0, 255, false, false, scale);

}
bool UI_Element::onTop() {
	int y;
	int x;

	App->input->GetMousePosition(x, y);

	if (x >= Position.x&&x < Position.x + UI_Rect.w*scale&&y >= Position.y&&y <= Position.y + UI_Rect.h*scale) {
		if (sound) {
			if(type==BUTTON)
				App->audio->PlayFx(App->gui->onTopSFX);
			sound = false;
		}
		return true;
		}
	else
		return false;
	
}

bool UI_Element::Clicked() {

	if (onTop()) {
		if (App->input->GetMouseButtonDown(KEY_DOWN) && isClicked == false) {
			App->audio->PlayFx(App->gui->clickSFX);
			if(Logic!=DRAGVOLUME)
				isClicked = true;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}