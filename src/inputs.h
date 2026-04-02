#pragma once
#include <Arduino.h>

struct InputState {
    int8_t lx;      // Left joystick X
    int8_t ly;      // Left joystick Y
    int8_t rx;      // Right joystick X
    int8_t ry;      // Right joystick Y
    int8_t omega;   // Rotation (from left joystick when in rot mode)
    bool buttons[10]; // D-Pad A (4) + D-Pad B (4) + Shoulders (2)
    bool joySelect[2]; // Joystick select buttons: [0]=left, [1]=right
    float batteryVoltage; // Battery voltage in volts
    bool changed;
};

extern InputState inputs;

void inputs_setup();
void calibrateRightJoystick();
