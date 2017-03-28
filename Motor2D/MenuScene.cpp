#include "MenuScene.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Scene.h"

MenuScene::MenuScene() : Scene("menu_scene")
{
}

MenuScene::~MenuScene()
{
}

bool MenuScene::Start()
{
	bool ret = true;

	return ret;
}

bool MenuScene::PreUpdate()
{
	bool ret = true;



	return ret;
}

bool MenuScene::Update(float dt)
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		App->scene->ChangeScene((Scene*)App->scene->second_scene);
	}
	
	return ret;
}

bool MenuScene::PostUpdate()
{
	bool ret = true;



	return ret;
}

bool MenuScene::CleanUp()
{
	bool ret = true;



	return ret;
}

bool MenuScene::Load(pugi::xml_node &)
{
	bool ret = true;



	return ret;
}

bool MenuScene::Save(pugi::xml_node &) const
{
	bool ret = true;



	return ret;
}

void MenuScene::OnCommand(std::list<std::string>& tokens)
{
}

void MenuScene::OnCVar(std::list<std::string>& tokens)
{
}

void MenuScene::SaveCVar(std::string & cvar_name, pugi::xml_node & node) const
{
}
