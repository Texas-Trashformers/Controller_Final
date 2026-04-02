#include "display.h"
#include "config.h"

TFT_eSPI tft = TFT_eSPI();

void display_setup() {
  tft.init();
  tft.setRotation(TFT_ROTATION);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);

  tft.drawString("Mock Controller", 8, 8);
  tft.drawLine(0, 26, 160, 26, TFT_DARKGREY);
}

void drawUI(const InputState& state) {
  // Clear value areas only (not the entire display)
  tft.fillRect(32, 28, 50, 10, TFT_BLACK);   // LX value
  tft.fillRect(32, 38, 50, 10, TFT_BLACK);   // LY value
  tft.fillRect(115, 28, 45, 10, TFT_BLACK);  // RX value
  tft.fillRect(115, 38, 45, 10, TFT_BLACK);  // RY value
  tft.fillRect(32, 54, 50, 10, TFT_BLACK);   // Omega value
  tft.fillRect(45, 70, 45, 10, TFT_BLACK);   // Battery value
  tft.fillRect(8, 88, 152, 10, TFT_BLACK);   // Button states

  // Left Joystick (LX / LY)
  tft.setTextSize(1);
  tft.setTextColor(TFT_CYAN);
  tft.setCursor(8, 28);   tft.print("LX:");
  tft.setCursor(8, 38);   tft.print("LY:");
  
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(32, 28);  tft.printf("%+4d", state.lx);
  tft.setCursor(32, 38);  tft.printf("%+4d", state.ly);

  // Right Joystick (RX / RY)
  tft.setTextSize(1);
  tft.setTextColor(TFT_CYAN);
  tft.setCursor(95, 28);  tft.print("RX:");
  tft.setCursor(95, 38);  tft.print("RY:");
  
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(115, 28); tft.printf("%+4d", state.rx);
  tft.setCursor(115, 38); tft.printf("%+4d", state.ry);

  // Rotation (Omega)
  tft.setTextSize(1);
  tft.setTextColor(TFT_CYAN);
  tft.setCursor(8, 54);   tft.print("OM:");
  
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(32, 54);  tft.printf("%+4d", state.omega);

  // Rotation mode indicator
  if (state.buttons[8]) {  // SHOULDER_1
    tft.setTextSize(1);
    tft.setTextColor(TFT_ORANGE);
    tft.setCursor(95, 54);
    tft.print("ROT");
  }

  // Battery voltage
  tft.setTextSize(1);
  tft.setTextColor(TFT_CYAN);
  tft.setCursor(8, 70);   tft.print("Batt:");
  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(45, 70);  tft.printf("%.2fV", state.batteryVoltage);

  // Joystick select buttons
  tft.setTextColor(state.joySelect[0] ? TFT_GREEN : TFT_RED);
  tft.setCursor(95, 70);  tft.print("L:");
  tft.printf("%d", state.joySelect[0]);
  
  tft.setTextColor(state.joySelect[1] ? TFT_GREEN : TFT_RED);
  tft.setCursor(120, 70); tft.print("R:");
  tft.printf("%d", state.joySelect[1]);

  // Button states (D-Pad A, D-Pad B, Shoulder buttons)
  tft.setCursor(8, 88);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.print("Btns: ");
  for (int i = 0; i < 10; i++) {
    tft.setTextColor(state.buttons[i] ? TFT_GREEN : TFT_RED);
    tft.printf("%d", state.buttons[i]);
  }
}