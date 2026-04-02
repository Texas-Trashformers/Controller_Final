#pragma once

// Pins
// Left Joystick
#define JOY_L_HORIZONTAL 1
#define JOY_L_VERTICAL 2
#define JOY_L_SELECT 48

// Right Joystick
#define JOY_R_HORIZONTAL 4
#define JOY_R_VERTICAL 5
#define JOY_R_SELECT 10

// Buttons (D-Pads and Shoulders)
#define DPAD_A_UP 38      // SW1
#define DPAD_A_DOWN 35    // SW3
#define DPAD_A_LEFT 36    // SW4
#define DPAD_A_RIGHT 37   // SW5
#define DPAD_B_UP 15      // SW7
#define DPAD_B_DOWN 18    // SW8
#define DPAD_B_LEFT 16    // SW9
#define DPAD_B_RIGHT 17   // SW10
#define SHOULDER_1 43     // SW2
#define SHOULDER_2 44     // SW6

// Battery Monitoring
#define BATTERY_MONITOR 45
#define BATTERY_DIVIDER_RATIO 3.1276f  // (100k + 47k) / 47k

// Display
#define SCREEN_UPDATE_MS 50
#define TFT_ROTATION 1

// Preferences
#define DEFAULT_DEADZONE 50
#define DEFAULT_EXPO_CURVE 1.5f
#define DEFAULT_MAX_OUTPUT 127

// Debug
#define SERIAL_DEBUG