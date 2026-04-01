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
  pkt.vx = inputs.vx;
  pkt.vy = inputs.vy;
  pkt.omega = inputs.omega;
  pkt.mask = 0;
  for (int i = 0; i < 8; i++) {
    if (inputs.buttons[i]) pkt.mask |= (1U << i);
  }

  espNowSender.send(pkt);
  lastSentPacket = pkt;
  lastSendTime = now;
}