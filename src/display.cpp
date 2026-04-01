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
  tft.fillRect(0, 28, 160, 90, TFT_BLACK);   // clear only dynamic area

  // VX / VY / OMEGA
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(105, 38);  tft.printf("%+4d", state.vx);
  tft.setCursor(105, 60);  tft.printf("%+4d", state.vy);
  tft.setCursor(105, 82);  tft.printf("%+4d", state.omega);

  // Rotation mode indicator
  if (state.buttons[5]) {
    tft.setTextColor(TFT_ORANGE);
    tft.drawString("ROT MODE", 95, 110);
  }

  // Button states
  tft.setCursor(8, 98);
  tft.setTextColor(TFT_WHITE);
  tft.print("Btns: ");
  for (int i = 0; i < 8; i++) {
    tft.setTextColor(state.buttons[i] ? TFT_GREEN : TFT_RED);
    tft.printf("%d", state.buttons[i]);
  }
}