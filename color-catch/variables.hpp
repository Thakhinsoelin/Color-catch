#pragma once
#include "rini.h"
int screenWidth = 1280;
int screenHeight = 800;

int scoreCounter = 0;
int fontSize = 24;

void LoadBasketValue(const char* filepath) {
#ifdef DEBUG
	rini_config config = rini_load_config("Color Catch Config.ini");

	float basketScale = rini_get_config_valuef(config, "Basket_Scale");
	int basketVelocity = rini_get_config_value(config, "Basket_Velocity");
#else

	float basketScale = 0.075;
	int basketVelocity = 756;
#endif
}