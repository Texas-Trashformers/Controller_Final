#include <Arduino.h>
#include "config.h"
#include "display.h"
#include "inputs.h"
#include "button_manager.h"
#include "preference_manager.h"
// #include "comm.h"


void setup() {
  Serial.begin(115200);
  delay(1500);
  Serial.println("\n\n=== MOCK CONTROLLER - SAFE BUTTON DEBUG MODE ===");
  Serial.println("Build time: " __DATE__ " " __TIME__);
  Serial.flush();

  // 1. Preferences
  Serial.println("[1/5] Initializing Preferences...");
  prefs.begin();
  Serial.printf("   Deadzone:%d Expo:%.2f Max:%d\n",
                prefs.deadzone, prefs.expoCurve, prefs.maxOutput);
  Serial.printf("   RX: min=%d center=%d max=%d\n",
                prefs.minRX, prefs.centerRX, prefs.maxRX);
  Serial.printf("   RY: min=%d center=%d max=%d\n",
                prefs.minRY, prefs.centerRY, prefs.maxRY);
  Serial.flush();

  // 2. Inputs Setup — launches FreeRTOS task on Core 0 for periodic ADC + button sampling
  Serial.println("[2/5] Initializing inputs (FreeRTOS task on Core 0)...");
  inputs_setup();
  Serial.println("   Input task running.");
  Serial.flush();

  // 3. Display
  Serial.println("[3/5] Initializing TFT...");
  display_setup();
  Serial.println("   TFT ready.");
  Serial.flush();

  // 4. Safe Button Setup (configure pins; task handles reading)
  Serial.println("[4/5] Setting up button pins SAFELY (skipping flash pins 26/27/32)...");
  for (uint8_t i = 0; i < 10; i++) {
    uint8_t p = buttonPins[i];
    if (p == 26 || p == 27 || p == 32) {
      Serial.printf("   Pin %2d → SKIPPED (flash conflict)\n", p);
      continue;
    }
    pinMode(p, INPUT_PULLUP);
    Serial.printf("   Pin %2d → INPUT_PULLUP enabled\n", p);
  }
  Serial.println("   Button pins ready.");
  Serial.flush();

  // 5. Initialize button manager state
  Serial.println("[5/5] Initializing ButtonManager state...");
  for (int i = 0; i < 10; i++) {
    buttons.buttons[i].pressed = false;
    buttons.buttons[i].wasPressed = false;
    buttons.buttons[i].lastChange = millis();
  }
  Serial.println("   ButtonManager ready.");
  Serial.flush();

  Serial.println("[6/6] Skipping comm_setup() for stability test.");
  Serial.println("\n=== SETUP COMPLETE ===");
  Serial.flush();

  drawUI(inputs);
}

void loop() {
  static uint32_t lastDraw   = 0;   // display rate limiter (50ms = 20fps)
  static uint32_t lastStatus = 0;   // serial status print (2s)
  static uint32_t loopCount  = 0;
  static uint32_t calibHoldStart = 0;
  static bool     calibTriggered  = false;

  loopCount++;

  // Serial calibration command: send 'c' to calibrate right joystick center
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'c') calibrateRightJoystick();
  }

  // Calibration: hold both shoulders 3 seconds while joystick is at center
  bool bothShoulders = buttons.isPressed(8) && buttons.isPressed(9);
  if (bothShoulders) {
    if (calibHoldStart == 0) calibHoldStart = millis();
    else if (!calibTriggered && (millis() - calibHoldStart >= 3000)) {
      calibrateRightJoystick();
      calibTriggered = true;
    }
  } else {
    calibHoldStart = 0;
    calibTriggered = false;
  }

  // Display update: rate-limited to 50ms — inputs struct is kept fresh by Core 0 task
  if (millis() - lastDraw >= 50) {
    lastDraw = millis();
    drawUI(inputs);
  }

  // Serial status every 2 seconds
  if (millis() - lastStatus >= 2000) {
    Serial.printf("[DEBUG] Loop #%lu | Heap:%d | Millis:%lu | Btns: ",
                  loopCount, ESP.getFreeHeap(), millis());
    for (int i = 0; i < 10; i++) {
      Serial.print(inputs.buttons[i] ? "1" : "0");
    }
    Serial.println();
    Serial.flush();
    lastStatus = millis();
  }

  delay(5);
  yield();
}
