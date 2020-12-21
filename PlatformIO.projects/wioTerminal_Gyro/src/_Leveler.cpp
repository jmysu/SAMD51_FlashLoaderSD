#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Wire.h>
extern TFT_eSPI tft;

extern void checkSdLaunch();

#include "LIS3DHTR.h"
//#include <LovyanGFX.hpp>
#include <math.h>
/*
#define TFT_GREENYELLOW 0x38761DU
#define GREEN24 0x00FF00
#define TFT_RED 0xFF0000U
#define TFT_YELLOW 0xFFFF00U
#define TFT_WHITE 0xFFFFFFU
#define TFT_BLACK 0x000000U
*/
LIS3DHTR<TwoWire> lis;
//static LGFX tft;
//static TFT_eSprite canvas(&tft);
static TFT_eSprite sight(&tft);
static TFT_eSprite bottom(&tft);
static TFT_eSprite right(&tft);
static TFT_eSprite bubble(&sight);
static TFT_eSprite idx_X(&tft);
static TFT_eSprite idx_Y(&tft);
static auto transpalette = TFT_TRANSPARENT;


void setup() {
    //Serial.begin(115200);
    //   while (!Serial);
    checkSdLaunch();
    delay(1000);

    pinMode(WIO_5S_UP, INPUT_PULLUP);
    pinMode(WIO_5S_DOWN, INPUT_PULLUP);

    lis.begin(Wire1);

    if (!lis) {
        Serial.println("ERROR");
        while (1)
            ;
    }
    lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ);  //Data output rate
    lis.setFullScaleRange(LIS3DHTR_RANGE_2G);       //Scale range set to 2g

    // Display set up
    //tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    //tft.setBrightness(128);
    //tft.setBacklight(120);
    //keep memory
    //canvas.createSprite(200, 200);
    sight.createSprite(200, 200);
    bottom.createSprite(206, 20);
    right.createSprite(20, 206);
    bubble.createSprite(20, 20);
    idx_X.createSprite(6, 20);
    idx_Y.createSprite(20, 6);

    // create parts
    sight.fillScreen(transpalette);
    bottom.fillScreen(transpalette);
    right.fillScreen(transpalette);
    bubble.fillScreen(transpalette);
    idx_X.fillScreen(transpalette);
    idx_Y.fillScreen(transpalette);

    sight.drawCircle(100, 100, 90, TFT_WHITE);
    sight.drawCircle(100, 100, 60, TFT_WHITE);
    sight.drawCircle(100, 100, 30, TFT_WHITE);
    sight.drawLine(100, 10, 100, 190, TFT_WHITE);
    sight.drawLine(10, 100, 190, 100, TFT_WHITE);
    sight.fillCircle(100, 100, 3, TFT_RED);

    bottom.fillRoundRect(0, 0, 206, 20, 10, TFT_DARKCYAN);
    right.fillRoundRect(0, 0, 20, 206, 10, TFT_DARKCYAN);
    bubble.fillCircle(10, 10, 10, TFT_YELLOW);
    idx_X.fillRect(0, 0, 6, 20, TFT_WHITE);
    idx_X.drawFastVLine(3, 0, 20, TFT_BLACK);
    idx_Y.fillRect(0, 0, 20, 6, TFT_WHITE);
    idx_Y.drawFastHLine(0, 3, 20, TFT_BLACK);

    tft.setTextColor(TFT_YELLOW, TFT_TRANSPARENT);
    tft.setTextSize(1);
    sight.pushSprite(59, 19);

    tft.drawString("Angle", 5, 20);
    tft.drawString("X: ", 10, 40);
    tft.drawString("Y: ", 10, 60);
    tft.drawString("Accel.", 5, 120);
    tft.drawString("X: ", 10, 140);
    tft.drawString("Y: ", 10, 160);
    tft.drawString("Z: ", 10, 180);
    bottom.pushSprite(56, 215);
    right.pushSprite(295, 16);
}

int p_plot_x = 159, p_plot_y = 119;
float px_values = 0.0, py_values = 0.0, pz_values = 0.0;
float px_angle = 0, py_angle = 0;
int scale = 1, p_scale = 5;
;

void loop() {
    float x_values, y_values, z_values, x_angle, y_angle;
    int plot_x, plot_y;

    x_values = lis.getAccelerationX();
    y_values = lis.getAccelerationY();
    z_values = lis.getAccelerationZ();

    x_angle = 180.0 / 3.1415 * asin(x_values);
    y_angle = 180.0 / 3.1415 * asin(y_values);

    plot_x = 100 + scale * (90.0 / 3.1415 * 2 * asin(y_values));
    if (plot_x > 197) plot_x = 197;
    if (plot_x < 3) plot_x = 3;
    plot_y = 100 - scale * (90.0 / 3.1415 * 2 * asin(x_values));
    if (plot_y > 197) plot_y = 197;
    if (plot_y < 3) plot_y = 3;

    //sight.clear();
    sight.fillSprite(TFT_TRANSPARENT);
    sight.drawCircle(100, 100, 90, TFT_WHITE);
    sight.drawCircle(100, 100, 60, TFT_WHITE);
    sight.drawCircle(100, 100, 30, TFT_WHITE);
    sight.drawLine(100, 10, 100, 190, TFT_WHITE);
    sight.drawLine(10, 100, 190, 100, TFT_WHITE);
    sight.fillCircle(100, 100, 5, TFT_RED);
    sight.fillCircle(plot_x, plot_y, 10, TFT_YELLOW);
    sight.pushSprite(59, 19);

    bottom.pushSprite(56, 215);
    right.pushSprite(295, 16);
    idx_X.pushSprite(plot_x - 3 + 60, 215);
    idx_Y.pushSprite(295, plot_y - 3 + 20);

    tft.setTextColor(TFT_BLACK, TFT_TRANSPARENT);
    tft.drawString(String(px_angle), 50, 40);
    tft.drawString(String(py_angle), 50, 60);
    tft.drawString(String(px_values), 50, 140);
    tft.drawString(String(py_values), 50, 160);
    tft.drawString(String(pz_values), 50, 180);
    tft.setTextColor(TFT_WHITE, TFT_TRANSPARENT);
    tft.drawString(String(x_angle), 50, 40);
    tft.drawString(String(y_angle), 50, 60);
    tft.drawString(String(x_values), 50, 140);
    tft.drawString(String(y_values), 50, 160);
    tft.drawString(String(z_values), 50, 180);
    tft.drawString("Scl", 260, 20);
    if (scale != p_scale) {
        tft.fillRect(270, 41, 25, 20, TFT_BLACK);
        tft.drawString(String(scale), 270, 41);
    }

    p_plot_x = plot_x;
    p_plot_y = plot_y;
    px_values = x_values;
    py_values = y_values;
    pz_values = z_values;
    px_angle = x_angle;
    py_angle = y_angle;

    if (digitalRead(WIO_5S_UP) == LOW) {
        scale++;
        if (scale > 5) scale = 5;
        while (digitalRead(WIO_5S_UP) == LOW)
            ;
    }
    if (digitalRead(WIO_5S_DOWN) == LOW) {
        scale--;
        if (scale < 1) scale = 1;
        while (digitalRead(WIO_5S_DOWN) == LOW)
            ;
    }

    delay(50);
}
