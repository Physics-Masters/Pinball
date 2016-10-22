#pragma once

#include "p2List.h"
#include "Globals.h"

class Module;
class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleLevel;
class ModuleInput;
class ModuleAudio;
class ModulePlayer;
class ModuleSceneIntro;
class ModulePhysics;
class ModuleSensors;


class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleLevel* level;
	ModuleInput* input;
	ModuleAudio* audio;
	ModulePlayer* player;
	ModuleSceneIntro* scene_intro;
	ModulePhysics* physics;
	ModuleSensors* sensors;

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