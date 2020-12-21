/*
 * Wio WiFi Analyzer
 * Require Wio Terminal rpcWiFi@2.0.4
 * 
 * Adapted from https://github.com/moononournation/Arduino_GFX/blob/master/examples/WioWiFiAnalyzer/WioWiFiAnalyzer.ino
 * 
 */
#include <Arduino.h>
#include <TFT_eSPI.h>
extern TFT_eSPI tft;

#include "rpcWiFi.h"

#define SCAN_INTERVAL 1000

int16_t w, h, text_size, banner_height, graph24_baseline, graph50_baseline, graph_baseline, graph_height, channel24_width, channel50_width, channel_width, signal_width;

// RSSI RANGE
#define RSSI_CEILING -40
#define RSSI_FLOOR -100

#define RED TFT_RED
#define ORANGE TFT_ORANGE
#define YELLOW TFT_YELLOW
#define GREEN TFT_GREEN
#define CYAN TFT_CYAN
#define BLUE TFT_BLUE
#define MAGENTA TFT_MAGENTA
#define WHITE TFT_WHITE
#define BLACK TFT_BLACK

// Channel legend mapping
uint16_t channel_legend[] = {
    1, 2, 3, 4, 5, 6, 7,       //  1,  2,  3,  4,  5,  6,  7,
    8, 9, 10, 11, 12, 13, 14,  //  8,  9, 10, 11, 12, 13, 14,
    32, 0, 0, 0, 40, 0, 0,     // 32, 34, 36, 38, 40, 42, 44,
    0, 48, 0, 0, 0, 56, 0,     // 46, 48, 50, 52, 54, 56, 58,
    0, 0, 64, 0, 0, 0,         // 60, 62, 64, 68,N/A, 96,
    100, 0, 0, 0, 108, 0, 0,   //100,102,104,106,108,110,112,
    0, 116, 0, 0, 0, 124, 0,   //114,116,118,120,122,124,126,
    0, 0, 132, 0, 0, 0, 140,   //128,N/A,132,134,136,138,140,
    0, 0, 0, 149, 0, 0, 0,     //142,144,N/A,149,151,153,155,
    157, 0, 0, 0, 165, 0, 0,   //157,159,161,163,165,167,169,
    0, 173};                   //171,173

// Channel color mapping
uint16_t channel_color[] = {
    RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA,
    RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA,
    RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA,
    RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA,
    RED, ORANGE, YELLOW, GREEN, WHITE, MAGENTA,
    RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA,
    RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA,
    RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA,
    RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA,
    RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA,
    RED, ORANGE};

uint8_t scan_count = 0;

uint16_t channelIdx(int channel) {
    if (channel <= 14)  // 2.4 GHz, channel 1-14
    {
        return channel - 1;
    }
    if (channel <= 64)  // 5 GHz, channel 32 - 64
    {
        return 14 + ((channel - 32) / 2);
    }
    if (channel == 68) {
        return 31;
    }
    if (channel == 96) {
        return 33;
    }
    if (channel <= 144) {
        return 34 + ((channel - 100) / 2);  // channe;
    }
    return 58 + ((channel - 149) / 2);
}

extern void checkSdLaunch();
void setup() {
    checkSdLaunch();
    delay(1000);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    // init LCD

    w = tft.width();
    h = tft.height();
    text_size = (h < 200) ? 1 : 2;
    banner_height = (text_size * 8) + 4;
    graph_height = ((tft.height() - banner_height) / 2) - 30;
    graph24_baseline = banner_height + graph_height + 10;
    graph50_baseline = graph24_baseline + graph_height + 30;
    channel24_width = w / 17;
    channel50_width = w / 62;
    /*
    // init banner 
    tft.setTextSize(text_size);
    tft.fillScreen(BLACK);
    tft.setTextColor(BLUE);
    tft.setCursor(50, 2);
    tft.print("Wio");
    tft.setTextColor(WHITE);
    tft.print(" rpcWiFi ");
    tft.setTextColor(TFT_DARKGREY);
    tft.print(rpc_system_version());*/

    tft.setTextColor(TFT_LIGHTGREY);
    tft.setTextSize(2);
    tft.drawString("WioTerm rpcWiFi", 40, 0, 1);
    tft.drawString(rpc_system_version(), 240, 0, 1);
    tft.drawFastHLine(0, 16, 320, TFT_DARKGREY);
    tft.setTextSize(1);
}

