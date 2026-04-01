#include "inputs.h"
#include "button_manager.h"
#include "config.h"
#include "preference_manager.h"   // only for centerX/Y

extern PreferencesManager prefs;

InputState inputs;
Ticker inputTimer;

int8_t applyExpo(int raw, int center) {
  int deviation = raw - center;
  if (abs(deviation) < prefs.deadzone) return 0;

  float norm = (float)abs(deviation) / (2048.0f - prefs.deadzone);
  float curved = pow(norm, prefs.expoCurve) * prefs.maxOutput;
  return (deviation > 0) ? (int8_t)curved : (int8_t)-curved;
}

void inputs_setup() {
  analogSetPinAttenuation(JOY_VRX, ADC_11db);
  analogSetPinAttenuation(JOY_VRY, ADC_11db);

  for (uint8_t i = 0; i < 8; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void updateInputs() {
  int8_t old_vx = inputs.vx;
  int8_t old_vy = inputs.vy;
  int8_t old_omega = inputs.omega;

  // Joystick
  int rawX = analogRead(JOY_VRX);
  int rawY = analogRead(JOY_VRY);

  int8_t new_vx = applyExpo(rawX, prefs.centerX);
  int8_t new_vy = -applyExpo(rawY, prefs.centerY);

  bool rotMode = buttons.isPressed(5);   // shoulder button 1 = rotation mode

  inputs.vx = rotMode ? 0 : new_vx;
  inputs.vy = rotMode ? 0 : new_vy;
  inputs.omega = rotMode ? new_vx : 0;

  // Buttons
  buttons.update();
  bool buttonChange = false;
  for (int i = 0; i < 8; i++) {
    bool current = buttons.isPressed(i);
    if (current != inputs.buttons[i]) {
      inputs.buttons[i] = current;
      buttonChange = true;
    }
  }

  // Set the changed flag only if there's a change in joystick values or button states
  if (buttonChange || inputs.vx != old_vx || inputs.vy != old_vy || inputs.omega != old_omega) {
    inputs.changed = true;
  }
}