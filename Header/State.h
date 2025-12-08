#pragma once

extern bool klimaOn;

// precaga
extern float precagaHeight;
extern float holeHeight;

// lavor (voda)
extern float waterHeight;
extern float maxWaterHeight;

// ekrani
extern int desiredTemp;
extern int currentTemp;

void UpdateState();
void UpdateTemp();
