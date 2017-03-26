#ifndef _SECONDSCENE_
#define _SECONDSCENE_

#include "Scene.h"

class SecondScene : public Scene
{
public:
	SecondScene();

	virtual ~SecondScene();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	void OnCommand(std::list<std::string>& tokens);

	void OnCVar(std::list<std::string>& tokens);

	void SaveCVar(std::string& cvar_name, pugi::xml_node& node) const;
};

#endif