#pragma once

#include <TFT_eSPI.h>
#include "inputs.h"
#include "config.h"

extern TFT_eSPI tft;

void display_setup();
void display_restore();   // Clear + redraw static labels (use after calibration)
void drawUI(const InputState& state);

// Calibration overlay.  jx/jy: -1=left/fwd  0=center  +1=right/back
// countdown: 3/2/1=seconds remaining  0="Sampling..."
void drawCalibrationStep(uint8_t step, uint8_t total, const char* label,
                         int8_t jx, int8_t jy, int countdown);
