#include "j1App.h"
#include "j1Collisions.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Map.h"


j1Collisions::j1Collisions()
{

	//Set all matrix positions to false
	for (int i = 0; i < COLLIDER_MAX; i++) {
		for (int j = 0; j < COLLIDER_MAX; j++)
			matrix[i][j] = false;
	}

	//Set all matrix positions of "PLAYER" to true except PLAYER-NONE & PLAYER-PLAYER (which remain false)
	for (int p = 0; p < COLLIDER_MAX - 1; p++) {

		if (p != COLLIDER_NONE || p != COLLIDER_PLAYER|| p!=COLLIDER_PLAYER_BULLET)
			matrix[COLLIDER_PLAYER][p] = true;
	}

	//Set all matrix positions that must be true to true (the others remain false)
	matrix[TRIGGER_WIN][COLLIDER_PLAYER] = true;
	matrix[TRIGGER_PUSHOFF][COLLIDER_PLAYER] = true;
	matrix[TRIGGER_PUSH][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_BLINKING][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_FALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_STATIC][TRIGGER_PUSHOFF] = true;
	matrix[COLLIDER_STATIC][COLLIDER_PLAYER] = true;

	//Set enemy matrix
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER_BULLET] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_STATIC] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_FALL] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_BLINKING] = true;

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
	
	Init();

	if (App->scene->currentLevel == LEVEL1)
		AssignMapColliders("Level1.tmx"); //This should be called in scene!!!!

	else if (App->scene->currentLevel == LEVEL2)
		AssignMapColliders("Level2.tmx");

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


	return ret;
}


bool j1Collisions::Update(float dt) {
	
	Collider *c1;
	Collider *c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {

		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		for (uint j = 0; j < MAX_COLLIDERS; ++j) {

			if (colliders[j] == nullptr)
				continue;

			c2 = colliders[j];

			if (c1->CheckCollision(c2->rect)) {

				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);
				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);

				if (matrix[c1->type][c2->type] && c1->callback2)
					c1->callback2->OnCollision(c1, c2);
				if (matrix[c2->type][c1->type] && c2->callback2)
					c2->callback2->OnCollision(c2, c1);
			}
		}
	}
	return true;
}

bool j1Collisions::PostUpdate() {

	DebugDraw();
	return true;
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
		case COLLIDER_FALL:
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, 40);
			break;
		case COLLIDER_BLINKING:
			App->render->DrawQuad(colliders[i]->rect, 0, 200, 200, 40);
			break;
		case TRIGGER_PUSH:
			App->render->DrawQuad(colliders[i]->rect, 200, 0, 200, 40);
			break;
		case TRIGGER_PUSHOFF:
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 200, 40);
			break;
		case TRIGGER_WIN:
			App->render->DrawQuad(colliders[i]->rect, 200 ,100, 255, 40);
			break;
		case COLLIDER_ENEMY:
			App->render->DrawQuad(colliders[i]->rect, 200, 200, 200, 40);
			break;
		case COLLIDER_PLAYER_BULLET:
			App->render->DrawQuad(colliders[i]->rect, 100, 50, 73, 100);
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

Collider* j1Collisions::AddColliderEntity(SDL_Rect rect, COLLIDER_TYPE type, j1Entity *callback) {

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

void j1Collisions::AssignMapColliders(const char* file_name) { 

	p2SString tmp("%s%s", App->map->folder.GetString(), file_name);

	pugi::xml_parse_result result = App->map->map_file.load_file(tmp.GetString());

	if (result == NULL) {

		LOG("Could not load xml file %s. pugi error: %s", file_name, result.description());
		return;
	}
	pugi::xml_node collider;
	pugi::xml_node type;
	const char* collidertype;

	for (type = App->map->map_file.child("map").child("objectgroup");type && result; type = type.next_sibling("objectgroup")) {

		collidertype = type.attribute("name").as_string();

		for (collider = type.child("object"); collider&&result; collider= collider.next_sibling("object")) {
			
			if (strcmp(collidertype, "Colliders") == 0) {
				AddCollider({ collider.attribute("x").as_int(),collider.attribute("y").as_int(),collider.attribute("width").as_int(),collider.attribute("height").as_int() }, COLLIDER_TYPE::COLLIDER_STATIC);
			}
			if (strcmp(collidertype, "Death_Colliders") == 0) {
				AddCollider({ collider.attribute("x").as_int(),collider.attribute("y").as_int(),collider.attribute("width").as_int(),collider.attribute("height").as_int() }, COLLIDER_TYPE::COLLIDER_FALL);
			}
			if (strcmp(collidertype, "Bloc_Colliders") == 0) {
				AddCollider({ collider.attribute("x").as_int(),collider.attribute("y").as_int(),collider.attribute("width").as_int(),collider.attribute("height").as_int() }, COLLIDER_TYPE::COLLIDER_BLINKING);
			}
			if (strcmp(collidertype, "Push_Triggers") == 0) {
				AddCollider({ collider.attribute("x").as_int(),collider.attribute("y").as_int(),collider.attribute("width").as_int(),collider.attribute("height").as_int() }, COLLIDER_TYPE::TRIGGER_PUSH);
			}
			if (strcmp(collidertype, "PushOff_Triggers") == 0) {
				AddCollider({ collider.attribute("x").as_int(),collider.attribute("y").as_int(),collider.attribute("width").as_int(),collider.attribute("height").as_int() }, COLLIDER_TYPE::TRIGGER_PUSHOFF);
			}
			if (strcmp(collidertype, "Win_Trigger") == 0) {
				AddCollider({ collider.attribute("x").as_int(),collider.attribute("y").as_int(),collider.attribute("width").as_int(),collider.attribute("height").as_int() }, COLLIDER_TYPE::TRIGGER_WIN);
			}
		}
	}
}