void loop() {
    uint8_t ap_count_list[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int32_t peak_list[] = {RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR, RSSI_FLOOR};
    int16_t peak_id_list[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int32_t channel;
    uint16_t idx;
    int32_t rssi;
    String ssid;
    uint16_t color;
    int16_t height, offset, text_width;

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks(false /* async */, true /* show_hidden */, true /* passive */, 500 /* max_ms_per_chan */);

    // clear old graph
    tft.fillRect(0, banner_height, w, h - banner_height, BLACK);
    tft.setTextSize(1);

    if (n == 0) {
        tft.setTextColor(WHITE);
        tft.setCursor(0, banner_height);
        tft.println("No networks found");
    } else {
        for (int i = 0; i < n; i++) {
            channel = WiFi.channel(i);
            idx = channelIdx(channel);
            rssi = WiFi.RSSI(i);

            // channel peak stat
            if (peak_list[idx] < rssi) {
                peak_list[idx] = rssi;
                peak_id_list[idx] = i;
            }

            ap_count_list[idx]++;
        }

        // plot found WiFi info
        for (int i = 0; i < n; i++) {
            channel = WiFi.channel(i);
            idx = channelIdx(channel);
            rssi = WiFi.RSSI(i);
            color = channel_color[idx];
            height = constrain(map(rssi, RSSI_FLOOR, RSSI_CEILING, 1, graph_height), 1, graph_height);
            if (idx < 14) {
                graph_baseline = graph24_baseline;
                channel_width = channel24_width;
                signal_width = channel24_width * 2;
                offset = (idx + 2) * channel24_width;
            } else {
                graph_baseline = graph50_baseline;
                channel_width = channel50_width;
                signal_width = channel50_width * 2;
                offset = (idx - 14 + 2) * channel50_width;
            }

            // trim rssi with RSSI_FLOOR
            if (rssi < RSSI_FLOOR) {
                rssi = RSSI_FLOOR;
            }

            // plot chart
            tft.startWrite();
            //tft.drawEllipseHelper(offset, graph_baseline + 1, signal_width, height, 0b0011, color); //Adafruit_GFX
            tft.drawLine(offset, graph_baseline - height, offset - signal_width, graph_baseline + 1, color);
            tft.drawLine(offset, graph_baseline - height, offset + signal_width, graph_baseline + 1, color);
            tft.endWrite();

            if (i == peak_id_list[idx]) {
                // Print SSID, signal strengh and if not encrypted
                String ssid = WiFi.SSID(i);
                if (ssid.length() == 0) {
                    ssid = WiFi.BSSIDstr(i);
                }
                text_width = (ssid.length() + 6) * 6;
                if (text_width > w) {
                    offset = 0;
                } else {
                    if ((offset + text_width) > w) {
                        offset = w - text_width;
                    }
                }
                tft.setTextColor(color);
                tft.setCursor(offset, graph_baseline - 10 - height);
                tft.print(ssid);
                tft.print('(');
                tft.print(rssi);
                tft.print(')');
                if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) {
                    tft.print('*');
                }
            }
        }
    }

    // print WiFi found
    tft.setTextColor(WHITE);
    tft.setCursor(2, banner_height);
    tft.print(n);
    tft.print(" networks");

    // draw 2.4 GHz graph base axle
    tft.drawFastHLine(0, graph24_baseline, 320, WHITE);
    for (idx = 0; idx < 14; idx++) {
        channel = channel_legend[idx];
        offset = (idx + 2) * channel24_width;
        if (channel > 0) {
            tft.setTextColor(channel_color[idx]);
            tft.setCursor(offset - ((channel < 10) ? 3 : 6), graph24_baseline + 2);
            tft.print(channel);
        }
        if (ap_count_list[idx] > 0) {
            tft.setTextColor(TFT_DARKGREY);
            tft.setCursor(offset - ((ap_count_list[idx] < 10) ? 3 : 6), graph24_baseline + 8 + 2);
            tft.print(ap_count_list[idx]);
        }
    }

    // draw 5 GHz graph base axle
    tft.drawFastHLine(0, graph50_baseline, 320, WHITE);
    for (idx = 14; idx < 71; idx++) {
        channel = channel_legend[idx];
        offset = (idx - 14 + 2) * channel50_width;
        if (channel > 0) {
            tft.setTextColor(channel_color[idx]);
            tft.setCursor(offset - ((channel < 100) ? 6 : 9), graph50_baseline + 2);
            tft.print(channel);
        }
        if (ap_count_list[idx] > 0) {
            tft.setTextColor(TFT_DARKGREY);
            tft.setCursor(offset - ((ap_count_list[idx] < 10) ? 3 : 6), graph50_baseline + 8 + 2);
            tft.print(ap_count_list[idx]);
        }
    }

    // Wait a bit before scanning again
    delay(SCAN_INTERVAL);

#if defined(SCAN_COUNT_SLEEP)
    //POWER SAVING
    if (++scan_count >= SCAN_COUNT_SLEEP) {
#if defined(LCD_PWR_PIN)
        pinMode(LCD_PWR_PIN, INPUT);  // disable pin
#endif
#if defined(TFT_BL)
        pinMode(TFT_BL, INPUT);  // disable pin
#endif
    }
#endif  // defined(SCAN_COUNT_SLEEP)
}
