#ifndef __j1UDIO_H__
#define __j1UDIO_H__

#include "j1Module.h"
#include "j1Render.h"

class DDUI_Scene;
class DDUI_Element;
class UI_Element;
class UI_Window;
class UI_Button;
class UI_Text;
class UI_Image;
class UI_Text_Input;
class UI_Scroll_Bar;

class DataDrivenUI : public j1Module
{
public:

	DataDrivenUI();

	virtual ~DataDrivenUI();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	UI_Element* GetElementByName(char* name);

private:
	void LoadScene(char* name);

	void CheckForElements(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* curr_win, vector<UI_Element*>& added_elements);

	void CheckForGeneralVars(pugi::xml_node element_node, UI_Element* element);

	UI_Window* AddWindow(pugi::xml_node element_node, DDUI_Scene* scene);
	UI_Button* AddButton(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* window);
	UI_Text* AddText(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* window);
	UI_Image* AddImage(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* window);
	UI_Text_Input* AddTextInput(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* window);
	UI_Scroll_Bar* AddScrollBar(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* window);
	UI_ColoredRect* AddColoredRect(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* window);

public:

private:
	vector<DDUI_Scene*> scenes;
	pugi::xml_document xml;

};

// ---------------------------
// ---------------------- DataDrivenUI

// -----------------------------------
// Scene -----------------------------

class DDUI_Scene
{
public:
	DDUI_Scene(const char* name);
	~DDUI_Scene();

	const char* GetName();
	void AddElement(DDUI_Element element);

private:

public:
	bool loaded = false;

private:
	string name;
	vector<DDUI_Element> elements;
};

// ---------------------------
// ----------------------------- Scene

// -----------------------------------
// Element ---------------------------

class DDUI_Element
{
public:
	DDUI_Element(const char* name, UI_Element* element);
	~DDUI_Element();

	const char* GetName();
	UI_Element* GetElement();
private:

public:

private:
	string name;
	UI_Element* element = nullptr;
};

#endif // __j1AUDIO_H__