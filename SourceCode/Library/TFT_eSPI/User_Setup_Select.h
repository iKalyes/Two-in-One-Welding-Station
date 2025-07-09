// This header file contains a list of user setup files and defines which one the
// compiler uses when the IDE performs a Verify/Compile or Upload.
//
// Users can create configurations for different boards and TFT displays.
// This makes selecting between hardware setups easy by "uncommenting" one line.

// The advantage of this hardware configuration method is that the examples provided
// with the library should work with immediately without any other changes being
// needed. It also improves the portability of users sketches to other hardware
// configurations and compatible libraries.
//
// Create a shortcut to this file on your desktop to permit quick access for editing.
// Re-compile and upload after making and saving any changes to this file.

// Example User_Setup files are stored in the "User_Setups" folder. These can be used
// unmodified or adapted for a particular hardware configuration.

#ifndef USER_SETUP_LOADED //  Lets PlatformIO users define settings in
                          //  platformio.ini, see notes in "Tools" folder.

///////////////////////////////////////////////////////
//   User configuration selection lines are below    //
///////////////////////////////////////////////////////

// Only ONE line below should be uncommented to define your setup.  Add extra lines and files as needed.

//#include <User_Setup.h>           // Default setup is root library folder
//#include <User_Setups/Setup60_RP2040_ILI9341.h>              // Setup file for RP2040 with SPI ILI9341
#include <User_Setups/Setup61_RP2040_ILI9341_PIO_SPI.h>      // Setup file for RP2040 with PIO SPI ILI9341

#endif // USER_SETUP_LOADED



/////////////////////////////////////////////////////////////////////////////////////
//                                                                                 //
//     DON'T TINKER WITH ANY OF THE FOLLOWING LINES, THESE ADD THE TFT DRIVERS     //
//       AND ESP8266 PIN DEFINITONS, THEY ARE HERE FOR BODMER'S CONVENIENCE!       //
//                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////


// Identical looking TFT displays may have a different colour ordering in the 16-bit colour
#define TFT_BGR 0   // Colour order Blue-Green-Red
#define TFT_RGB 1   // Colour order Red-Green-Blue

// Legacy setup support, RPI_DISPLAY_TYPE replaces RPI_DRIVER
#if defined (RPI_DRIVER)
  #if !defined (RPI_DISPLAY_TYPE)
    #define RPI_DISPLAY_TYPE
  #endif
#endif

// Legacy setup support, RPI_ILI9486_DRIVER form is deprecated
// Instead define RPI_DISPLAY_TYPE and also define driver (e.g. ILI9486_DRIVER) 
#if defined (RPI_ILI9486_DRIVER)
  #if !defined (ILI9486_DRIVER)
    #define ILI9486_DRIVER
  #endif
  #if !defined (RPI_DISPLAY_TYPE)
    #define RPI_DISPLAY_TYPE
  #endif
#endif

// Invoke 18-bit colour for selected displays
#if !defined (RPI_DISPLAY_TYPE) && !defined (TFT_PARALLEL_8_BIT) && !defined (TFT_PARALLEL_16_BIT) && !defined (ESP32_PARALLEL)
  #if defined (ILI9481_DRIVER) || defined (ILI9486_DRIVER) || defined (ILI9488_DRIVER)
    #define SPI_18BIT_DRIVER
  #endif
#endif

// Load the right driver definition - do not tinker here !
#if   defined (ILI9341_DRIVER) || defined(ILI9341_2_DRIVER) || defined (ILI9342_DRIVER)
     #include <TFT_Drivers/ILI9341_Defines.h>
     #define  TFT_DRIVER 0x9341
#elif defined (ST7735_DRIVER)
     #include <TFT_Drivers/ST7735_Defines.h>
     #define  TFT_DRIVER 0x7735
#elif defined (ILI9163_DRIVER)
     #include <TFT_Drivers/ILI9163_Defines.h>
     #define  TFT_DRIVER 0x9163
