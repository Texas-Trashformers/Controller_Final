#pragma once
#include <stdint.h>

// Must match ChassisBrain exactly — 4 bytes, no padding
#define PACKET_SIZE 4

#define SLOW_BIT     3   // mask |= (1<<3) → max PWM 65
#define FAST_BIT     4   // mask |= (1<<4) → max PWM 255
#define ROT_MODE_BIT 5   // mask |= (1<<5) → zero vx/vy, rotate only

struct ControlPacket {
  int8_t  vx;     // left/right strafe  -100..100
  int8_t  vy;     // forward/back       -100..100
  int8_t  omega;  // rotation           -100..100
  uint8_t mask;   // button bits (see defines above)
};
