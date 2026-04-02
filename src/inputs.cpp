#include "inputs.h"
#include "button_manager.h"
#include "config.h"
#include "preference_manager.h"
#include "display.h"

extern PreferencesManager prefs;

InputState inputs;

// Maps a raw ADC reading to [-maxOutput..+maxOutput] using the calibrated
// center/min/max for that axis.  Full deflection always produces maxOutput.
static int8_t applyAxis(int raw, int center, int minVal, int maxVal) {
  int deviation = raw - center;
  if (abs(deviation) < prefs.deadzone) return 0;

  // Pick the calibrated half-range for the direction of deflection
  float range = (deviation > 0)
    ? (float)(maxVal - center)
    : (float)(center - minVal);

  if (range < 1.0f) return 0;  // Uncalibrated or degenerate — skip

  // Normalize [deadzone..range] → [0..1], apply expo curve
  float norm = (float)(abs(deviation) - prefs.deadzone) / (range - prefs.deadzone);
  norm = constrain(norm, 0.0f, 1.0f);
  float curved = pow(norm, prefs.expoCurve) * (float)prefs.maxOutput;

  return (deviation > 0) ? (int8_t)curved : (int8_t)-curved;
}

// Runs on Core 0 every 10ms — samples ADC and buttons independently of display
static void inputSamplingTask(void* pvParameters) {
  for (;;) {
    // Right Joystick (SOLDERED)
    int rawRX = analogRead(JOY_R_HORIZONTAL);
    int rawRY = analogRead(JOY_R_VERTICAL);
    inputs.rx =  applyAxis(rawRX, prefs.centerRX, prefs.minRX, prefs.maxRX);
    inputs.ry = -applyAxis(rawRY, prefs.centerRY, prefs.minRY, prefs.maxRY); // Invert Y

    // Left joystick disabled (not yet soldered)
    inputs.lx = 0;
    inputs.ly = 0;
    inputs.omega = 0;
    inputs.joySelect[0] = false;
    inputs.joySelect[1] = false;
    inputs.batteryVoltage = 0.0f;

    // Button states
    buttons.update();
    for (int i = 0; i < 10; i++) {
      inputs.buttons[i] = buttons.isPressed(i);
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void inputs_setup() {
  // Right Joystick ADC pins (SOLDERED)
  analogSetPinAttenuation(JOY_R_HORIZONTAL, ADC_11db);
  analogSetPinAttenuation(JOY_R_VERTICAL, ADC_11db);

  // Left Joystick ADC pins - NOT YET SOLDERED - DO NOT READ
  // analogSetPinAttenuation(JOY_L_HORIZONTAL, ADC_11db);
  // analogSetPinAttenuation(JOY_L_VERTICAL, ADC_11db);

  // Battery Monitor - ONLY IF SOLDERED
  // analogSetPinAttenuation(BATTERY_MONITOR, ADC_11db);

  // Joystick select buttons - ONLY IF SOLDERED
  // pinMode(JOY_L_SELECT, INPUT);
  // pinMode(JOY_R_SELECT, INPUT);

  // Launch input sampling task on Core 0 (display/logic runs on Core 1)
  BaseType_t ok = xTaskCreatePinnedToCore(
    inputSamplingTask,
    "Inputs",
    4096,     // stack size in bytes
    nullptr,
    2,        // priority above Arduino loop (1), so it always meets its 10ms period
    nullptr,
    0         // Core 0
  );
  if (ok != pdPASS) {
    Serial.println("[ERROR] Input sampling task failed to create!");
  }
}

// ---------------------------------------------------------------------------
// Calibration
// ---------------------------------------------------------------------------

// Average 64 ADC samples over ~320ms to reduce noise
static int sampleAxis(uint8_t pin) {
  const int SAMPLES = 64;
  long sum = 0;
  for (int i = 0; i < SAMPLES; i++) {
    sum += analogRead(pin);
    delay(5);
  }
  return (int)(sum / SAMPLES);
}

// Show calibration step on screen with 3→2→1→"Sampling..." countdown,
// then leave the screen on "Sampling..." while sampleAxis() runs.
static void calCountdown(uint8_t step, const char* label, int8_t jx, int8_t jy) {
  Serial.printf("\nStep %d/5: %s  Sampling in 3s...\n", step, label);
  for (int t = 3; t >= 1; t--) {
    drawCalibrationStep(step, 5, label, jx, jy, t);
    delay(1000);
  }
  drawCalibrationStep(step, 5, label, jx, jy, 0); // "Sampling..."
}

// Full 5-position calibration.
// Trigger: hold both shoulders 3s, or send 'c' over Serial.
// Each step: move to position, hold still — sampling begins automatically.
void calibrateRightJoystick() {
  Serial.println("\n=== RIGHT JOYSTICK CALIBRATION ===");

  // Step 1 — Center
  calCountdown(1, "CENTER", 0, 0);
  prefs.centerRX = sampleAxis(JOY_R_HORIZONTAL);
  prefs.centerRY = sampleAxis(JOY_R_VERTICAL);
  Serial.printf("  Center:  RX=%d  RY=%d\n", prefs.centerRX, prefs.centerRY);

  // Step 2 — Right
  calCountdown(2, "RIGHT", 1, 0);
  int rightRaw = sampleAxis(JOY_R_HORIZONTAL);
  Serial.printf("  Right:   RX=%d\n", rightRaw);

  // Step 3 — Left
  calCountdown(3, "LEFT", -1, 0);
  int leftRaw = sampleAxis(JOY_R_HORIZONTAL);
  Serial.printf("  Left:    RX=%d\n", leftRaw);

  // Assign X min/max — higher ADC value is maxRX regardless of physical direction
  prefs.maxRX = max(rightRaw, leftRaw);
  prefs.minRX = min(rightRaw, leftRaw);

  // Step 4 — Forward
  calCountdown(4, "FORWARD", 0, -1);
  int fwdRaw = sampleAxis(JOY_R_VERTICAL);
  Serial.printf("  Forward: RY=%d\n", fwdRaw);

  // Step 5 — Back
  calCountdown(5, "BACK", 0, 1);
  int backRaw = sampleAxis(JOY_R_VERTICAL);
  Serial.printf("  Back:    RY=%d\n", backRaw);

  // Assign Y min/max
  prefs.maxRY = max(fwdRaw, backRaw);
  prefs.minRY = min(fwdRaw, backRaw);

  prefs.saveAll();

  Serial.println("\n[CAL] Saved!");
  Serial.printf("  RX  min=%d  center=%d  max=%d\n", prefs.minRX, prefs.centerRX, prefs.maxRX);
  Serial.printf("  RY  min=%d  center=%d  max=%d\n", prefs.minRY, prefs.centerRY, prefs.maxRY);
  Serial.println("=================================\n");

  // Restore normal UI
  display_restore();
  drawUI(inputs);
}
