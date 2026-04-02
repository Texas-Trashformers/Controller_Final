#pragma once
#include <esp_now.h>
#include <WiFi.h>
#include "protocol.h"

static uint8_t chassisMAC[6] = {0xAC, 0xA7, 0x04, 0xE3, 0xBF, 0x34};

static void onSendCb(const uint8_t* mac, esp_now_send_status_t status) {
  // Delivery confirmation — ESP_NOW_SEND_SUCCESS or ESP_NOW_SEND_FAIL
  (void)mac;
  (void)status;
}

class ESPNowSender {
public:
  ESPNowSender() {}

  void begin() {
    WiFi.mode(WIFI_STA);  // Required for ESP-NOW radio — do NOT call WiFi.begin()

    if (esp_now_init() != ESP_OK) {
      Serial.println("[ESP-NOW] Init failed!");
      return;
    }
    esp_now_register_send_cb(onSendCb);

    esp_now_peer_info_t peer = {};
    memcpy(peer.peer_addr, chassisMAC, 6);
    peer.channel = 0;       // Default channel — neither side connects to a router
    peer.encrypt = false;
    esp_now_add_peer(&peer);

    Serial.printf("[ESP-NOW] Ready. Peer: %02X:%02X:%02X:%02X:%02X:%02X\n",
                  chassisMAC[0], chassisMAC[1], chassisMAC[2],
                  chassisMAC[3], chassisMAC[4], chassisMAC[5]);
  }

  void send(const ControlPacket& pkt) {
    esp_now_send(chassisMAC, (const uint8_t*)&pkt, PACKET_SIZE);
  }
};
