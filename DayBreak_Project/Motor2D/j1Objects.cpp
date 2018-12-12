#include "j1Objects.h"
#include "j1EntityManager.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "p2Log.h"

j1Objects::j1Objects(iPoint pos, OBJECT_TYPE objType) : j1Entity(ENTITY_TYPE::OBJECT_ENT) { name.create("Objects"); }

j1Objects::~j1Objects() {}

j1Objects* j1Objects::CreateObject(iPoint pos, OBJECT_TYPE objType) {

	j1Objects* Object = nullptr;
	Object = new j1Objects(pos, objType);

	App->entity_manager->entities_list.add(Object);
	return Object;

}