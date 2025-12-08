#include "Header/State.h"
#include "Header/Input.h"

// precaga
float precagaHeight = 0.10f;
float holeHeight = 0.0f;

static float minH = 0.02f;
static float maxH = 0.10f;
static float speed = 0.0005f;

// lavor
float waterHeight = 0.0f;
float maxWaterHeight = 0.20f;   // visina lavora

static double lastWaterIncrease = 0;

// ekrani
int desiredTemp = 24;
int currentTemp = 30;

static double lastTempChange = 0.0;


void UpdateState()
{
    // precaga - pomeranje 
    if (klimaOn)
    {
        if (precagaHeight > minH)
            precagaHeight -= speed;

        if (holeHeight < 0.08f)
            holeHeight += speed;
    }
    else
    {
        if (precagaHeight < maxH)
            precagaHeight += speed;

        if (holeHeight > 0.0f)
            holeHeight -= speed;
    }

    // voda u lavoru
    double now = glfwGetTime();
    if (klimaOn && now - lastWaterIncrease >= 1.0)
    {
        waterHeight += 0.02f;
        if (waterHeight > maxWaterHeight)
            waterHeight = maxWaterHeight;
        lastWaterIncrease = now;
    }

    if (waterHeight >= maxWaterHeight)
    {
        klimaOn = false;
    }
}

void UpdateTemp()
{
    double now = glfwGetTime();

    // menja temperaturu samo na svakih 2 sekunde
    if (now - lastTempChange < 2.0)
        return;

    if (currentTemp < desiredTemp)
        currentTemp++;
    else if (currentTemp > desiredTemp)
        currentTemp--;

    lastTempChange = now;
}

