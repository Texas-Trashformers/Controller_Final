#pragma once

#include <TFT_eSPI.h>
#include "inputs.h"
#include "config.h"

extern TFT_eSPI tft;

void display_setup();
void drawUI(const InputState& state);