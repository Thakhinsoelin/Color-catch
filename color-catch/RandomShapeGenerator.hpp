#pragma once

#include "raylib.h"
#include "raymath.h"
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
    Shape shape;
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
    shape.velocity = { 0, (float)disSpeed(gen) };
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
    if (shape.Position.y + shape.size.y >= basketBounds.y && shape.Position.y - shape.size.y <= basketBounds.y + basketBounds.height) {
        // Check horizontal collision
        if (shape.Position.x + shape.size.x >= basketBounds.y && shape.Position.x - shape.size.x <= basketBounds.x + basketBounds.width) {
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

