// ================================================
// User_Setup.h for Lonely Binary ESP32-S3 + ST7735
// ================================================

#define USER_SETUP_INFO "Lonely Binary S3 + ST7735"
#define DISABLE_ALL_LIBRARY_WARNINGS

// === DRIVER ===
#define ST7735_DRIVER
#define TFT_WIDTH  128
#define TFT_HEIGHT 160

// === PINS (PCB revision) ===
#define TFT_MOSI 41
#define TFT_SCLK 42
#define TFT_CS    6
#define TFT_DC   40
#define TFT_RST  39
#define TFT_MISO -1

#define TFT_BL   -1     // LED+ hardwired to +3.3V on PCB

// === FONTS ===
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define SMOOTH_FONT

// === SPI ===
#define SPI_FREQUENCY  10000000
// USE_HSPI_PORT intentionally omitted for ESP32-S3
