#ifndef _SCENE_
#define _SCENE_

#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"

#include <string>
#include <list>

class Scene
{
public:
	Scene(char* _name) { name = _name; };

	virtual ~Scene() {};

	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	char* GetName() { return name; };

	virtual void OnCommand(std::list<std::string>& tokens) {}

	virtual void OnCVar(std::list<std::string>& tokens) {}

	virtual void SaveCVar(std::string& cvar_name, pugi::xml_node& node) const {}

	virtual bool isDone() { return change_scene; }

public:
	bool change_scene = false;
	Scene* next_scene = nullptr;

protected:

private:
	char* name;

};

#endif