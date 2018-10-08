
#include "j1App.h"
#include "j1Colliders.h"
#include "j1Input.h"
#include "j1Render.h"



j1Colliders::j1Colliders()
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


j1Colliders::~j1Colliders()
{
}


bool j1Colliders::PreUpdate() {

	bool ret = true;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	// create the colliders to compare if they have collisioned
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		for (uint j = i + 1; j < MAX_COLLIDERS; ++j)
		{

			if (colliders[j] == nullptr) //empty colliders
				continue;

			c2 = colliders[j];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)		//this compares the matrix to see if is true or false and then 
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}

	return ret;
}

bool j1Colliders::Update(float dt)
{
	bool ret = true;

	DebugDraw();

	return ret;
}

void j1Colliders::DebugDraw() {

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (debug == false) 
		return;
	
	for (uint i=0; i < MAX_COLLIDERS; i++) {

		if (colliders[i] == nullptr) {
			continue;
		}

		switch (colliders[i]->type)
		{
			case COLLIDER_NONE:
				App->render->DrawQuad(colliders[i]->rect, 255, 255, 255,40);
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

bool j1Colliders::CleanUp()
{
	
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Colliders::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback )
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	if ((r.x + r.w) < rect.x || (r.y + r.h) < rect.y || r.x > (rect.x + rect.w) || r.y > (rect.y + rect.h)) { return false; }
	return true;
}
