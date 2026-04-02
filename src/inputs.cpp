#include "inputs.h"
#include "button_manager.h"
#include "config.h"
#include "preference_manager.h"   // for center calibration values

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
  // NOTE: Only set ADC attenuation for pins that are actually soldered and connected
  // Reading unconnected ADC pins can cause noise and interfere with SPI communication to display
  
  // Right Joystick ADC pins (SOLDERED)
  analogSetPinAttenuation(JOY_R_HORIZONTAL, ADC_11db);
  analogSetPinAttenuation(JOY_R_VERTICAL, ADC_11db);
  
  // Left Joystick ADC pins - NOT YET SOLDERED - DO NOT READ
  // analogSetPinAttenuation(JOY_L_HORIZONTAL, ADC_11db);
  // analogSetPinAttenuation(JOY_L_VERTICAL, ADC_11db);
  
  // Battery Monitor - ONLY IF SOLDERED
  // analogSetPinAttenuation(BATTERY_MONITOR, ADC_11db);

  // Set pin modes for D-Pad and shoulder buttons (INPUT_PULLUP for active-low no external pull-up)
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Joystick select buttons - ONLY IF SOLDERED
  // pinMode(JOY_L_SELECT, INPUT);
  // pinMode(JOY_R_SELECT, INPUT);
}

void updateInputs() {
  int8_t old_lx = inputs.lx;
  int8_t old_ly = inputs.ly;
  int8_t old_rx = inputs.rx;
  int8_t old_ry = inputs.ry;
  int8_t old_omega = inputs.omega;
  bool old_joyL = inputs.joySelect[0];
  bool old_joyR = inputs.joySelect[1];
  float old_batt = inputs.batteryVoltage;

  // Left Joystick
  int rawLX = analogRead(JOY_L_HORIZONTAL);
  int rawLY = analogRead(JOY_L_VERTICAL);

  int8_t new_lx = applyExpo(rawLX, prefs.centerLX);
  int8_t new_ly = -applyExpo(rawLY, prefs.centerLY);  // Invert Y axis

  // Right Joystick
  int rawRX = analogRead(JOY_R_HORIZONTAL);
  int rawRY = analogRead(JOY_R_VERTICAL);

  int8_t new_rx = applyExpo(rawRX, prefs.centerRX);
  int8_t new_ry = -applyExpo(rawRY, prefs.centerRY);  // Invert Y axis

  // Joystick select buttons (with debouncing)
  static uint32_t lastSelectChange[2] = {0, 0};
  static bool debounceSelect[2] = {false, false};
  uint32_t now = millis();

  for (int j = 0; j < 2; j++) {
    bool raw = buttons.isJoySelectPressed(j);
    if (raw != debounceSelect[j] && (now - lastSelectChange[j] > DEBOUNCE_MS)) {
      debounceSelect[j] = raw;
      lastSelectChange[j] = now;
      inputs.joySelect[j] = raw;
    }
  }

  // Battery voltage monitoring
  int rawBatt = analogRead(BATTERY_MONITOR);
  float battVoltage = (rawBatt / 4095.0f) * 3.3f * BATTERY_DIVIDER_RATIO;
  inputs.batteryVoltage = battVoltage;

  // Rotation mode: when shoulder button 1 (index 8) is pressed, use left X for rotation
  bool rotMode = buttons.isPressed(8);   // SHOULDER_1

  inputs.lx = rotMode ? 0 : new_lx;
  inputs.ly = rotMode ? 0 : new_ly;
  inputs.rx = new_rx;
  inputs.ry = new_ry;
  inputs.omega = rotMode ? new_lx : 0;

  // D-Pad and shoulder buttons
  buttons.update();
  bool buttonChange = false;
  for (int i = 0; i < 10; i++) {
    bool current = buttons.isPressed(i);
    if (current != inputs.buttons[i]) {
      inputs.buttons[i] = current;
      buttonChange = true;
    }
  }

  // Set the changed flag if there's a change in joystick values or button states
  if (buttonChange || inputs.lx != old_lx || inputs.ly != old_ly || 
      inputs.rx != old_rx || inputs.ry != old_ry || inputs.omega != old_omega ||
      inputs.joySelect[0] != old_joyL || inputs.joySelect[1] != old_joyR ||
      inputs.batteryVoltage != old_batt) {
    inputs.changed = true;
  }
}