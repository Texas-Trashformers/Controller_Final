#include "display.h"
#include "config.h"

TFT_eSPI tft = TFT_eSPI();

// Draws the static labels onto whatever is already on screen.
// Called by display_setup() on boot and by display_restore() after calibration.
static void drawStaticLabels() {
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Mock Controller", 8, 8);
  tft.drawLine(0, 26, 160, 26, TFT_DARKGREY);

  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setCursor(8,  28); tft.print("LX:");
  tft.setCursor(8,  38); tft.print("LY:");
  tft.setCursor(95, 28); tft.print("RX:");
  tft.setCursor(95, 38); tft.print("RY:");
  tft.setCursor(8,  54); tft.print("OM:");
  tft.setCursor(8,  70); tft.print("Batt:");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(8,  88); tft.print("Btns: ");
}

void display_setup() {
  tft.init();
  tft.setRotation(TFT_ROTATION);
  tft.fillScreen(TFT_BLACK);
  drawStaticLabels();
}

void display_restore() {
  tft.fillScreen(TFT_BLACK);
  drawStaticLabels();
}

void drawUI(const InputState& state) {
  tft.setTextSize(1);

  // Joystick values — background color means no fillRect needed, no flicker
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(32,  28); tft.printf("%+4d", state.lx);
  tft.setCursor(32,  38); tft.printf("%+4d", state.ly);
  tft.setCursor(115, 28); tft.printf("%+4d", state.rx);
  tft.setCursor(115, 38); tft.printf("%+4d", state.ry);
  tft.setCursor(32,  54); tft.printf("%+4d", state.omega);

  // ROT indicator: drawn in its color when active, black when inactive
  tft.setTextColor(state.buttons[8] ? TFT_ORANGE : TFT_BLACK, TFT_BLACK);
  tft.setCursor(95, 54); tft.print("ROT");

  // Battery voltage
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(45, 70); tft.printf("%.2fV", state.batteryVoltage);

  // Joystick select buttons (label + digit together so color applies to both)
  tft.setTextColor(state.joySelect[0] ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.setCursor(95,  70); tft.print("L:"); tft.printf("%d", state.joySelect[0]);
  tft.setTextColor(state.joySelect[1] ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.setCursor(120, 70); tft.print("R:"); tft.printf("%d", state.joySelect[1]);

  // Button states — cursor continues from after "Btns: " label at x=44
  tft.setCursor(44, 88);
  for (int i = 0; i < 10; i++) {
    tft.setTextColor(state.buttons[i] ? TFT_GREEN : TFT_RED, TFT_BLACK);
    tft.printf("%d", state.buttons[i]);
  }
}

// ---------------------------------------------------------------------------
// Calibration overlay
// ---------------------------------------------------------------------------
//
// Layout (160×128 screen):
//   y=  4  header bar "CALIBRATE RJ"
//   y= 14  divider
//   y= 20  "Step X/5  LABEL"
//   y=42–94  joystick circle diagram  (center 80,68  r=26)
//   y=110  countdown / status line

void drawCalibrationStep(uint8_t step, uint8_t total, const char* label,
                         int8_t jx, int8_t jy, int countdown) {
  tft.fillScreen(TFT_BLACK);

  // Header
  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(8, 4);
  tft.print("== CALIBRATE RJ ==");
  tft.drawLine(0, 14, 160, 14, TFT_DARKGREY);

  // Step info
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(8, 20);
  tft.printf("Step %d/%d  %s", step, total, label);

  // Joystick circle diagram
  const int cx = 80, cy = 68, r = 26;
  tft.drawCircle(cx, cy, r, TFT_DARKGREY);
  tft.drawLine(cx - r, cy,     cx + r, cy,     TFT_DARKGREY); // horizontal cross
  tft.drawLine(cx,     cy - r, cx,     cy + r, TFT_DARKGREY); // vertical cross

  // Target dot — cyan ring shows where to hold the joystick
  int dotX = cx + jx * 20;
  int dotY = cy + jy * 20;
  tft.fillCircle(dotX, dotY, 6, TFT_CYAN);
  tft.drawCircle(dotX, dotY, 8, TFT_WHITE); // white ring for visibility

  // Countdown / status
  tft.setCursor(8, 110);
  if (countdown > 0) {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.printf("Hold still...  %ds  ", countdown);
  } else {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.print("Sampling...        ");
  }
}
