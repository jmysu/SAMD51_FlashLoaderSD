/*
 * SD Launch App Template
 *
 */
#include <Arduino.h>

#include <TFT_eSPI.h>
#include <cstdint>
#include <ExtFlashLoader.h>

TFT_eSPI tft;

void checkSdLaunch() {
    //Init TFT Display
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(0);
    //Check KEY_A pressed?
    pinMode(WIO_KEY_A, INPUT_PULLUP);
    if( digitalRead(WIO_KEY_A) == LOW) {
      tft.printf("Launching QSPI application\r\n");
      ExtFlashLoader::ExtFlashLoader loader;
      }

    //-------------------------------------------------------------------------
    tft.setCursor(40,100);
    tft.setTextSize(2);
    tft.printf("USB SDCARD Runing...\r\n");

    //--------------------------------------------------------------------------
}

void updateSdInfo()
{
  
}