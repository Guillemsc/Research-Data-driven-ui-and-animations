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

	// Save starting scene
	current_scene = App->scene->GetCurrentScene();

	// Load UI for starting scene
	LoadScene(App->scene->GetCurrentScene()->GetName());

	return ret;
}

bool DataDrivenUI::Update(float dt)
{
	bool ret = true;

	// Perform code created by XML
	PerformActions();

	// Change UI when changing scene
	if (current_scene != App->scene->GetCurrentScene())
	{
		UnloadScene(current_scene->GetName());
		LoadScene(App->scene->GetCurrentScene()->GetName());
		current_scene = App->scene->GetCurrentScene();
	}

	return ret;
}

bool DataDrivenUI::CleanUp()
{
	bool ret = true;

	// Delete all scenes
	for(vector<DDUI_Scene*>::iterator it = scenes.begin(); it != scenes.end();)
	{
		(*it)->DeleteElements();
		RELEASE(*it);
		it = scenes.erase(it);
	}

	scenes.clear();

	return ret;
}

UI_Element* DataDrivenUI::GetElementByName(const char * name)
{
	UI_Element* ret = nullptr;

	for (int i = 0; i < scenes.size(); i++)
	{
		DDUI_Element* curr = scenes.at(i)->FindElement(name);

		if (curr != nullptr)
		{
			ret = curr->GetElement();
			break;
		}
	}

	return ret;
}

DDUI_Variable * DataDrivenUI::GetVariableByName(const char * name)
{
	DDUI_Variable* ret = nullptr;

	for (int i = 0; i < scenes.size(); i++)
	{
		DDUI_Variable* curr = scenes.at(i)->FindVariable(name);

		if (curr != nullptr)
		{
			ret = curr;
			break;
		}
	}

	return ret;
}

