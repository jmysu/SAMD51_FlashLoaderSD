/*  
 * MSC SD USB 
 */
#include <Arduino.h>

#include <TFT_eSPI.h>
extern TFT_eSPI tft;

#include "SPI.h"
#include "SdFat.h"
#include "Adafruit_TinyUSB.h"

const int chipSelect = SDCARD_SS_PIN; //Use WioTerminal SDCARD

// File system on SD Card
SdFat sd;

SdFile root;
SdFile file;

// USB Mass Storage object
Adafruit_USBD_MSC usb_msc;

// Set to true when PC write to flash
bool changed;

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and
// return number of copied bytes (must be multiple of block size)
int32_t msc_read_cb(uint32_t lba, void* buffer, uint32_t bufsize) {
    return sd.card()->readBlocks(lba, (uint8_t*)buffer, bufsize / 512) ? bufsize : -1;
}

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and
// return number of written bytes (must be multiple of block size)
int32_t msc_write_cb(uint32_t lba, uint8_t* buffer, uint32_t bufsize) {
    digitalWrite(LED_BUILTIN, HIGH);
    return sd.card()->writeBlocks(lba, buffer, bufsize / 512) ? bufsize : -1;
}

// Callback invoked when WRITE10 command is completed (status received and accepted by host).
// used to flush any pending cache.
void msc_flush_cb(void) {
    sd.card()->syncBlocks();
    // clear file system's cache to force refresh
    sd.cacheClear();
    changed = true;
    digitalWrite(LED_BUILTIN, LOW);
}

int iFiles;
int iDirs;
int32_t iFilesSizes;

//
//Show folder recursively
//
void showFolder(const char* foldername) {
    SdFile dir;
    SdFile file;
    char fn[32];
    int iFnLen;

    if (!dir.open(foldername)) {
        Serial.print(foldername);
        Serial.println(" open folder failed");
    } else {
        //Serial.print(foldername);
        Serial.println("");
        while (file.openNext(&dir, O_RDONLY)) {
            file.getName(fn, 32);
            Serial.print(fn);
            iFnLen = strlen(fn);
            if (file.isDir()) {
                for (int i = 0; i < (32 - iFnLen); i++)
                    Serial.print("-");
                String sFn = foldername;
                sFn += "/";
                sFn += fn;
                showFolder(sFn.c_str());  //Recursive to folder
                iDirs++;
            } else {
                for (int i = 0; i < (32 - iFnLen); i++)
                    Serial.print(" ");
                Serial.println(file.fileSize());
                iFiles++;
                iFilesSizes += file.fileSize();
            }
            file.close();
        }
        dir.close();
    }
    return;
}

extern void checkSdLaunch();
//-----------------------------------------------------------------------------
// the setup function runs once when you press reset or power the board
void setup() {
    checkSdLaunch(); //Check if Wio KEY_A pressed!
  
    pinMode(LED_BUILTIN, OUTPUT);

    // Set disk vendor id, product id and revision with string up to 8, 16, 4 characters respectively
    usb_msc.setID("TinyUSB", "Mass Storage", "1.0");
 
    // Set read write callback
    usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);

    // Still initialize MSC but tell usb stack that MSC is not ready to read/write
    // If we don't initialize, board will be enumerated as CDC only
    usb_msc.setUnitReady(false);
    usb_msc.begin();

    Serial.begin(115200);
    //while (!Serial) delay(10);  // wait for native usb
    delay(1000);

    Serial.println("TinyUSB Mass Storage SDFAT Card");

    Serial.print("\nInitializing SDFAT card... ");
    Serial.print("CS = ");
    Serial.println(chipSelect);

    if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
        Serial.println("initialization failed. Things to check:");
        Serial.println("* is a card inserted?");
        Serial.println("* is your wiring correct?");
        Serial.println("* did you change the chipSelect pin to match your shield or module?");
        while (1) delay(1);
    }

    // Size in blocks (512 bytes)
    uint32_t block_count = sd.card()->cardSize();

    Serial.print("Volume size (MB):  ");
    Serial.println((block_count / 2) / 1024);

    // Set disk size, SD block size is always 512
    usb_msc.setCapacity(block_count, 512);

    // MSC is ready for read/write
    usb_msc.setUnitReady(true);

    changed = true;  // to print contents initially
}

int iSec;  
void loop() {
    if (changed) {
        //sd.ls(LS_SIZE|LS_R);
        iDirs = iFiles = iFilesSizes =0;
        showFolder("/");
        
        tft.setTextSize(2);
        tft.setCursor(40,132);
        tft.printf("Dirs :%3d", iDirs);
        tft.setCursor(40,150);
        tft.printf("Files:%3d, %6ldKB", iFiles, iFilesSizes/1024);

        delay(1000);  // refresh every 0.5 second
        changed = false;
        }
    
    if ((millis()%1000)==0) { iSec++;
        tft.drawFastHLine(40,120,240,TFT_BLACK);
        tft.drawFastHLine(40,120,8*(iSec%30),TFT_GREENYELLOW);
        }    
}