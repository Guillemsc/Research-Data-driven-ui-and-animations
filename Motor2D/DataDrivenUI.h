#ifndef __j1UDIO_H__
#define __j1UDIO_H__

#include "j1Module.h"
#include "j1Render.h"

class DDUI_Scene;
class DDUI_Element;
class UI_Element;

class DataDrivenUI : public j1Module
{
public:

	DataDrivenUI();

	virtual ~DataDrivenUI();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

private:
	void LoadUI();
	void LoadScene(char* name);

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