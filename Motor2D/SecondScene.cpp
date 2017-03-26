#include "SecondScene.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Scene.h"

SecondScene::SecondScene() : Scene("second_scene")
{
}

SecondScene::~SecondScene()
{
}

bool SecondScene::Start()
{
	bool ret = true;



	return ret;
}

bool SecondScene::PreUpdate()
{
	bool ret = true;



	return ret;
}

bool SecondScene::Update(float dt)
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		App->scene->ChangeScene((Scene*)App->scene->menu_scene);
	}

	return ret;
}

bool SecondScene::PostUpdate()
{
	bool ret = true;



	return ret;
}

bool SecondScene::CleanUp()
{
	bool ret = true;



	return ret;
}

bool SecondScene::Load(pugi::xml_node &)
{
	return false;
}

bool SecondScene::Save(pugi::xml_node &) const
{
	bool ret = true;



	return ret;
}

void SecondScene::OnCommand(std::list<std::string>& tokens)
{
}

void SecondScene::OnCVar(std::list<std::string>& tokens)
{
}

void SecondScene::SaveCVar(std::string & cvar_name, pugi::xml_node & node) const
{
}
