#include <Arduino.h>
#include <TFT_eSPI.h>
extern TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);  //sprite

#include <Seeed_vl53l0x.h>
Seeed_vl53l0x VL53L0X;

#include "LIS3DHTR.h"
LIS3DHTR<TwoWire> LIS;

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
#define SERIAL SerialUSB
#else
#define SERIAL Serial
#endif

void setupTFT() {
    //tft.begin();
    //tft.setRotation(3);

    //Head
    tft.fillScreen(TFT_BLACK);
    tft.setFreeFont(&FreeSansBoldOblique18pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.drawString("Vcsel Ranger", 50, 10, 1);

    //Line
    for (int8_t line_index = 0; line_index < 5; line_index++) {
        tft.drawLine(0, 50 + line_index, tft.width(), 50 + line_index, TFT_GREENYELLOW);
    }

    //Range Rect
    tft.drawRoundRect(5, 60, ((tft.width()*3)/4) - 20, tft.height() - 65, 10, TFT_WHITE);  // L1

    //VCO Text
    tft.setFreeFont(&FreeSansBoldOblique12pt7b);
    tft.setTextColor(TFT_RED);
    tft.drawString("Meter", 7, 65, 1);
    tft.setTextColor(TFT_GREEN);
    tft.drawString("mm", 155, 108, 1);

    //CO Text
    tft.setFreeFont(&FreeSansBoldOblique12pt7b);
    tft.setTextColor(TFT_RED);
    tft.drawString("Inch", 7, 150, 1);
    tft.setTextColor(TFT_GREEN);
    tft.drawString("\"", 155, 193, 1);

    // Temp rect
    tft.drawRoundRect( (((tft.width()*3) /4)) - 5, 60, (tft.width() / 4), (tft.height() - 65) / 2, 10, TFT_BLUE);  // s2

    tft.setFreeFont(&FreeSansBoldOblique9pt7b);
    tft.setTextColor(TFT_RED);
    tft.drawString("Temp", (tft.width()*3 / 4) - 1, 70, 1);  // Print the test text in the custom font
    tft.setTextColor(TFT_GREEN);
    tft.drawString("o", ((tft.width()*3) / 4) + 30, 95, 1);
    tft.drawString("C", ((tft.width()*3) / 4) + 40, 100, 1);

  }

extern void checkSdLaunch();
extern void setupBQ27441();

void setup() {
    checkSdLaunch();

    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    SERIAL.begin(115200);
    delay(1000);

    Status = VL53L0X.VL53L0X_common_init();
    if (VL53L0X_ERROR_NONE != Status) {
        SERIAL.println("start vl53l0x mesurement failed!");
        VL53L0X.print_pal_error(Status);
        while (1);
    }

    VL53L0X.VL53L0X_high_accuracy_ranging_init();
    if (VL53L0X_ERROR_NONE != Status) {
        SERIAL.println("start vl53l0x mesurement failed!");
        VL53L0X.print_pal_error(Status);
        while (1);
        }

    //Init Gyro
    LIS.begin(Wire1); //IIC0 init
    LIS.openTemp();////If ADC3 is used, the temperature detection needs to be turned off.
    delay(100);
    LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
    if (!LIS) {
        Serial.println("LIS3DHTR didn't connect.");
        while (1);
        return;
    }

    setupTFT();    
    //setupBQ27441();
}

VL53L0X_RangingMeasurementData_t RangingMeasurementData;
extern void printBatteryStats();

void loopTFT() {
  //Ranger
  float r = RangingMeasurementData.RangeMilliMeter;
  spr.createSprite(72, 30);
  spr.setFreeFont(&FreeSansBoldOblique12pt7b);
  spr.setTextColor(TFT_WHITE);
  spr.drawNumber(r, 0, 0, 1);
  spr.setTextColor(TFT_GREEN);
  spr.pushSprite(((tft.width()*1)/4) - 1, 108);
  spr.deleteSprite();
  //draw inch
  spr.createSprite(72, 30);
  spr.setTextColor(TFT_WHITE);
  spr.drawFloat(r*0.0393701, 2, 0, 0, 1);
  spr.setTextColor(TFT_GREEN);
  spr.pushSprite(((tft.width()*1)/4) - 1, 193);
  spr.deleteSprite();

  //float t = dht.readTemperature();
  float t = LIS.getTemperature();
  //int tem = round(t);
  int tem = t;
  Serial.print("Temperature: ");
  Serial.print(tem);
  Serial.println( "*C");

  spr.createSprite(30, 30);
  spr.setFreeFont(&FreeSansBoldOblique12pt7b);
  spr.setTextColor(TFT_WHITE);
  spr.drawNumber(t, 0, 0, 1);
  spr.setTextColor(TFT_GREEN);
  spr.pushSprite(((tft.width()*3)/4) - 1, 100);
  spr.deleteSprite();


  //printBatteryStats();
}

void loop() {
    //VL53L0X_RangingMeasurementData_t RangingMeasurementData;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    memset(&RangingMeasurementData, 0, sizeof(VL53L0X_RangingMeasurementData_t));
    Status = VL53L0X.PerformSingleRangingMeasurement(&RangingMeasurementData);
    if (VL53L0X_ERROR_NONE == Status) {
        if (RangingMeasurementData.RangeMilliMeter >= 2000) {
            SERIAL.println("out of range!!");
        } else {
            SERIAL.print("Measured distance:");
            SERIAL.print(RangingMeasurementData.RangeMilliMeter);
            SERIAL.println(" mm");
        }
    } else {
        SERIAL.print("mesurement failed !! Status code =");
        SERIAL.println(Status);
    }

    loopTFT();
    delay(300);
}
