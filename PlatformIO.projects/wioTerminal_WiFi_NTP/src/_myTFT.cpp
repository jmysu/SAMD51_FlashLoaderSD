#include <Arduino.h>
#include <TFT_eSPI.h>
TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);  //sprite
#include <TimeLib.h>

#include "icons.h"

#define NUMFLAKES 200
#define XPOS 0
#define YPOS 1
#define DELTA_Y 2
uint16_t flake[NUMFLAKES][3];

float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;  // Saved H, M, S x & y multipliers
float sdeg = 0, mdeg = 0, hdeg = 0;

uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;

int cx, cy, cy1, r;
uint16_t osx = cx, osy = cy, omx = cx, omy = cy, ohx = cx, ohy = cy;  // Saved H, M, S x & y coords

void drawClockFace() {
    // Draw clock face
    cx = tft.width() / 2;
    cy = tft.height() / 2;
    r = min(cx, cy) - 25;
    cy1 = cy + 25;  //adjust y position w/ header
    for (int i=2;i<8;i++)
        tft.drawCircle(cx, cy1, r - i, TFT_BLUE);
    //tft.fillCircle(cx, cy1, r - 2, TFT_BLUE);
    //tft.fillCircle(cx, cy1, r - 8, TFT_TRANSPARENT);

    // Draw 12 lines
    for (int i = 0; i < 360; i += 30) {
        sx = cos((i - 90) * 0.0174532925);
        sy = sin((i - 90) * 0.0174532925);
        x0 = sx * (r - 6) + cx;
        yy0 = sy * (r - 6) + cy1;
        x1 = sx * (r - 20) + cx;
        yy1 = sy * (r - 20) + cy1;
        tft.drawLine(x0, yy0, x1, yy1, TFT_BLUE);
    }
    // Draw 60 dots
    for (int i = 0; i < 360; i += 6) {
        sx = cos((i - 90) * 0.0174532925);
        sy = sin((i - 90) * 0.0174532925);
        int x2 = sx * (r - 18) + cx;
        int y2 = sy * (r - 18) + cy1;
        // Draw minute markers
        tft.drawPixel(x2, y2, TFT_DARKGREY);
        // Draw main quadrant dots
        if (i == 0 || i == 180) tft.fillCircle(x2, y2, 2, TFT_DARKGREY);
        if (i == 90 || i == 270) tft.fillCircle(x2, y2, 2, TFT_DARKGREY);
    }

    tft.fillCircle(cx, cy1, 3, TFT_LIGHTGREY);
}

uint8_t hh, mm, ss;
bool isInitClock = 1;
void drawClockHands(time_t t) {
    hh = hour(t);
    mm = minute(t);
    ss = second(t);
    if (ss == 60) {
        ss = 0;
        mm++;  // Advance minute
        if (mm > 59) {
            mm = 0;
            hh++;  // Advance hour
            if (hh > 23) {
                hh = 0;
            }
        }
    }

    // Pre-compute hand degrees, x & y coords for a fast screen update
    sdeg = ss * 6;                      // 0-59 -> 0-354
    mdeg = mm * 6 + sdeg * 0.01666667;  // 0-59 -> 0-360 - includes seconds
    hdeg = hh * 30 + mdeg * 0.0833333;  // 0-11 -> 0-360 - includes minutes and seconds
    hx = cos((hdeg - 90) * 0.0174532925);
    hy = sin((hdeg - 90) * 0.0174532925);
    mx = cos((mdeg - 90) * 0.0174532925);
    my = sin((mdeg - 90) * 0.0174532925);
    sx = cos((sdeg - 90) * 0.0174532925);
    sy = sin((sdeg - 90) * 0.0174532925);

    if ((ss == 0) || isInitClock) {
        // Erase hour and minute hand positions every minute
        tft.drawLine(ohx, ohy, cx, cy1, TFT_BLACK);
        ohx = hx * (r - 48) + cx;
        ohy = hy * (r - 48) + cy1;
        tft.drawLine(omx, omy, cx, cy1, TFT_BLACK);
        omx = mx * (r - 36) + cx;
        omy = my * (r - 36) + cy1;
    }

    // Redraw new hand positions, hour and minute hands not erased here to avoid flicker
    tft.drawLine(osx, osy, cx, cy1, TFT_BLACK);
    osx = sx * (r - 20) + cx;
    osy = sy * (r - 20) + cy1;
    tft.drawLine(osx, osy, cx, cy1, TFT_RED);
    tft.drawLine(ohx, ohy, cx, cy1, TFT_WHITE);
    tft.drawLine(omx, omy, cx, cy1, TFT_WHITE);
    tft.drawLine(osx, osy, cx, cy1, TFT_RED);

    tft.fillCircle(cx, cy1, 3, TFT_RED);
}

