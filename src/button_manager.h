#pragma once
#include <Arduino.h>

extern const uint8_t buttonPins[8];
const uint32_t DEBOUNCE_MS = 50;

struct Button {
    bool pressed;
    bool wasPressed;
    uint32_t lastChange;
};

class ButtonManager {
public:
    Button buttons[8];
    void update();
    bool isPressed(uint8_t btn) { return buttons[btn].pressed; }
    bool wasPressed(uint8_t btn);
};

extern ButtonManager buttons;