DDUI_Animation * DataDrivenUI::GetAnimationByName(const char * name)
{
	DDUI_Animation* ret = nullptr;

	for (int i = 0; i < scenes.size(); i++)
	{
		DDUI_Animation* curr = scenes.at(i)->FindAnimation(name);

		if (curr != nullptr)
		{
			ret = curr;
			break;
		}
	}

	return ret;
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

DDUI_Scene * DataDrivenUI::GetScene(const char * name)
{
	DDUI_Scene* ret = nullptr;

	for (int i = 0; i < scenes.size(); i++)
	{
		if (TextCmp(scenes.at(i)->GetName(), name))
		{
			ret = scenes.at(i);
			break;
		}
	}

	return ret;
}

void DataDrivenUI::UnloadScene(const char * name)
{
	// Get Scenes
	if (!scenes.empty())
	{
		for (vector<DDUI_Scene*>::iterator it = scenes.begin(); it != scenes.end();)
		{
			if (TextCmp((*it)->GetName(), name))
			{
				(*it)->DeleteElements();
				RELEASE(*it);
				it = scenes.erase(it);
				break;
			}
			else
				++it;
		}
	}

}

void DataDrivenUI::PerformActions()
{
	// Get Scenes
	for (pugi::xml_node scene_node = xml.child("file").child("scene"); scene_node; scene_node = scene_node.next_sibling("scene"))
	{
		string scene_name = scene_node.attribute("name").as_string();

		if (!TextCmp(scene_name.c_str(), current_scene->GetName()))
			continue;

		DDUI_Scene* scene = GetScene(scene_name.c_str());

		for (pugi::xml_node action_node = scene_node.child("Action"); action_node; action_node = action_node.next_sibling("Action"))
		{
			CheckForActions(action_node, scene);
		}

		scene->UpdateAnimations();
	}
}

void DataDrivenUI::CheckForElements(pugi::xml_node window_node, DDUI_Scene * scene, UI_Window * curr_win, vector<UI_Element*>& added_elements)
{
	for (pugi::xml_node element_node = window_node.child("Element"); element_node; element_node = element_node.next_sibling("Element"))
	{
		// -------------------------------------------------------- //
		// Add here all the possible variables for all the Elements //
		// -------------------------------------------------------- //
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
		else if (TextCmp(element_node.attribute("type").as_string(""), "CheckBox"))
		{
			added_elements.push_back(AddCheckBox(element_node, scene, curr_win));
		}
	}
}

void DataDrivenUI::CheckForActions(pugi::xml_node action_node, DDUI_Scene * scene)
{

	// --------------------------------------------------- //
	// Add here all the possible actions for every Element //
	// --------------------------------------------------- //
	for (pugi::xml_node act_node = action_node.child("act"); act_node; act_node = act_node.next_sibling("act"))
	{
		string name = act_node.attribute("name").as_string("");

		// Find Element
		UI_Element* element = GetElementByName(name.c_str());

		if (element != nullptr && element->type != ui_element::ui_element_null)
		{
			// Button
			if (element->type == ui_element::ui_button)
			{
				UI_Button* button = (UI_Button*)element;

				// Button Actions
				if (TextCmp(act_node.attribute("left_pressed").name(), "left_pressed"))
				{
					bool pressed = act_node.attribute("left_pressed").as_bool(true);

					if (pressed && button->MouseClickEnterLeft())
					{
						CheckForActions(act_node, scene);
					}
					else if (!pressed && button->MouseClickOutLeft())
					{
						CheckForActions(act_node, scene);
					}
				}
			}
			// CheckBox
			else if (element->type == ui_element::ui_check_box)
			{

			}
			// Colored rect
			else if (element->type == ui_element::ui_colored_rect)
			{

			}
			// Image
			else if (element->type == ui_element::ui_image)
			{
	
			}
			// Scroll Bar
			else if (element->type == ui_element::ui_scroll_bar)
			{

			}
			// Text
			else if (element->type == ui_element::ui_text)
			{

			}
			// Text Input
			else if (element->type == ui_element::ui_text_input)
			{

			}
			// Window
			else if (element->type == ui_element::ui_window)
			{
			
			}

			// General Actions for Elements
			if (TextCmp(act_node.attribute("enabled").name(), "enabled"))
			{
				int enabled = act_node.attribute("enabled").as_int(-1);

				if (enabled == 0 || enabled == 1)
				{
					element->SetEnabledAndChilds(enabled);
				}
				else
				{
					element->SetEnabledAndChilds(!element->enabled);
				}
			}
		}

		// ---------------------------------------------------- //
		// Add here all the possible actions for every variable //
		// ---------------------------------------------------- //
		// Find variable
		else
		{
			DDUI_Variable* variable = GetVariableByName(name.c_str());

			if (variable != nullptr)
			{
				// Add to variable
				if (TextCmp(act_node.attribute("add").name(), "add"))
				{
					variable->AddToValue(act_node.attribute("add").as_int());
				}
				if (TextCmp(act_node.attribute("substract").name(), "substract"))
				{
					variable->SubstractToValue(act_node.attribute("substract").as_int());
				}
				// Set to
				if (TextCmp(act_node.attribute("set_to").name(), "set_to"))
				{
					string element_name = act_node.attribute("set_to").as_string("");
					UI_Element* element = GetElementByName(element_name.c_str());

					if (element->type == ui_element::ui_text)
					{
						UI_Text* text = (UI_Text*)element;
						p2SString str("%0.f", variable->GetValue());
						string s = str.GetString();
						text->SetText(s);
					}
				}
			}
		}

		// --------------------------------------------------------------------------- //
		// Add here all the actions that don't deppend from variables neither Elements //
		// --------------------------------------------------------------------------- //
		if (TextCmp(act_node.attribute("load_scene").name(), "load_scene"))
		{
			string scene_name = act_node.attribute("load_scene").as_string("");

			Scene* new_scene = App->scene->GetSceneByName(scene_name.c_str());

			if (new_scene != nullptr)
			{
				App->scene->ChangeScene(new_scene);
			}
		}
		
	}
}

void DataDrivenUI::CheckForGeneralVars(pugi::xml_node element_node, UI_Element * element)
{
	// Vars
	for (pugi::xml_node var_node = element_node.child("var"); var_node; var_node = var_node.next_sibling("var"))
	{
		if (TextCmp(var_node.attribute("click_through").name(), "click_through"))
		{
			element->click_through = var_node.attribute("click_through").as_bool(false);
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

void DataDrivenUI::AddVars(pugi::xml_node element_node, DDUI_Scene * scene)
{
	// New vars
	for (pugi::xml_node new_var_node = element_node.child("new_var"); new_var_node; new_var_node = new_var_node.next_sibling("new_var"))
	{
		int start_value = new_var_node.attribute("start_value").as_int(0);
		string name = new_var_node.attribute("name").as_string("");

		DDUI_Variable* var = new DDUI_Variable(name.c_str(), start_value);

		if (TextCmp(new_var_node.attribute("max_value").name(), "max_value"))
		{
			var->SetMaxValue(new_var_node.attribute("max_value").as_int(0));
		}
		if (TextCmp(new_var_node.attribute("min_value").name(), "min_value"))
		{
			var->SetMinValue(new_var_node.attribute("min_value").as_int(0));
		}

		scene->AddVariable(var);
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
	scene->AddElement(new DDUI_Element(name.c_str(), curr_win));

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
	scene->AddElement(new DDUI_Element(name.c_str(), button));

	CheckForGeneralVars(element_node, button);

	// Personal vars
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

			if (TextCmp(name.c_str(), "idle"))
				button->SetIdle(rect);
			else if (TextCmp(name.c_str(), "pressed"))
				button->SetPressed(rect);
			else if (TextCmp(name.c_str(), "over"))
				button->SetOver(rect);
		}
	}

	AddVars(element_node, scene);

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
	int b = element_node.attribute("color_b").as_int(0);
	bool dinamic = element_node.attribute("dinamic").as_bool(false);

	UI_Text* text = curr_win->CreateText(pos, App->font->GetFont(font), spacing, dinamic, r, g, b);
	scene->AddElement(new DDUI_Element(name.c_str(), text));

	CheckForGeneralVars(element_node, text);

	// Personal vars
	for (pugi::xml_node var_node = element_node.child("var"); var_node; var_node = var_node.next_sibling("var"))
	{
		if (TextCmp(var_node.attribute("text").name(), "text"))
		{
			text->SetText(var_node.attribute("text").as_string());
		}
	}

	AddVars(element_node, scene);

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
	scene->AddElement(new DDUI_Element(name.c_str(), image));

	CheckForGeneralVars(element_node, image);

	// Personal vars
	for (pugi::xml_node var_node = element_node.child("var"); var_node; var_node = var_node.next_sibling("var"))
	{

	}

	AddVars(element_node, scene);

	// Test
	DDUI_A_Movement* m1 = new DDUI_A_Movement("hi", ddui_anim_type::ddui_anim_movement, image, iPoint(300, image->GetPos().y), 5, a_move_type::a_ease_in_ease_out);
	scene->AddAnimation(m1);
	// -----

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
	int b = element_node.attribute("color_b").as_int(0);
	bool dinamic = element_node.attribute("dinamic").as_bool(false);

	UI_Text_Input* text_input = curr_win->CreateTextInput(pos, size_w, App->font->GetFont(font), dinamic, r, g, b);
	scene->AddElement(new DDUI_Element(name.c_str(), text_input));

	CheckForGeneralVars(element_node, text_input);;

	// Personal vars
	for (pugi::xml_node var_node = element_node.child("var"); var_node; var_node = var_node.next_sibling("var"))
	{

	}

	AddVars(element_node, scene);

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
	scene->AddElement(new DDUI_Element(name.c_str(), scroll_bar));

	vector<UI_Element*> elements;
	CheckForElements(element_node, scene, curr_win, elements);

	CheckForGeneralVars(element_node, scroll_bar);

	for (int i = 0; i < elements.size(); i++)
	{
		scroll_bar->AddElement(elements.at(i));
		App->gui->EraseFromElementsList(elements.at(i));
	}

	AddVars(element_node, scene);

	// Personal vars
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
	scene->AddElement(new DDUI_Element(name.c_str(), colored_rect));

	CheckForGeneralVars(element_node, colored_rect);

	// Personal vars
	for (pugi::xml_node var_node = element_node.child("var"); var_node; var_node = var_node.next_sibling("var"))
	{

	}

	AddVars(element_node, scene);

	return colored_rect;
}

UI_Check_Box * DataDrivenUI::AddCheckBox(pugi::xml_node element_node, DDUI_Scene * scene, UI_Window * curr_window)
{
	iPoint pos = { element_node.attribute("position_x").as_int(0), element_node.attribute("position_y").as_int(0) };
	string name = element_node.attribute("name").as_string("");
	int size_w = element_node.attribute("size_w").as_int(0);
	int size_h = element_node.attribute("size_h").as_int(0);
	bool multiple_choices = element_node.attribute("multiple_choices").as_bool(false);
	bool dinamic = element_node.attribute("dinamic").as_bool(false);

	UI_Check_Box* check_box = curr_window->CreateCheckBox(pos, size_w, size_h, NULLRECT, NULLRECT, multiple_choices, dinamic);
	scene->AddElement(new DDUI_Element(name.c_str(), check_box));

	CheckForGeneralVars(element_node, check_box);

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

			if (TextCmp(name.c_str(), "idle"))
				check_box->SetIdle(rect);
			else if (TextCmp(name.c_str(), "pressed"))
				check_box->SetPressed(rect);
		}
		else if (TextCmp(var_node.attribute("add_box").name(), "add_box"))
		{
			string name = var_node.attribute("add_box").as_string();
			iPoint pos = { var_node.attribute("position_x").as_int(0), var_node.attribute("position_y").as_int(0) };
			int size_w = var_node.attribute("size_w").as_int(0);
			int size_h = var_node.attribute("size_h").as_int(0);
			check_box->AddBox(pos, size_w, size_h, name.c_str());
		}
	}

	AddVars(element_node, scene);
	
	return check_box;
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

void DDUI_Scene::AddElement(DDUI_Element* element)
{
	elements.push_back(element);
}

void DDUI_Scene::AddVariable(DDUI_Variable * var)
{
	variables.push_back(var);
}

void DDUI_Scene::AddAnimation(DDUI_Animation * anim)
{
	animations.push_back(anim);
}

void DDUI_Scene::UpdateAnimations()
{
	for (int i = 0; i < animations.size(); i++)
	{
		animations.at(i)->update();
	}
}

DDUI_Element* DDUI_Scene::FindElement(const char * name)
{
	DDUI_Element* ret = nullptr;

	for (int i = 0; i < elements.size(); i++)
	{
		if (TextCmp(elements.at(i)->GetName(), name))
		{
			ret = elements.at(i);
			break;
		}
	}

	return ret;
}

DDUI_Variable * DDUI_Scene::FindVariable(const char * name)
{
	DDUI_Variable* ret = nullptr;

	for (int i = 0; i < variables.size(); i++)
	{
		if (TextCmp(variables.at(i)->GetName(), name))
		{
			ret = variables.at(i);
			break;
		}
	}

	return ret;
}

DDUI_Animation * DDUI_Scene::FindAnimation(const char * name)
{
	DDUI_Animation* ret = nullptr;

	for (int i = 0; i < animations.size(); i++)
	{
		if (TextCmp(animations.at(i)->GetName(), name))
		{
			ret = animations.at(i);
			break;
		}
	}

	return ret;
}

void DDUI_Scene::DeleteElements()
{
	if (!elements.empty())
	{
		for (vector<DDUI_Element*>::iterator it = elements.begin(); it != elements.end();)
		{
			if ((*it)->GetElement()->type == ui_element::ui_window)
				App->gui->DeleteElement((*it)->GetElement());

			RELEASE(*it);
			it = elements.erase(it);
		}
	}

	if (!variables.empty())
	{
		for (vector<DDUI_Variable*>::iterator it = variables.begin(); it != variables.end();)
		{
			RELEASE(*it);
			it = variables.erase(it);
		}
	}

	if (!animations.empty())
	{
		for (vector<DDUI_Animation*>::iterator it = animations.begin(); it != animations.end();)
		{
			RELEASE(*it);
			it = animations.erase(it);
		}
	}

	elements.clear();
	variables.clear();
	animations.clear();
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

DDUI_Variable::DDUI_Variable(const char * _name, float _value)
{
	name = _name;
	value = _value;
}

DDUI_Variable::~DDUI_Variable()
{
}

const char * DDUI_Variable::GetName()
{
	return name.c_str();
}

float DDUI_Variable::GetValue()
{
	return value;
}

void DDUI_Variable::SetValue(float _value)
{
	value = _value;

	if (has_max_value && value > max_value)
		value = max_value;
	if (has_min_value && value < min_value)
		value = min_value;
}

void DDUI_Variable::AddToValue(float addition)
{
	value += addition;

	if (has_max_value && value > max_value)
		value = max_value;
	if (has_min_value && value < min_value)
		value = min_value;
}

void DDUI_Variable::SubstractToValue(float substraction)
{
	value -= substraction;

	if (has_max_value && value > max_value)
		value = max_value;
	if (has_min_value && value < min_value)
		value = min_value;
}

void DDUI_Variable::SetMaxValue(float value)
{
	max_value = value;
	has_max_value = true;
}

void DDUI_Variable::SetMinValue(float value)
{
	min_value = value;
	has_min_value = true;
}

const char * DDUI_Animation::GetName()
{
	return name.c_str();
}

UI_Element * DDUI_Animation::GetElement()
{
	return element;
}

ddui_anim_type DDUI_Animation::GetType()
{
	return type;
}

bool DDUI_A_Movement::update()
{
	if (stop)
		return true;

	if (first_time)
	{
		timer.Start();
		first_time = false;
		test_pos = starting_pos;
	}

	iPoint destance_between_ends;
	destance_between_ends.x = starting_pos.x - destination.x;
	destance_between_ends.y = starting_pos.y - destination.y;

	App->render->DrawCircle(destination.x, destination.y, 5, 255, 255, 255);
	App->render->DrawCircle(GetElement()->GetPos().x, GetElement()->GetPos().y, 5, 255, 255, 255);
	App->render->DrawCircle(starting_pos.x, starting_pos.y, 5, 255, 255, 255);
	App->render->DrawLine(starting_pos.x, starting_pos.y, destination.x, destination.y, 255, 255, 255);

	vector<fPoint> points;

	points.push_back(fPoint(0, 0));
	switch (movement_type)
	{
	case a_move_linear:
		break;
	case a_ease_in_ease_out:
		points.push_back(fPoint(1.00f, 0.0f));
		points.push_back(fPoint(0.00f, 1.00f));
		break;
	}
	points.push_back(fPoint(1, 1));

	fPoint bezier = Bezier(timer.ReadSec(), time, points);
	
	float m = bezier.y;

	fPoint new_pos;
	new_pos.x = starting_pos.x - m*destance_between_ends.x;
	new_pos.y = starting_pos.y + m*destance_between_ends.y;

	GetElement()->fSetPos(new_pos);

	if (timer.ReadSec() > time)
		stop = true;

	if (App->debug_mode)
	{
		for (float i = 0; i < 1; i += 0.01f)
		{
			//float b = Bezier(i, 1, points).y;
			test_pos = iPoint(starting_pos.x + (Bezier(i, 1, points).x * 100), starting_pos.y - (Bezier(i, 1, points).y * 100));
			App->render->DrawCircle(test_pos.x, test_pos.y, 1, 255, 255, 255);
		}
		test_pos = starting_pos;
		test_pos = iPoint(starting_pos.x + (Bezier(timer.ReadSec(), time, points).x * 100), starting_pos.y - (Bezier(timer.ReadSec(), time, points).y * 100));
		App->render->DrawCircle(test_pos.x, test_pos.y, 2, 0, 255, 255);
		App->render->DrawCircle(test_pos.x, starting_pos.y, 2, 0, 255, 255);
		App->render->DrawCircle(starting_pos.x, test_pos.y, 2, 0, 255, 255);
		App->render->DrawQuad({ starting_pos.x , starting_pos.y, 100, -100 }, 255, 255, 255, -1, 255, false);
		test_pos = starting_pos;
	}

	return true;
}

float DDUI_A_Movement::GetTime()
{
	return time;
}

iPoint DDUI_A_Movement::GetDestination()
{
	return destination;
}

float DDUI_A_Movement::BezierXAsTime(float curr_time, float end_time, vector<fPoint> points)
{
	return 0.0f;
}