#elif defined (S6D02A1_DRIVER)
     #include <TFT_Drivers/S6D02A1_Defines.h>
     #define  TFT_DRIVER 0x6D02
#elif defined (ST7796_DRIVER)
      #include "TFT_Drivers/ST7796_Defines.h"
      #define  TFT_DRIVER 0x7796
#elif defined (ILI9486_DRIVER)
     #include <TFT_Drivers/ILI9486_Defines.h>
     #define  TFT_DRIVER 0x9486
#elif defined (ILI9481_DRIVER)
     #include <TFT_Drivers/ILI9481_Defines.h>
     #define  TFT_DRIVER 0x9481
#elif defined (ILI9488_DRIVER)
     #include <TFT_Drivers/ILI9488_Defines.h>
     #define  TFT_DRIVER 0x9488
#elif defined (HX8357D_DRIVER)
     #include "TFT_Drivers/HX8357D_Defines.h"
     #define  TFT_DRIVER 0x8357
#elif defined (EPD_DRIVER)
     #include "TFT_Drivers/EPD_Defines.h"
     #define  TFT_DRIVER 0xE9D
#elif defined (ST7789_DRIVER)
     #include "TFT_Drivers/ST7789_Defines.h"
     #define  TFT_DRIVER 0x7789
#elif defined (R61581_DRIVER)
     #include "TFT_Drivers/R61581_Defines.h"
     #define  TFT_DRIVER 0x6158
#elif defined (ST7789_2_DRIVER)
     #include "TFT_Drivers/ST7789_2_Defines.h"
     #define  TFT_DRIVER 0x778B
#elif defined (RM68140_DRIVER)
     #include "TFT_Drivers/RM68140_Defines.h"
     #define  TFT_DRIVER 0x6814
#elif defined (SSD1351_DRIVER)
     #include "TFT_Drivers/SSD1351_Defines.h"
     #define  TFT_DRIVER 0x1351
#elif defined (SSD1963_480_DRIVER)
     #include "TFT_Drivers/SSD1963_Defines.h"
     #define  TFT_DRIVER 0x1963
#elif defined (SSD1963_800_DRIVER)
     #include "TFT_Drivers/SSD1963_Defines.h"
     #define  TFT_DRIVER 0x1963
#elif defined (SSD1963_800ALT_DRIVER)
     #include "TFT_Drivers/SSD1963_Defines.h"
     #define  TFT_DRIVER 0x1963
#elif defined (SSD1963_800BD_DRIVER)
     #include "TFT_Drivers/SSD1963_Defines.h"
     #define  TFT_DRIVER 0x1963
#elif defined (GC9A01_DRIVER)
     #include "TFT_Drivers/GC9A01_Defines.h"
     #define  TFT_DRIVER 0x9A01
#elif defined (ILI9225_DRIVER)
     #include "TFT_Drivers/ILI9225_Defines.h"
     #define  TFT_DRIVER 0x9225
#elif defined (RM68120_DRIVER)
     #include "TFT_Drivers/RM68120_Defines.h"
     #define  TFT_DRIVER 0x6812
#elif defined (HX8357B_DRIVER)
     #include "TFT_Drivers/HX8357B_Defines.h"
     #define  TFT_DRIVER 0x835B
#elif defined (HX8357C_DRIVER)
     #include "TFT_Drivers/HX8357C_Defines.h"
     #define  TFT_DRIVER 0x835C

                              // <<<<<<<<<<<<<<<<<<<<<<<< ADD NEW DRIVER HERE
                              // XYZZY_init.h and XYZZY_rotation.h must also be added in TFT_eSPI.cpp
#elif defined (XYZZY_DRIVER)
     #include "TFT_Drivers/XYZZY_Defines.h"
     #define  TFT_DRIVER 0x0000
#else
     #define  TFT_DRIVER 0x0000
#endif
