#include "Header/State.h"

bool klimaOn = false;
bool clickHandled = false;

float lampX = 0.65f;
float lampY = 0.50f;
float lampR = 0.02f;

// precaga
float precagaHeight = 0.10f;
float holeHeight = 0.0f;

static float precagaMin = 0.02f;
static float holeMax = 0.08f;
static float speed = 0.00001f;

void UpdateState(bool klimaOn)
{
    if (klimaOn)
    {
        if (precagaHeight > precagaMin) precagaHeight -= speed;
        if (holeHeight < holeMax) holeHeight += speed;
    }
    else
    {
        if (precagaHeight < 0.10f) precagaHeight += speed;
        if (holeHeight > 0.0f) holeHeight -= speed;
    }
}
