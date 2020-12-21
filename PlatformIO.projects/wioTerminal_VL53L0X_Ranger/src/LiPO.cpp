#include <Arduino.h>
#include <SparkFunBQ27441.h>

#include "TFT_eSPI.h"

extern TFT_eSPI tft;
TFT_eSprite sprLipo = TFT_eSprite(&tft);  // Sprite
#define FF17 &FreeSans9pt7b

const unsigned int BATTERY_CAPACITY = 650;  // Set Wio Terminal Battery's Capacity

void setupBQ27441(void) {
    // Use lipo.begin() to initialize the BQ27441-G1A and confirm that it's
    // connected and communicating.
    if (!lipo.begin())  // begin() will return true if communication is successful
    {
        // If communication fails, print an error message and loop forever.
        Serial.println("Error: Unable to communicate with BQ27441.");
        Serial.println("  Check wiring and try again.");
        Serial.println("  (Battery must be plugged into Battery Babysitter!)");
        tft.setTextColor(TFT_RED);
        tft.setCursor((320 - tft.textWidth("Battery Not Initialised!")) / 2, 120);
        tft.print("Battery Not Initialised!");
        while (1)
            ;
    }
    Serial.println("Connected to BQ27441!");

    // Uset lipo.setCapacity(BATTERY_CAPACITY) to set the design capacity
    // of your battery.
    lipo.setCapacity(BATTERY_CAPACITY);
}

void printBatteryStats() {
    // Read battery stats from the BQ27441-G1A
    unsigned int soc = lipo.soc();                    // Read state-of-charge (%)
    unsigned int volts = lipo.voltage();              // Read battery voltage (mV)
    int current = lipo.current(AVG);                  // Read average current (mA)
    unsigned int fullCapacity = lipo.capacity(FULL);  // Read full capacity (mAh)
    unsigned int capacity = lipo.capacity(REMAIN);    // Read remaining capacity (mAh)
    int power = lipo.power();                         // Read average power draw (mW)
    int health = lipo.soh();                          // Read state-of-health (%)

    // Now print out those values:
    String toPrint = String(soc) + "% | ";
    toPrint += String(volts) + " mV | ";
    toPrint += String(current) + " mA | ";
    toPrint += String(capacity) + " / ";
    toPrint += String(fullCapacity) + " mAh | ";
    toPrint += String(power) + " mW | ";
    toPrint += String(health) + "%";

    Serial.println(toPrint);

    // LCD Graphics
    tft.setTextColor(TFT_BLUE);
    tft.drawRoundRect(10, 10, 300, 220, 10, TFT_BLUE);
    tft.setTextColor(TFT_MAGENTA);
    tft.drawString("State of Chage:", 20, 30);
    tft.drawString("Battery Voltage:", 20, 55);
    tft.drawString("Average Current:", 20, 80);
    tft.drawString("Remain Capacity:", 20, 105);
    tft.drawString("Full Capacity:", 20, 130);
    tft.drawString("Average Power:", 20, 155);
    tft.drawString("State of Health:", 20, 180);

    // Data
    sprLipo.createSprite(80, 170);
    sprLipo.fillSprite(TFT_BLACK);
    sprLipo.setFreeFont(FF17);
    sprLipo.setTextColor(TFT_WHITE);
    sprLipo.drawString(String(soc) + " % ", 0, 0);
    sprLipo.drawString(String(volts) + " mV ", 0, 25);
    sprLipo.drawString(String(current) + " mA ", 0, 50);
    sprLipo.drawString(String(capacity) + " mAh ", 0, 75);
    sprLipo.drawString(String(fullCapacity) + " mAh ", 0, 100);
    sprLipo.drawString(String(power) + " mW ", 0, 125);
    sprLipo.drawString(String(health) + " % ", 0, 150);
    sprLipo.pushSprite(170, 30);
    sprLipo.deleteSprite();
}
