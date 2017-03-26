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

	// LoadXML
	App->LoadXML(XML_NAME, xml);

	LoadScene(App->scene->GetCurrentScene()->GetName());

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

UI_Element * DataDrivenUI::GetElementByName(char * name)
{
	for (int i = 0; i < scenes.size(); i++)
	{

		scenes.at(i)->
	}
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
			UI_Window* curr_win = AddWindow(window_node, scene);

			// Get Window elements
			vector<UI_Element*> elements;
			CheckForElements(window_node, scene, curr_win, elements);
		
		}

		scenes.push_back(scene);
	}
}

void DataDrivenUI::CheckForElements(pugi::xml_node window_node, DDUI_Scene * scene, UI_Window * curr_win, vector<UI_Element*>& added_elements)
{
	for (pugi::xml_node element_node = window_node.child("Element"); element_node; element_node = element_node.next_sibling("Element"))
	{
		if (TextCmp(element_node.attribute("type").as_string(""), "Button"))
		{
			added_elements.push_back(AddButton(element_node, scene, curr_win));
		}
		else if (TextCmp(element_node.attribute("type").as_string(""), "Text"))
		{
			added_elements.push_back(AddText(element_node, scene, curr_win));
		}
		else if (TextCmp(element_node.attribute("type").as_string(""), "Image"))
		{
			added_elements.push_back(AddImage(element_node, scene, curr_win));
		}
		else if (TextCmp(element_node.attribute("type").as_string(""), "TextInput"))
		{
			added_elements.push_back(AddTextInput(element_node, scene, curr_win));
		}
		else if (TextCmp(element_node.attribute("type").as_string(""), "ScrollBar"))
		{
			added_elements.push_back(AddScrollBar(element_node, scene, curr_win));
		}
		else if (TextCmp(element_node.attribute("type").as_string(""), "ColoredRect"))
		{
			added_elements.push_back(AddColoredRect(element_node, scene, curr_win));
		}
	}
}

void DataDrivenUI::CheckForGeneralVars(pugi::xml_node element_node, UI_Element * element)
{
	for (pugi::xml_node var_node = element_node.child("var"); var_node; var_node = var_node.next_sibling("var"))
	{
		if (TextCmp(var_node.attribute("click_through").name(), "click_through"))
		{
			element->click_through = var_node.attribute("click_throug").as_bool(false);
		}
		else if (TextCmp(var_node.attribute("blit_layer").name(), "blit_layer"))
		{
			element->blit_layer = var_node.attribute("blit_layer").as_int(0);
		}
		else if (TextCmp(var_node.attribute("enabled").name(), "enabled"))
		{
			element->enabled = var_node.attribute("enabled").as_bool(true);
		}
		else if (TextCmp(var_node.attribute("is_ui").name(), "is_ui"))
		{
			element->is_ui = var_node.attribute("is_ui").as_bool(true);
		}
	}
}

UI_Window* DataDrivenUI::AddWindow(pugi::xml_node window_node, DDUI_Scene* scene)
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

	return curr_win;
}

UI_Button * DataDrivenUI::AddButton(pugi::xml_node element_node, DDUI_Scene * scene, UI_Window * window)
{
	iPoint pos = { element_node.attribute("position_x").as_int(0), element_node.attribute("position_y").as_int(0) };
	string name = element_node.attribute("name").as_string("");
	int size_w = element_node.attribute("size_w").as_int(0);
	int size_h = element_node.attribute("size_h").as_int(0);
	bool dinamic = element_node.attribute("dinamic").as_bool(false);

	UI_Button* button = window->CreateButton(pos, size_w, size_h, dinamic);
	scene->AddElement(DDUI_Element(name.c_str(), button));

	CheckForGeneralVars(element_node, button);

	for (pugi::xml_node var_node = element_node.child("var"); var_node; var_node = var_node.next_sibling("var"))
	{
		if (TextCmp(var_node.attribute("add_image").name(), "add_image"))
		{
			string name = var_node.attribute("add_image").as_string();
			SDL_Rect rect;
			rect.x = var_node.attribute("rect_x").as_int();
			rect.y = var_node.attribute("rect_y").as_int();
			rect.w = var_node.attribute("rect_w").as_int();
			rect.h = var_node.attribute("rect_h").as_int();
			button->AddImage(name.c_str(), rect);
		}
	}

	return button;
}

UI_Text * DataDrivenUI::AddText(pugi::xml_node element_node, DDUI_Scene * scene, UI_Window * curr_win)
{
	iPoint pos = { element_node.attribute("position_x").as_int(0), element_node.attribute("position_y").as_int(0) };
	string name = element_node.attribute("name").as_string("");
	int font = element_node.attribute("font").as_int(0);
	int spacing = element_node.attribute("spacing").as_int(0);
	int r = element_node.attribute("color_r").as_int(0);
	int g = element_node.attribute("color_g").as_int(0);
	int b = element_node.attribute("color_r").as_int(0);
	bool dinamic = element_node.attribute("dinamic").as_bool(false);

	UI_Text* text = curr_win->CreateText(pos, App->font->GetFont(font), spacing, dinamic, r, g, b);
	scene->AddElement(DDUI_Element(name.c_str(), text));

	CheckForGeneralVars(element_node, text);

	for (pugi::xml_node var_node = element_node.child("var"); var_node; var_node = var_node.next_sibling("var"))
	{
		if (TextCmp(var_node.attribute("text").name(), "text"))
		{
			text->SetText(var_node.attribute("text").as_string());
		}
	}

	return text;
}

