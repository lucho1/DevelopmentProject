#include "j1Objects.h"
#include "j1EntityManager.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Scene.h"

j1Objects::j1Objects(iPoint pos, OBJECT_TYPE objType) : j1Entity(ENTITY_TYPE::OBJECT_ENT) { name.create("Objects"); }

j1Objects::~j1Objects() {}

j1Objects* j1Objects::CreateObject(iPoint pos, OBJECT_TYPE objType) {

	j1Objects* Object = nullptr;
	Object = new j1Objects(pos, objType);

	if (objType == PUSHOFF) {

		pugi::xml_document doc;

		if(App->scene->currentLevel == LEVEL1)
			pugi::xml_parse_result res = doc.load_file("Level_1_settings.xml");
		if(App->scene->currentLevel == LEVEL2)
			pugi::xml_parse_result res = doc.load_file("Level_2_settings.xml");

		pugi::xml_node node = doc.child("game_state").child("Objects");
	//	LoadPushbacks(node, pushoff_anim);
	}

	App->entity_manager->entities_list.add(Object);
	return Object;

}

void j1Objects::LoadPushbacks(pugi::xml_node node, Animation &animation) {

	animation.loop = node.attribute("loop").as_bool();
	animation.speed = node.attribute("speed").as_float();
	SDL_Rect rect;

	for (node = node.child("Pushback"); node; node = node.next_sibling("Pushback")) {

		rect.x = node.attribute("x").as_int();
		rect.y = node.attribute("y").as_int();
		rect.w = node.attribute("w").as_int();
		rect.h = node.attribute("h").as_int();
		animation.PushBack({ rect });
	}
}

/*
animation.loop = node.attribute("loop").as_bool();
animation.speed = node.attribute("speed").as_float();
SDL_Rect rect;

for (node = node.child("Pushback"); node; node = node.next_sibling("Pushback")) {

rect.x = node.attribute("x").as_int();
rect.y = node.attribute("y").as_int();
rect.w = node.attribute("w").as_int();
rect.h = node.attribute("h").as_int();
animation.PushBack({ rect });
}

*/