#ifndef _MAINSCENE_
#define _MAINSCENE_

#include "Scene.h"
class UI_Button;
class UI_Text;
class MenuScene : public Scene
{
public:
	MenuScene();

	virtual ~MenuScene();

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

private:
	UI_Button* button_minus = nullptr;
	UI_Button* button_plus = nullptr;
	UI_Text*   percentage_text = nullptr;
};

#endif