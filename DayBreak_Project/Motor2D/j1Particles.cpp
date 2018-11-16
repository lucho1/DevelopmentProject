
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Particles.h"
#include "j1Player.h"

#include "SDL/include/SDL_timer.h"


j1Particles::j1Particles() {

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) 
		active[i] = nullptr;
	
	//	srand(2);
}

j1Particles::~j1Particles() {}

bool j1Particles::Start() {

	//LOG("Loading Particles");
	//particle0 = App->textures->Load("Images/Player/Ship&Ball_Sprite.png");
	Particle_1 = App->tex->Load("maps/Projectil_test.png");
	Shoot.Anim.PushBack({ 0,0,163,50 });
	Shoot.Anim.PushBack({ 163,0,163,50 });
	Shoot.Anim.PushBack({ 326,0,163,50 });
	Shoot.Anim.PushBack({ 0,50,163,50 });
	Shoot.Anim.loop = true;
	Shoot.Anim.speed = 0.15f;
	Shoot.Life = 1200;
	Shoot.Sprites = Particle_1;

	////BOOOOOOOOOOOOOOOOOOOOOOOOOOOMBAAAA un movimiento muy sexy: se-xy, un movimiento muy sensual: sen-sual
	//HipopotamoBomba.Anim.PushBack({ 0, 306, 16, 6 });
	//HipopotamoBomba.Anim.PushBack({ 17, 303, 15, 10 });
	//HipopotamoBomba.Anim.PushBack({ 34, 301, 10, 15 });

	//HipopotamoBomba.Anim.loop = false;
	//HipopotamoBomba.Anim.speed = 0.1f;
	//HipopotamoBomba.Sprites = Particle0;
	//HipopotamoBomba.Life = 1200;
	//HipopotamoBomba.Speed.y = 3;
	//HipopotamoBomba.Speed.x = (HipopotamoBomba.Position.x ^ 2);


	return true;

}

bool j1Particles::CleanUp() {

	LOG("Unloading Particles");

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) {

		if (active[i] != nullptr) {
			App->tex->UnLoad(active[i]->Sprites);
			delete active[i];
			active[i] = nullptr;
		}
	}

	return true;
}

bool j1Particles::Update(float ds) {

	bool ret = true;
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) {

		Particle *p = active[i];

		if (p == nullptr) {
			continue;
		}

		if (p->Update() == false) {

			delete p;
			active[i] = nullptr;
		}

		else if (SDL_GetTicks() >= p->Born) {

			App->render->Blit(p->Sprites, p->Position.x, p->Position.y, &(p->Anim.GetCurrentFrame()),1,0,0,0,p->Flip);

			if (!p->fx_played)
				p->fx_played = true;
			
		}
	}

	return ret;
}

void j1Particles::AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type, iPoint speed,SDL_RendererFlip Flip, Uint32 delay) {

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) {
		if (active[i] == nullptr) {

			Particle* p = new Particle(particle);
			p->Born = SDL_GetTicks() + delay;
			p->Position.x = x;
			p->Position.y = y;
			p->Sprites = particle.Sprites;
			p->Speed = speed;
			p->Flip = Flip;
			if (collider_type != COLLIDER_NONE) {
				p->collider = App->collisions->AddCollider(p->Anim.GetCurrentFrame(), collider_type, this);
			}

			active[i] = p;
			break;

		}
	}
}

void j1Particles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr && active[i]->collider == c1) {

			if (c2->type == COLLIDER_STATIC /*|| c2->type == COLLIDER_WALL*/) {

				delete active[i];
				active[i] = nullptr;

				break;
			}
		}
	}
}

Particle::Particle() {

	Position.SetToZero();
	Speed.SetToZero();
}

Particle::Particle(const Particle &p) :
	Anim(p.Anim), Position(p.Position), Speed(p.Speed),
	fx(p.fx), Born(p.Born), Life(p.Life) {}

Particle::~Particle()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

bool Particle::Update() {

	bool ret = true;

	if (Life > 0) {

		if (((int)SDL_GetTicks() - (int)Born) > (int)Life)
			ret = false;
	}
	else
		if (Anim.Finished())
			ret = false;

	if (collider != nullptr)
		ret = false;

	if (SDL_GetTicks() >= Born) {

		Position.x += Speed.x;
		Position.y += Speed.y;

		if (collider != nullptr)
			collider->SetPos(Position.x, Position.y);
	}

	return ret;

}