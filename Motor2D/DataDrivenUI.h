#ifndef __j1UDIO_H__
#define __j1UDIO_H__

#include "j1Module.h"
#include "j1Render.h"

class Scene;
class DDUI_Scene;
class DDUI_Element;
class DDUI_Variable;
class UI_Element;
class UI_Window;
class UI_Button;
class UI_Text;
class UI_Image;
class UI_Text_Input;
class UI_Scroll_Bar;
class UI_Check_Box;

class DataDrivenUI : public j1Module
{
public:

	DataDrivenUI();

	virtual ~DataDrivenUI();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	// Returns an UI_Element given the name set on the XML
	UI_Element* GetElementByName(const char* name);
	DDUI_Variable* GetVariableByName(const char* name);

private:
	// Loads an scene given the scene name
	void LoadScene(char* name);
	DDUI_Scene* GetScene(const char* name);

	// Unloads an scene given the scene name
	void UnloadScene(const char* name);

	void PerformActions();

	// Given a node, adds the UI_Elements to the window
	void CheckForElements(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* curr_win, vector<UI_Element*>& added_elements);

	void CheckForActions(pugi::xml_node action_node, DDUI_Scene* scene);

	// Given a node, adds properties to an UI_Element
	void CheckForGeneralVars(pugi::xml_node element_node, UI_Element* element);
	void AddVars(pugi::xml_node element_node, DDUI_Scene* scene);

	UI_Window* AddWindow(pugi::xml_node element_node, DDUI_Scene* scene);
	UI_Button* AddButton(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* window);
	UI_Text* AddText(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* window);
	UI_Image* AddImage(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* window);
	UI_Text_Input* AddTextInput(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* window);
	UI_Scroll_Bar* AddScrollBar(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* window);
	UI_ColoredRect* AddColoredRect(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* window);
	UI_Check_Box* AddCheckBox(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* window);

public:

private:
	vector<DDUI_Scene*> scenes;
	pugi::xml_document  xml;
	Scene*				current_scene = nullptr;

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
	void AddElement(DDUI_Element* element);
	void AddVariable(DDUI_Variable* var);
	DDUI_Element* FindElement(const char* name);
	DDUI_Variable* FindVariable(const char* name);
	void DeleteElements();

private:

public:
	bool loaded = false;

private:
	string name;
	vector<DDUI_Element*>  elements;
	vector<DDUI_Variable*> variables;
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

// ---------------------------
// --------------------------- Element

// -----------------------------------
// Variable --------------------------

class DDUI_Variable
{
public:
	DDUI_Variable(const char* name, float value);
	~DDUI_Variable();

	const char* GetName();
	float GetValue();
	void SetValue(float value);
	void AddToValue(float addition);
	void SubstractToValue(float substraction);
	void SetMaxValue(float value);
	void SetMinValue(float value);

private:
	string name;
	float value = 0.0f;

	bool  has_max_value = false;
	bool  has_min_value = false;
	float max_value = 0.0f;
	float min_value = 0.0f;
};

#endif // __j1AUDIO_H__