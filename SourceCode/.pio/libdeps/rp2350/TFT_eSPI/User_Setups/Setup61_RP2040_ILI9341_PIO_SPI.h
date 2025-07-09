// Setup file for RP2040 and SPI ILI9341 display using PIO for the display interface
#define USER_SETUP_ID 61
// The PIO can only be user with Earle Philhower's RP2040 board package:
// https://github.com/earlephilhower/arduino-pico

// PIO SPI allows high SPI clock rates to be used when the processor is over-clocked.
// PIO SPI is "write only" and the TFT_eSPI touch functions are not supported.
// A touch screen could be used with a third party library on different SPI pins.

// This invokes the PIO based SPI interface for the RP2040 processor.
#define RP2040_PIO_SPI

// TFT driver
#define ILI9342_DRIVER
#define TFT_RGB_ORDER TFT_BGR
#define TFT_INVERSION_ON

// Pins - the PIO will control MOSI, SCLK and DC pins
// Any digital GPIO pins may be used
//#define TFT_MISO   0 // MISO is not used or supported
#define TFT_MOSI   3
#define TFT_SCLK   2
#define TFT_CS     1  // Chip select control pin
#define TFT_DC     5  // Data Command control pin
#define TFT_RST    4  // Reset pin (could connect to Arduino RESET pin)

#define TFT_BL   6           // Optional LED back-light control pin
//#define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)

#define RP2040_PIO_CLK_DIV 1
#define RP2040_DMA


#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT

#define SPI_FREQUENCY  100000000
#define SPI_READ_FREQUENCY  10000000
