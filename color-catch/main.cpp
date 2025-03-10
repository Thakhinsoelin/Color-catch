/*******************************************************************************************
*
* raylib-extras [ImGui] example - Simple Integration
*
* This is a simple ImGui Integration
* It is done using C++ but with C style code
* It can be done in C as well if you use the C ImGui wrapper
* https://github.com/cimgui/cimgui
*
* Copyright (c) 2021 Jeffery Myers
*
********************************************************************************************/
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

#include "RandomShapeGenerator.hpp"

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    
    int screenWidth = 1280;
    int screenHeight = 800;

    int scoreCounter = 0;
    int fontSize = 24;


    

    char scoreText[255];
    sprintf(scoreText, "SCORE: %d\n", scoreCounter);
    //Vector2 textsize = MeasureTextEx(GetFontDefault()., scoreText, 30, spacing);
    //printf("ScoreText: %s Text Width: %d\n", scoreText, textsize.x);
    int y = 10;
	

#ifdef DEBUG
    rini_config config = rini_load_config("Color Catch Config.ini");

    float basketScale = rini_get_config_valuef(config, "Basket_Scale");
    int basketVelocity = rini_get_config_value(config, "Basket_Velocity");
#else

	float basketScale = 0.075;
	int basketVelocity = 756;
#endif
    /*float basketScale = 0.075;
    int basketVelocity = 10;*/
    int basketx = 0;


    std::vector<Shape> fallingShapes;
    float shapeSpawnTimer = 0.0f;
    float shapeSpawnInterval = 1.0f; // Spawn a shape every 1 second

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raylib-Extras [ImGui] example - simple ImGui Demo");

    SetTargetFPS(60);
#ifdef NDEBUG
	SetTraceLogLevel(LOG_NONE);
#endif

#ifdef DEBUG
    rlImGuiSetup(true);
#endif // DEBUG


    // Enable ImGui Viewports
    Image image = {
        .data = BASKETCP_DATA,
        .width = BASKETCP_WIDTH,
        .height = BASKETCP_HEIGHT,
		.mipmaps = 1,
        .format = BASKETCP_FORMAT,
    };
	Texture basket = LoadTextureFromImage(image);
    Vector2 basketPos = { ((float)screenWidth / 2 - (float)basket.width * basketScale / 2) + basketx, (float)screenHeight - basket.height * basketScale } ;
    Rectangle basketBound = {
		.x = basketPos.x,
        .y = basketPos.y,
		.width = (float)basket.width * basketScale,
		.height = (float)basket.height * basketScale
    };

    // Main game loop
    while (!WindowShouldClose())      // Detect window close button or ESC key
    {
        float deltatime = GetFrameTime();
        //handle keys
		if (IsKeyDown(KEY_LEFT)) {
            basketBound.x -= basketVelocity * deltatime;
		}
        if (IsKeyDown(KEY_RIGHT)) {
            basketBound.x += basketVelocity * deltatime;
        }
		
        if (basketBound.x <= 0) {
            basketBound.x = 0;
        }

        if (basketBound.x + basketBound.width >= screenWidth) {
            basketBound.x = screenWidth - basketBound.width;
        }

        shapeSpawnTimer += deltatime;
        if (shapeSpawnTimer >= shapeSpawnInterval) {
            fallingShapes.push_back(generateShape(GetScreenWidth()));
            shapeSpawnTimer = 0.0f;
        }

        updateShapes(fallingShapes, deltatime);
        
        BeginDrawing();
        ClearBackground(BLACK);

        // start ImGui Conent
#ifdef DEBUG
        rlImGuiBegin();
#endif
        DrawText(scoreText, screenWidth - 130, y, fontSize, GREEN);
        //Draw main things here
        drawShapes(fallingShapes);
        for (auto& e : fallingShapes) {
            if (checkCollision(e, basketBound)) {
                e.isActive = false;
                scoreCounter++;
                sprintf(scoreText, "SCORE: %d\n", scoreCounter);
            }
        }
        DrawTextureEx(basket, {basketBound.x, basketBound.y}, 0, basketScale, WHITE);
        

        // show ImGui Content
#ifdef DEBUG
        bool open = true;

        if (ImGui::Begin("Variable Control Window", &open))
        {
			ImGui::SliderFloat("Basket Scale", &basketScale, 0.0f, 0.5f);
            ImGui::SliderInt("Basket Velocity", &basketVelocity, 1, 1000);
        }
        ImGui::End();

        // end ImGui Content
        rlImGuiEnd();
#endif

        EndDrawing();
        fallingShapes.erase(std::remove_if(fallingShapes.begin(), fallingShapes.end(),
            [](const Shape& obj) { return !obj.isActive; }),
            fallingShapes.end());
#ifdef DEBUG
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
#endif
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------    
#ifdef DEBUG
    rlImGuiShutdown();
#endif

	UnloadTexture(basket);
    CloseWindow();          // Close window and OpenGL context

    

    // Define header comment lines
#ifdef DEBUG
    rini_set_config_comment_line(&config, NULL);   // Empty comment line, but including comment prefix delimiter
    rini_set_config_comment_line(&config, "rTexViewer initialization configuration options");
    rini_set_config_comment_line(&config, NULL);
    rini_set_config_comment_line(&config, "NOTE: This file is loaded at application startup,");
    rini_set_config_comment_line(&config, "if file is not found, default values are applied");
    rini_set_config_comment_line(&config, NULL);

    // Define config values
	rini_set_config_value(&config, "WINDOW_WIDTH", screenWidth, "Window width size");
	rini_set_config_value(&config, "WINDOW_HEIGHT", screenHeight, "Window height size");
	rini_set_config_valuef(&config, "Basket_Scale", basketScale, "Scale of the basket");
    rini_set_config_value(&config, "Basket_Velocity", basketVelocity, "Velocity of the basket");
    

    rini_save_config(config, "Color Catch Config.ini");

    rini_unload_config(&config);
#endif
    //--------------------------------------------------------------------------------------

    return 0;
};