#include "button_manager.h"

// Pins for the 8 buttons. TODO: Move to a config file if they need to be changed.
const uint8_t buttonPins[8] = {38, 35, 36, 37, 15, 18, 16, 17}; // Example for ESP32

ButtonManager buttons;

void ButtonManager::update() {
  uint32_t now = millis();
  for (int i = 0; i < 8; i++) {
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