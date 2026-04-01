// comm.h
#pragma once
#include "inputs.h"
#include "protocol.h"
#include "espnow_sender.h"

extern bool espNowTxOk;
extern ESPNowSender espNowSender;

void comm_setup();
void sendControlPacket();