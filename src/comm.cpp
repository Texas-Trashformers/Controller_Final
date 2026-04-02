// comm.cpp
#include "comm.h"

static uint32_t lastSendTime = 0;
static ControlPacket lastSentPacket = {0};
bool espNowTxOk = false;
ESPNowSender espNowSender;

void comm_setup() {
  espNowSender.begin();
}

void sendControlPacket() {
  uint32_t now = millis();
  if (now - lastSendTime < 25) return;   // min 40 Hz

  ControlPacket pkt = {0};
  pkt.vx = inputs.lx;
  pkt.vy = inputs.ly;
  pkt.omega = inputs.omega;
  pkt.mask = 0;
  for (int i = 0; i < 10; i++) {
    if (inputs.buttons[i]) pkt.mask |= (1U << i);
  }
  // Add joystick select buttons to mask (bits 10-11)
  if (inputs.joySelect[0]) pkt.mask |= (1U << 10);
  if (inputs.joySelect[1]) pkt.mask |= (1U << 11);

  espNowSender.send(pkt);
  lastSentPacket = pkt;
  lastSendTime = now;
}