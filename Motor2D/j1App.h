#ifndef __j1APP_H__
#define __j1APP_H__

#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"

class UI_Window;
class UI_Text;
class UI_ColoredRect;
class UI_Button;

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1FileSystem;
class j1Scene;
class j1Fonts;
class j1Gui;
class j1Console;
class j1Viewports;
class DataDrivenUI;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;
	float GetDT() const;

	void LoadGame(const char* file);
	void SaveGame(const char* file) const;

	// Load an XML file
	void LoadXML(const char* path, pugi::xml_document& doc);
	void UnloadXML(pugi::xml_document& doc);

	void CapFps(float fps);
	void EndSDL();

	void OpenWebPage(char* url);

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

	// Frame rate calculations uptade
	void FrameRateCalculations();

public:

	// Modules
	j1Window*			win = NULL;
	j1Input*			input = NULL;
	j1Render*			render = NULL;
	j1Textures*			tex = NULL;
	j1Audio*			audio = NULL;
	j1Scene*			scene = NULL;
	j1FileSystem*		fs = NULL;
	j1Fonts*			font = NULL;
	j1Gui*				gui = NULL;
	j1Console*			console = NULL;
	j1Viewports*        view = NULL;
	DataDrivenUI*		data_ui = NULL;

	// Console logs
	list<string>        logs;

	bool			    debug_mode = false;

private:

	bool                end_program = false;

	list<j1Module*>  	modules;
	int					argc;
	char**				args;

	string		    	title;
	string		     	organization;

	mutable bool		want_to_save = false;
	bool				want_to_load = false;
	p2SString			load_game;
	mutable p2SString	save_game;

	int					capped_ms = -1;
	j1PerfTimer			ptimer;
	uint64				frame_count = 0;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	float				dt = 0.0f;

	// Debug ui
	UI_Window*			debug_window = nullptr;
	UI_ColoredRect*		debug_colored_rect = nullptr;
	UI_Text*			debug_text = nullptr;
	UI_Button*			bug_report_button = nullptr;
	UI_ColoredRect*		bug_report_button_color = nullptr;
	UI_Text*			bug_report_text = nullptr;
};

extern j1App* App;

#endif