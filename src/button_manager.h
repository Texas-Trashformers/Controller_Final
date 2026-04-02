#pragma once
#include <Arduino.h>
#include "config.h"

extern const uint8_t buttonPins[10];
const uint32_t DEBOUNCE_MS = 50;

struct Button {
    bool pressed;
    bool wasPressed;
    uint32_t lastChange;
};

class ButtonManager {
public:
    Button buttons[10];
    void update();
    bool isPressed(uint8_t btn) { return buttons[btn].pressed; }
    bool wasPressed(uint8_t btn);
    // Joystick select buttons (separate, with INPUT mode due to external pull-ups)
    bool isJoySelectPressed(uint8_t joy);  // 0=left, 1=right
};

extern ButtonManager buttons;