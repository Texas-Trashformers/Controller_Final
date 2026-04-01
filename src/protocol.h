#pragma once
#include <stdint.h>

struct ControlPacket {
  int8_t vx;
  int8_t vy;
  int8_t omega;
  uint8_t mask;
};
