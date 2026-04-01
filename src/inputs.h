#pragma once
#include <Arduino.h>
#include <Ticker.h>

struct InputState {
    int8_t vx;
    int8_t vy;
    int8_t omega;
    bool buttons[8];
    bool changed;
};

extern InputState inputs;
extern Ticker inputTimer;

void inputs_setup();
void updateInputs();
