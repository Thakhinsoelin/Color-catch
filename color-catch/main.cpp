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


enum shapes {
	CIRCLE = 0,
	RECTANGLE,
	TRIANGLE
};

struct Shape {
	int type = 0;
    Color color = { 255, 0, 0, 255 };
    Vector2 Position = { 0 };
    Vector2 size = { 0 };
    Vector2 velocity{ 0 };
    bool isActive = { false };
};

// Function to generate a random shape
Shape generateShape(int screenWidth) {
    Shape shape ;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disX(0, screenWidth);
    std::uniform_real_distribution<> disColor(0, 3); // 4 colors
    std::uniform_real_distribution<> disShape(0, 2); // 3 shapes
    std::uniform_real_distribution<> disSize(20, 50);
    std::uniform_real_distribution<> disSpeed(300, 400);
	shape.type = disColor(gen);

    shape.Position.x = disX(gen);
    shape.Position.y = -50; // Start at the top
    
    shape.color = { (unsigned char)(disColor(gen) * 85), (unsigned char)(disColor(gen) * 85), (unsigned char)(disColor(gen) * 85), 255 }; // Example color generation.
    
    shape.size = { 50, (float)disSize(gen) };
    shape.velocity = {0, (float)disSpeed(gen) };
    shape.isActive = true;
    return shape;
}

// Function to update shape positions
void updateShapes(std::vector<Shape>& shapes, float deltaTime) {
    for (auto& shape : shapes) {
        shape.Position.y += shape.velocity.y * deltaTime;
    }
}

// Function to draw shapes
void drawShapes(const std::vector<Shape>& shapes) {
    for (const auto& shape : shapes) {
        if (shape.type == shapes::CIRCLE) {
            /*DrawCircle(shape.Position.x, shape.Position.y, shape.size / 2, shape.color);*/
			DrawCircleV(shape.Position, shape.size.x / 2, shape.color);
        }
        if (shape.type == shapes::RECTANGLE) {
            DrawRectangle(shape.Position.x - shape.size.x / 2, shape.Position.y - shape.size.y / 2, shape.size.x, shape.size.y, shape.color);
			//DrawRectangleV(shape.Position, shape.size, shape.color);
        } 
        if (shape.type == shapes::TRIANGLE) {
			DrawTriangle(shape.Position, { shape.Position.x - shape.size.x / 2, shape.Position.y + shape.size.y }, { shape.Position.x + shape.size.x / 2, shape.Position.y + shape.size.y }, shape.color);
        }
    }
}

// Function to check collisions (simplified)
bool checkCollision(const Shape& shape, const Vector2& basketPosition, const Texture& basket, float basketScale) {
    // Basic example: check y and color
    if (shape.Position.y + shape.size.y / 2 >= basketPosition.y && shape.Position.y - shape.size.y / 2 <= basketPosition.y + basket.height * basketScale) {
        // Check horizontal collision
        if (shape.Position.x + shape.size.x / 2 >= basketPosition.y && shape.Position.x - shape.size.x / 2 <= basketPosition.x + basket.width * basketScale) {
            // Position collision only
            return true;
        }
    }
    if (shape.Position.y > GetScreenHeight()) {
        return true;
    }
    return false;
}

bool checkCollisionold(const Shape& shape, const Rectangle& basketBounds) {
    // Basic example: check y and color
    if (shape.Position.y + shape.size.y >= basketBounds.y && shape.Position.y - shape.size.y  <= basketBounds.y + basketBounds.height) {
        // Check horizontal collision
        if (shape.Position.x + shape.size.x  >= basketBounds.y && shape.Position.x - shape.size.x <= basketBounds.x + basketBounds.width) {
            // Position collision only
            return true;
        }
    }
    if (shape.Position.y > GetScreenHeight()) {
        return true;
    }
    return false;
}

bool checkCollision(const Shape& shape, const Rectangle& basketBounds) {
    // Basic example: check y and color
    if (shape.Position.y + shape.size.y >= basketBounds.y && shape.Position.y - shape.size.y <= basketBounds.y + basketBounds.height) {
        // Check horizontal collision
        if (shape.Position.x + shape.size.x >= basketBounds.x && shape.Position.x - shape.size.x <= basketBounds.x + basketBounds.width) {
            // Position collision only
            return true;
        }
    }
    if (shape.Position.y > GetScreenHeight()) {
        return true;
    }
    return false;
}

// DPI scaling functions
float ScaleToDPIF(float value)
{
    return GetWindowScaleDPI().x * value;
}

int ScaleToDPII(int value)
{
    return int(GetWindowScaleDPI().x * value);
}

void GenerateRandomShapes(std::vector<int>& arr) {

}


int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    
    int screenWidth = 1280;
    int screenHeight = 800;

    const int gravity = NULL;
    int scoreCounter = 0;
    int fontSize = 24;

    int fontHeight = GetFontDefault().baseSize;

    

    char scoreText[255];
    sprintf(scoreText, "SCORE: %d", scoreCounter);
    int textWidth = MeasureText(scoreText, fontSize);
    TRACE
    int x = screenWidth - textWidth - 10; // 10 pixels padding from the right edge
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
        DrawText(scoreText, x, y, fontSize, GREEN);
        //Draw main things here
        drawShapes(fallingShapes);
        for (auto& e : fallingShapes) {
            if (checkCollision(e, basketBound)) {
                e.isActive = false;
                scoreCounter++;
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