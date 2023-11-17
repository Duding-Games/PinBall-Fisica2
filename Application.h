#pragma once

#include "p2List.h"
#include "Globals.h"

class Module;
class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFonts;
class ModulePlayer;
class ModuleFadeToBlack;
class ModuleMenu;
class ModuleSceneIntro;
class ModuleGameOver;
class ModulePhysics;

class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleFonts* fonts;
	ModulePlayer* player;
	ModuleFadeToBlack* fadeToBlack;
	ModuleMenu* scene_menu;
	ModuleSceneIntro* scene_intro;
	ModuleGameOver* scene_gameOver;
	ModulePhysics* physics;

private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
};