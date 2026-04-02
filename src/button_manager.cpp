#include "button_manager.h"
#include "config.h"

// Pins for the 10 buttons: D-Pad A (4), D-Pad B (4), Shoulders (2)
// Note: Joystick select buttons (GPIO48, GPIO10) are handled separately with INPUT mode
const uint8_t buttonPins[10] = {
    DPAD_A_UP, DPAD_A_DOWN, DPAD_A_LEFT, DPAD_A_RIGHT,  // 0-3: D-Pad A
    DPAD_B_UP, DPAD_B_DOWN, DPAD_B_LEFT, DPAD_B_RIGHT,  // 4-7: D-Pad B
    SHOULDER_1, SHOULDER_2                              // 8-9: Shoulders
};

ButtonManager buttons;

void ButtonManager::update() {
  uint32_t now = millis();
  for (int i = 0; i < 10; i++) {
    bool raw = !digitalRead(buttonPins[i]);
    if (raw != buttons[i].pressed && (now - buttons[i].lastChange > DEBOUNCE_MS)) {
      buttons[i].pressed = raw;
      buttons[i].lastChange = now;
      if (raw) buttons[i].wasPressed = true;
    }
  }
}

bool ButtonManager::wasPressed(uint8_t btn) {
  bool p = buttons[btn].wasPressed;
  buttons[btn].wasPressed = false;
  return p;
}

bool ButtonManager::isJoySelectPressed(uint8_t joy) {
  uint8_t pin = (joy == 0) ? JOY_L_SELECT : JOY_R_SELECT;
  // Active-low with external pull-up: pressed = LOW
  return !digitalRead(pin);
}