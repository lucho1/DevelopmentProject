#include "j1App.h"
#include "j1Collisions.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Map.h"


j1Collisions::j1Collisions()
{
	matrix[COLLIDER_NONE][COLLIDER_NONE] = false;
	matrix[COLLIDER_NONE][COLLIDER_STATIC] = false;
	matrix[COLLIDER_NONE][COLLIDER_PLAYER] = false;

	matrix[COLLIDER_STATIC][COLLIDER_NONE] = false;
	matrix[COLLIDER_STATIC][COLLIDER_STATIC] = false;
	matrix[COLLIDER_STATIC][COLLIDER_PLAYER] = true;

	matrix[COLLIDER_PLAYER][COLLIDER_NONE] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_STATIC] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
}


j1Collisions::~j1Collisions()
{
}

bool j1Collisions::Awake() {

	LOG("Collisions Module Loading");
	bool ret = true;
	return ret;
}

bool j1Collisions::Start() {

	return true;
}


bool j1Collisions::PreUpdate() {

	bool ret = true;

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {

		if (colliders[i] != nullptr && colliders[i]->to_delete) {

			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider *c1;
	Collider *c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {

		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		for (uint j = i + 1; j < MAX_COLLIDERS; ++j) {

			if (colliders[j] == nullptr)
				continue;

			c2 = colliders[j];

			if (c1->CheckCollision(c2->rect)) {

				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);
				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}

	return ret;
}


bool j1Collisions::Update(float dt) {

	bool ret = true;
	DebugDraw();
	return ret;
}

void j1Collisions::DebugDraw() {

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;
	if (!debug)
		return;

	for (uint i = 0; i < MAX_COLLIDERS; i++) {

		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type) {

		case COLLIDER_NONE:
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, 40);
			break;
		case COLLIDER_STATIC:
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, 40);
			break;
		case COLLIDER_PLAYER:
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, 40);
			break;
		default:
			break;

		}
	}
}


bool j1Collisions::CleanUp(){

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {

		if (colliders[i] != nullptr) {

			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}


Collider* j1Collisions::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module *callback) {

	Collider *ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {

		if (colliders[i] == nullptr) {

			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}
	return ret;
}



bool Collider::CheckCollision(const SDL_Rect &r) const {

	if ((r.x + r.w) < rect.x || (r.y + r.h) < rect.y || r.x > (rect.x + rect.w) || r.y > (rect.y + rect.h)) { return false; }
	return true;
}


void j1Collisions::AssignMapColliders(pugi::xml_node &node, const SDL_Rect r) { //At node pass the node layer = map_file.child("map").child("layer") It's at the map

	p2List_item<MapLayer*>*layers = App->map->data.layers.start;

	while (layers != nullptr) {

		if (layers->data->name == "Colliders") {

			pugi::xml_node layer_data = node.child("data");

			for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile")) {

				if(tile.attribute("gid").as_int() == 39){


				}
			}

			break;
		}
		else
			layers = layers->next;
	}

	LOG("Error Parsing map, couldn't find colliders layer");
}