extern void draw7Number(long n, unsigned int xLoc, unsigned int yLoc, char cS, unsigned int fC, unsigned int bC, char nD);
void drawClockDigits(time_t t) {
    char sBuf[16];
    int ss = second(t);
    if ((ss < 10) || (ss > 50) || isInitClock) {  //Draw Year-Month-Day
        int c = tft.width() / 2;
        draw7Number(8888, c - 60, 100, 1, TFT_BLACK, TFT_BLACK, 4);
        draw7Number(-88, c - 6, 100, 1, TFT_BLACK, TFT_BLACK, 3);
        draw7Number(-88, c + 30, 100, 1, TFT_BLACK, TFT_BLACK, 3);
        draw7Number(year(t), c - 60, 100, 1, TFT_LIGHTGREY, TFT_BLACK, 4);
        draw7Number(month(t) * -1, c - 6, 100, 1, TFT_LIGHTGREY, TFT_BLACK, 3);
        draw7Number(day(t) * -1, c + 30, 100, 1, TFT_LIGHTGREY, TFT_BLACK, 3);
    }
    spr.createSprite(220, 48);
    spr.fillSprite(TFT_BLACK);
    spr.setTextDatum(MC_DATUM);

    sprintf(sBuf, "88:88:88");
    spr.setTextColor(tft.color565(80, 80, 80));  //Dark Grey to erase digits
    spr.drawString(sBuf, 0, 0, 7);               //draw text

    sprintf(sBuf, "%02d:%02d:%02d", hour(t), minute(t), second(t));
    spr.setTextColor(TFT_ORANGE);
    spr.drawString(sBuf, 0, 0, 7);  //draw text

    spr.pushSprite(tft.width() / 2 - 110, tft.height() - 64, TFT_BLACK);
    spr.deleteSprite();
}

void setupTFT() {
    tft.init();
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    tft.setFreeFont(&FreeSansBoldOblique18pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.drawCentreString("NTP Timer", tft.width() / 2, 10, 1);

    //Line
    for (int i = 0; i < 3; i++)
        tft.drawFastHLine(0, 48 + i, tft.width() / (1 << (2 - i)), TFT_DARKGREY);
    // Draw bitmap with top right corner at x,y with foreground only color
    // Bits set to 1 plot as the defined color, bits set to 0 are not plotted
    //              x  y  xbm   xbm width  xbm height  color
    tft.drawXBitmap(tft.width() - 80, 24, WiFiIcon, WIFIICON_WIDTH, WIFIICON_HEIGHT, TFT_DARKGREY);

    isInitClock = 1;
    drawClockFace();
    drawClockDigits(now());
    drawClockHands(now());

    // initialise flakes
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
        flake[f][XPOS] = random(320);
        flake[f][YPOS] = random(50,240);
        flake[f][DELTA_Y] = random(5) + 1;
    }
    Serial.printf("TFT:%.4dx%.4d\n", tft.width(), tft.height());
}

void letItSnow() {
    
    //Erasing flakes, generates new flakes
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
        tft.drawPixel(flake[f][XPOS], flake[f][YPOS], TFT_BLACK);

        flake[f][XPOS] += random(3) - 1;
        flake[f][YPOS] += flake[f][DELTA_Y];

        if (flake[f][YPOS] > tft.height()) {
            flake[f][XPOS] = random(320);
            flake[f][YPOS] = random(50,240);
            flake[f][DELTA_Y] = random(5) + 1;
        }
    }
    //Drawing flakes w/ random white
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
        tft.drawPixel(flake[f][XPOS], flake[f][YPOS], ((random(0,2)==0)?TFT_WHITE:TFT_DARKGREY));
    }
}