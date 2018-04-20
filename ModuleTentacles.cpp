#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleTentacles.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"

ModuleTentacles::ModuleTentacles()
{}

ModuleTentacles::~ModuleTentacles()
{}

bool ModuleTentacles::Start() {

	LOG("Loading tentacle textures");

	graphics = App->textures->Load("Assets/Player.png");
	tentacle.anim.PushBack({ 21, 19, 7, 11 });
	tentacle.anim.loop = false;
	tentacle.anim.speed = 0.0f;

	anchor.anim.PushBack({ 90, 18, 19, 9 });
	anchor.anim.loop = false;
	anchor.anim.speed = 0.0f;

	/*
	tentacle.anim->PushBack({ 17,0,16,16 });
	tentacle.anim->PushBack({ 33,0,16,16 });
	tentacle.anim->PushBack({ 49,0,16,16 });
	tentacle.anim->PushBack({ 65,0,16,16 });*/
	return true;
}

bool ModuleTentacles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);

	for (uint i = 0; i < MAX_TENTACLES; ++i)
	{
		if (tentacles[i] != nullptr)
		{
			delete tentacles[i];
			tentacles[i] = nullptr;
		}
	}

	return true;
}

update_status ModuleTentacles::Update() {
	for (uint i = 0; i < MAX_TENTACLES; ++i)
	{
		Tentacle* p = tentacles[i];

		if (p == nullptr)
			continue;

		if (p->Update() == false)
		{
			delete p;
			tentacles[i] = nullptr;
		}else{
			setPosition(App->player->position.x, App->player->position.y);
			App->render->Blit(graphics, p->first_point.x, p->first_point.y, &p->anim.GetCurrentFrame(),1.0f, 0.0f);

		}
	}
	return UPDATE_CONTINUE;
}

void ModuleTentacles::AddTentacle(const Tentacle& tentacle, int x, int y, bool flip, bool anchor)
{
	for (uint i = 0; i < MAX_TENTACLES; ++i)
	{
		if (tentacles[i] == nullptr)
		{
			Tentacle* p = new Tentacle(tentacle);
			p->flip = flip;
			p->anchor = anchor;
			p->first_point.x = x;
			p->first_point.y = y;
			tentacles[i] = p;
			break;
		}
	}
}

void ModuleTentacles::setPosition(int x, int y) {
	for (uint i = 0; i < MAX_TENTACLES; ++i)
	{
		if (tentacles[i] != nullptr)
		{
			int invert = 0;
			Tentacle* p = tentacles[i];
			if (i >= MAX_TENTACLES/2)
				invert = -1;
			else
				invert = 1;

			if (i != 0 && i != MAX_TENTACLES/2) {
				p->first_point.x = tentacles[i - 1]->second_point.x;
				p->first_point.y = tentacles[i - 1]->second_point.y;
			}
			else {
				if (tentacles[i]->flip) {
					p->first_point.y = y + 9;
				}
				else {
					p->first_point.y = y - 4;
				}
				p->first_point.x = x + 20;
			}

			p->second_point.x = p->first_point.x + 4;
			p->second_point.y = p->first_point.y - p->anim.GetCurrentFrame().h * invert;
			tentacles[i] = p;
		}
	}
}

void ModuleTentacles::ShootLaser() {
	for (uint i = 0; i < MAX_TENTACLES; ++i)
	{
		if (tentacles[i] != nullptr)
		{
			int invert = 0;
			if (i >= MAX_TENTACLES / 2)
				invert = -1;
			else
				invert = -1;
			Tentacle* p = tentacles[i];
			if (p->anchor) {
				App->particles->AddParticle(App->particles->basic_laser, p->first_point.x, p->first_point.y - (App->particles->basic_laser.anim.GetCurrentFrame().h / 2) * invert, COLLIDER_PLAYER_SHOT);
			}
		}
	}
}


//--------------------------------------------
//--------------------------------------------

Tentacle::Tentacle()
{
	first_point.SetToZero();
}

Tentacle::Tentacle(const Tentacle& p) :
	first_point(p.first_point), second_point(p.second_point), 
	anchor(p.anchor), anim(p.anim)
{}

bool Tentacle::Update()
{
	bool ret = true;


	return ret;
}