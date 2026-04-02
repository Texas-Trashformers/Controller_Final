// comm.cpp
#include "comm.h"

static uint32_t lastSendTime = 0;
bool espNowTxOk = false;
ESPNowSender espNowSender;

void comm_setup() {
  espNowSender.begin();
}

// Button → mask bit assignments
//   buttons[8] SHOULDER_1 → ROT_MODE_BIT (matches "ROT" display indicator)
//   buttons[9] SHOULDER_2 → SLOW_BIT
//   FAST_BIT: unmapped until a dedicated button is wired

void sendControlPacket() {
  uint32_t now = millis();
  if (now - lastSendTime < 20) return;  // 50 Hz — ChassisBrain watchdog requires ≤200ms
  lastSendTime = now;

  ControlPacket pkt = {};

  // Right joystick drives movement (left joystick not yet soldered)
  pkt.vx    = inputs.rx;
  pkt.vy    = inputs.ry;
  pkt.omega = inputs.omega;

  // Mask bits
  if (inputs.buttons[8]) pkt.mask |= (1 << ROT_MODE_BIT);  // SHOULDER_1 → rotate only
  if (inputs.buttons[9]) pkt.mask |= (1 << SLOW_BIT);       // SHOULDER_2 → slow mode

  espNowSender.send(pkt);
}
