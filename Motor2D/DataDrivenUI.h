#ifndef __j1UDIO_H__
#define __j1UDIO_H__

#include "j1Module.h"
#include "j1Render.h"

class Scene;
class DDUI_Scene;
class DDUI_Element;
class DDUI_Variable;
class DDUI_Animation;
class DDUI_A_Movement;
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

	// Returns a DDUI_Variable given the name set on the XML
	DDUI_Variable* GetVariableByName(const char* name);

	// Returns a DDUI_Animation given the name set on the XML
	DDUI_Animation* GetAnimationByName(const char* name);

private:
	// Loads an scene given the scene name
	void LoadScene(char* name);
	DDUI_Scene* GetScene(const char* name);

	// Unloads an scene given the scene name
	void UnloadScene(const char* name);

	// Do actions on the current scene
	void PerformActions();

	// Given a node, finds and adds the UI_Elements to the window
	void CheckForElements(pugi::xml_node element_node, DDUI_Scene* scene, UI_Window* curr_win, vector<UI_Element*>& added_elements);

	// Given a node, finds and adds all the actions to the scene
	void CheckForActions(pugi::xml_node action_node, DDUI_Scene* scene);

	// Given a node, finds and adds properties to an UI_Element
	void CheckForGeneralVars(pugi::xml_node element_node, UI_Element* element);

	// Given a node, finds and creates a DDUI_variable to the scene
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

	// Adds an element to the scene
	void AddElement(DDUI_Element* element);

	// Adds a variable to the scene
	void AddVariable(DDUI_Variable* var);

	// Adds an anition to the scene
	void AddAnimation(DDUI_Animation* anim);

	// Finds an element stored on the scene given a name set on the XML
	DDUI_Element* FindElement(const char* name);

	// Finds a variable stored on the scene given a name set on the XML
	DDUI_Variable* FindVariable(const char* name);

	// Finds an animation stored on the scene given a name set on the XML
	DDUI_Animation* FindAnimation(const char* name);

	// Cleans scene to posterior release
	void DeleteElements();

private:

public:

private:
	string                  name;
	vector<DDUI_Element*>   elements;
	vector<DDUI_Variable*>  variables;
	vector<DDUI_Animation*> animations;
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
	string      name;
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

public:

private:
	string name;
	float  value = 0.0f;

	bool   has_max_value = false;
	bool   has_min_value = false;
	float  max_value = 0.0f;
	float  min_value = 0.0f;
};

// ---------------------------
// -------------------------- Variable

// -----------------------------------
// Animation -------------------------

enum ddui_anim_type
{
	ddui_anim_null,
	ddui_anim_movement,
};

class DDUI_Animation
{
public:
	DDUI_Animation(const char* name, ddui_anim_type type, UI_Element* target = nullptr) : type(type), name(name), target(target) {};
	virtual ~DDUI_Animation() {};
	virtual bool update() { return true; }

	const char* GetName();
	UI_Element* GetTarget();
	ddui_anim_type GetType();
private:

public:

private:
	string         name;
	UI_Element*    target = nullptr;
	ddui_anim_type type = ddui_anim_null;
};

// --------------
// --------------

enum a_move_type
{
	a_move_null,
	a_move_linear,
	a_move_test,
};

class DDUI_A_Movement : public DDUI_Animation
{
public:
	DDUI_A_Movement(const char* name, ddui_anim_type type, UI_Element* target, iPoint destination, float speed, a_move_type movement_type) 
		: destination(destination), speed(speed), movement_type(movement_type), DDUI_Animation(name, type, target) {};

	virtual ~DDUI_A_Movement() {};
	bool update();

	float GetSpeed();
	iPoint GetTarget();
private:
	float       speed = 0.0f;
	iPoint      destination = NULLPOINT;
	a_move_type movement_type = a_move_null;
};

// ---------------------------
// ------------------------- Animation

#endif // __j1AUDIO_H__