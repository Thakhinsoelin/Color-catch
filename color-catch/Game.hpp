#pragma once
#define RINI_IMPLEMENTATION
#define PHYSAC_IMPLEMENTATION
#pragma warning(disable: 4244)
#include "rini.h"

#include "raylib.h"
#include "raymath.h"
#include "physac.h"
#include "basketcp.h"

#include "imgui.h"
#include "rlImGui.h"

#include <iostream> 
#include <vector>
#include <random>
#include <chrono>

class Game {
public:
	Game(int width, int height, const char* TITLE);
	~Game();
	void Init();
	void Start();
	void ShutDown();
private:
};