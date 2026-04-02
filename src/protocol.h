#pragma once
#include <stdint.h>

struct ControlPacket {
  int8_t vx;
  int8_t vy;
  int8_t omega;
  uint16_t mask;  // Expanded to 16 bits for 12 buttons
};
