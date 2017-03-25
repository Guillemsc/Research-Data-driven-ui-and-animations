#include "DataDrivenUI.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Gui.h"
#include "Scene.h"
#include "j1Scene.h"
#include "Functions.h"

#define XML_NAME "ui.xml"

DataDrivenUI::DataDrivenUI()
{
}

DataDrivenUI::~DataDrivenUI()
{
}

bool DataDrivenUI::Start()
{
	bool ret = true;

	LoadUI();

	return ret;
}

bool DataDrivenUI::Update(float dt)
{
	bool ret = true;



	return ret;
}

bool DataDrivenUI::CleanUp()
{
	bool ret = true;



	return ret;
}

void DataDrivenUI::LoadUI()
{
	// LoadXML
	App->LoadXML(XML_NAME, xml);

	// Create UI
	LoadScene(App->scene->GetCurrentScene()->GetName());
}

void DataDrivenUI::LoadScene(char* name)
{
	// Create UI

	// Get Scenes
	for (pugi::xml_node scene_node = xml.child("file").child("scene"); scene_node; scene_node = scene_node.next_sibling("scene"))
	{
		string scene_name = scene_node.attribute("name").as_string();

		if (!TextCmp(scene_name.c_str(), name))
			continue;

		DDUI_Scene* scene = new DDUI_Scene(scene_name.c_str());

		// Get Windows
		for (pugi::xml_node window_node = scene_node.child("Window"); window_node; window_node = window_node.next_sibling("Window"))
		{
			iPoint pos = { window_node.attribute("position_x").as_int(0), window_node.attribute("position_y").as_int(0) };
			string name = window_node.attribute("name").as_string("");
			int size_w = window_node.attribute("size_w").as_int(0);
			int size_h = window_node.attribute("size_h").as_int(0);
			int blit = window_node.attribute("blit_layer").as_int(0);
			bool dinamic = window_node.attribute("dinamic").as_bool(false);
			bool is_ui = window_node.attribute("is_ui").as_bool(true);

			UI_Window* curr_win = App->gui->UI_CreateWin(pos, size_w, size_h, blit, false, dinamic, is_ui);
			scene->AddElement(DDUI_Element(name.c_str(), curr_win));

			// Get Window elements
			for (pugi::xml_node element_node = window_node.child("Element"); element_node; element_node = element_node.next_sibling("Element"))
			{
				// Button
				if (TextCmp(element_node.attribute("type").as_string(""), "Button"))
				{
					iPoint pos = { element_node.attribute("position_x").as_int(0), element_node.attribute("position_y").as_int(0) };
					string name = element_node.attribute("name").as_string("");
					int size_w = element_node.attribute("size_w").as_int(0);
					int size_h = element_node.attribute("size_h").as_int(0);
					bool dinamic = element_node.attribute("dinamic").as_bool(false);

					UI_Button* button = curr_win->CreateButton(pos, size_w, size_h, dinamic);
					scene->AddElement(DDUI_Element(name.c_str(), button));
				}
				else if (TextCmp(element_node.attribute("type").as_string(""), "Text"))
				{
					iPoint pos = { element_node.attribute("position_x").as_int(0), element_node.attribute("position_y").as_int(0) };
					string name = element_node.attribute("name").as_string("");
					int font = element_node.attribute("font").as_int(0);
					int spacing = element_node.attribute("spacing").as_int(0);
					int r = element_node.attribute("color_r").as_int(0);
					int g = element_node.attribute("color_g").as_int(0);
					int b = element_node.attribute("color_r").as_int(0);
					bool dinamic = element_node.attribute("dinamic").as_bool(false);
					string t = element_node.child("Text").attribute("text").as_string("");
					
					UI_Text* text = curr_win->CreateText(pos, App->font->GetFont(font), spacing, dinamic, r, g, b);
					text->SetText(t.c_str());

				}
			}
		}


		scenes.push_back(scene);
	}
}

DDUI_Scene::DDUI_Scene(const char * _name)
{
	name = _name;
}

DDUI_Scene::~DDUI_Scene()
{
}

const char * DDUI_Scene::GetName()
{
	return name.c_str();
}

void DDUI_Scene::AddElement(DDUI_Element element)
{
}

DDUI_Element::DDUI_Element(const char * _name, UI_Element* _element)
{
	name = _name; element = _element;
}

DDUI_Element::~DDUI_Element()
{
}

const char * DDUI_Element::GetName()
{
	return name.c_str();
}

UI_Element * DDUI_Element::GetElement()
{
	return element;
}
