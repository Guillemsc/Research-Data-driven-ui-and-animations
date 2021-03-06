#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1Console.h"
#include "MenuScene.h"
#include "SecondScene.h"
#include "Functions.h"

#define NUMBER_OF_PLAYERS 4

j1Scene::j1Scene() : j1Module()
{
	name = "scene";
}

// Destructor
j1Scene::~j1Scene()
{

}

// Called before render is available
bool j1Scene::Awake()
{
	bool ret = true;

	LOG("Loading SceneManager");

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = false;

	LOG("Start module scene");

	// Creating scenes
	menu_scene = new MenuScene();		scenes.push_back(menu_scene);
	second_scene = new SecondScene();	scenes.push_back(second_scene);
	// -------------

	// Starting scene
	current_scene = menu_scene;

	// Start scene
	if (current_scene != nullptr)
		ret = current_scene->Start();
	

	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	bool ret = false;

	if (current_scene != nullptr)
		ret = current_scene->PreUpdate();

	return ret;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	bool ret = false;

	if (current_scene != nullptr)
		ret = current_scene->Update(dt);
	
	return ret;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = false;

	if (current_scene != nullptr)
		ret = current_scene->PostUpdate();

	DoChangeScene();

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scenes");

	bool ret = false;
	if (current_scene != nullptr)
		ret = current_scene->CleanUp();

	for (list<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it)
	{
		RELEASE(*it);
	}

	return ret;
}

void j1Scene::ChangeScene(Scene * new_scene)
{
	LOG("Changing current scene");

	current_scene->next_scene = new_scene;
	current_scene->change_scene = true;
}

Scene * j1Scene::GetCurrentScene()
{
	return current_scene;
}

Scene * j1Scene::GetSceneByName(const char * name)
{
	Scene* ret = nullptr;

	for (list<Scene*>::iterator it = scenes.begin(); it != scenes.end(); it++)
	{
		if (TextCmp((*it)->GetName(), name))
		{
			ret = *it;
			break;
		}
	}

	return ret;
}

void j1Scene::OnCommand(std::list<std::string>& tokens)
{
	current_scene->OnCommand(tokens);
}

void j1Scene::OnCVar(std::list<std::string>& tokens)
{
	current_scene->OnCVar(tokens);
}

void j1Scene::SaveCVar(std::string & cvar_name, pugi::xml_node & node) const
{
	current_scene->SaveCVar(cvar_name,node);
}

void j1Scene::DoChangeScene()
{
	if (current_scene->change_scene)
	{
		current_scene->change_scene = false;
		Scene* last_scene = current_scene;
		current_scene = current_scene->next_scene;
		last_scene->CleanUp();
		current_scene->Start();
	}
}



