#include <Arduino.h>
#include "config.h"
#include "display.h"
#include "inputs.h"
#include "button_manager.h"
// #include "preference_manager.h"   // commented for now
// #include "comm.h"


void setup() {
  Serial.begin(115200);
  delay(1500);
  Serial.println("\n\n=== MOCK CONTROLLER - SAFE BUTTON DEBUG MODE ===");
  Serial.println("Build time: " __DATE__ " " __TIME__);
  Serial.flush();

  // 1. Preferences
  Serial.println("[1/5] Initializing Preferences...");
  //prefs.begin();
  //Serial.printf("   Deadzone:%d Expo:%.2f Max:%d CenterX:%d CenterY:%d\n",
  //              prefs.deadzone, prefs.expoCurve, prefs.maxOutput,
  //              prefs.centerX, prefs.centerY);
  Serial.flush();

  // 2. Display
  Serial.println("[2/5] Initializing TFT...");
  display_setup();
  Serial.println("   TFT ready.");
  Serial.flush();

  // 3. Safe Button Setup (this is the critical part)
  Serial.println("[3/5] Setting up buttons SAFELY (skipping flash pins 26/27/32)...");
  
  // Only configure safe pins
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t p = buttonPins[i];
    if (p == 26 || p == 27 || p == 32) {
      Serial.printf("   Pin %2d → SKIPPED (flash conflict)\n", p);
      continue;
    }
    pinMode(p, INPUT_PULLUP);
    Serial.printf("   Pin %2d → INPUT_PULLUP enabled\n", p);
  }
  Serial.println("   Button pinMode complete (safe pins only).");
  Serial.flush();

  // 4. Initialize button manager state
  Serial.println("[4/5] Initializing ButtonManager state...");
  for (int i = 0; i < 8; i++) {
    buttons.buttons[i].pressed = false;
    buttons.buttons[i].wasPressed = false;
    buttons.buttons[i].lastChange = millis();
  }
  Serial.println("   ButtonManager ready.");
  Serial.flush();

  // 5. Skip comm for now
  Serial.println("[5/5] Skipping comm_setup() for stability test.");
  Serial.println("\n=== SETUP COMPLETE - WATCHING BUTTONS & DISPLAY ===");
  Serial.println("Buttons on screen should now reflect real hardware (except skipped pins).");
  Serial.flush();

  // Initial draw
  drawUI(inputs);   // inputs struct is zero-initialized by default
}

void loop() {
  static uint32_t lastStatus = 0;
  static uint32_t lastUpdate = 0;
  static uint32_t loopCount = 0;

  loopCount++;

  // Update buttons safely every 20ms
  if (millis() - lastUpdate > 20) {
    buttons.update();                    // safe version from earlier

    // Copy button states to inputs struct for display
    bool anyChange = false;
    for (int i = 0; i < 8; i++) {
      bool current = buttons.isPressed(i);
      if (current != inputs.buttons[i]) {
        inputs.buttons[i] = current;
        anyChange = true;
      }
    }

    // Simple fake joystick values so the screen isn't empty
    inputs.vx = 42;
    inputs.vy = -67;
    inputs.omega = 15;

    if (anyChange || (millis() - lastUpdate > 200)) {
      drawUI(inputs);
      lastUpdate = millis();
    }
  }

  // Status every 2 seconds
  if (millis() - lastStatus > 2000) {
    Serial.printf("[DEBUG] Loop #%lu | Heap:%d | Millis:%lu | Btns: ",
                  loopCount, ESP.getFreeHeap(), millis());
    
    for (int i = 0; i < 8; i++) {
      Serial.print(inputs.buttons[i] ? "1" : "0");
    }
    Serial.println();
    Serial.flush();
    lastStatus = millis();
  }

  delay(5);
  yield();
}