UI_Image * DataDrivenUI::AddImage(pugi::xml_node element_node, DDUI_Scene * scene, UI_Window * curr_win)
{
	iPoint pos = { element_node.attribute("position_x").as_int(0), element_node.attribute("position_y").as_int(0) };
	string name = element_node.attribute("name").as_string("");
	SDL_Rect rect;
	rect.x = element_node.attribute("rect_x").as_int(0);
	rect.y = element_node.attribute("rect_y").as_int(0);
	rect.w = element_node.attribute("rect_w").as_int(0);
	rect.h = element_node.attribute("rect_h").as_int(0);
	bool dinamic = element_node.attribute("dinamic").as_bool(false);

	UI_Image* image = curr_win->CreateImage(pos, rect, dinamic);
	scene->AddElement(DDUI_Element(name.c_str(), image));

	CheckForGeneralVars(element_node, image);

	for (pugi::xml_node var_node = element_node.child("var"); var_node; var_node = var_node.next_sibling("var"))
	{

	}

	return image;
}

UI_Text_Input* DataDrivenUI::AddTextInput(pugi::xml_node element_node, DDUI_Scene * scene, UI_Window * curr_win)
{
	iPoint pos = { element_node.attribute("position_x").as_int(0), element_node.attribute("position_y").as_int(0) };
	string name = element_node.attribute("name").as_string("");
	int size_w = element_node.attribute("size_w").as_int(0);
	int font = element_node.attribute("font").as_int(0);
	int r = element_node.attribute("color_r").as_int(0);
	int g = element_node.attribute("color_g").as_int(0);
	int b = element_node.attribute("color_r").as_int(0);
	bool dinamic = element_node.attribute("dinamic").as_bool(false);

	UI_Text_Input* text_input = curr_win->CreateTextInput(pos, size_w, App->font->GetFont(font), dinamic, r, g, b);
	scene->AddElement(DDUI_Element(name.c_str(), text_input));

	CheckForGeneralVars(element_node, text_input);

	for (pugi::xml_node var_node = element_node.child("var"); var_node; var_node = var_node.next_sibling("var"))
	{

	}

	return text_input;
}

UI_Scroll_Bar * DataDrivenUI::AddScrollBar(pugi::xml_node element_node, DDUI_Scene * scene, UI_Window * curr_win)
{
	iPoint pos = { element_node.attribute("position_x").as_int(0), element_node.attribute("position_y").as_int(0) };
	string name = element_node.attribute("name").as_string("");
	int size_w = element_node.attribute("size_w").as_int(0);
	int size_h = element_node.attribute("size_h").as_int(0);
	int button_size = element_node.attribute("button_size").as_int(0);
	bool dinamic = element_node.attribute("dinamic").as_bool(false);

	UI_Scroll_Bar* scroll_bar = curr_win->CreateScrollBar(pos, size_w, size_h, button_size, dinamic);
	scene->AddElement(DDUI_Element(name.c_str(), scroll_bar));

	vector<UI_Element*> elements;
	CheckForElements(element_node, scene, curr_win, elements);

	CheckForGeneralVars(element_node, scroll_bar);

	for (int i = 0; i < elements.size(); i++)
	{
		scroll_bar->AddElement(elements.at(i));
		App->gui->EraseFromElementsList(elements.at(i));
	}

	for (pugi::xml_node var_node = element_node.child("var"); var_node; var_node = var_node.next_sibling("var"))
	{

	}

	return scroll_bar;
}

UI_ColoredRect * DataDrivenUI::AddColoredRect(pugi::xml_node element_node, DDUI_Scene * scene, UI_Window * curr_window)
{
	iPoint pos = { element_node.attribute("position_x").as_int(0), element_node.attribute("position_y").as_int(0) };
	string name = element_node.attribute("name").as_string("");
	int size_w = element_node.attribute("size_w").as_int(0);
	int size_h = element_node.attribute("size_h").as_int(0);
	int r = element_node.attribute("color_r").as_int(0);
	int g = element_node.attribute("color_g").as_int(0);
	int b = element_node.attribute("color_r").as_int(0);
	bool filled = element_node.attribute("filled").as_bool(false);
	bool dinamic = element_node.attribute("dinamic").as_bool(false);

	UI_ColoredRect* colored_rect = curr_window->CreateColoredRect(pos, size_w, size_h, { (Uint8)r, (Uint8)g, (Uint8)b, 255 }, filled, dinamic);
	scene->AddElement(DDUI_Element(name.c_str(), colored_rect));

	CheckForGeneralVars(element_node, colored_rect);

	for (pugi::xml_node var_node = element_node.child("var"); var_node; var_node = var_node.next_sibling("var"))
	{

	}

	return colored_rect;
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
