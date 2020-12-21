#include <Arduino.h>
#include <SD/Seeed_SD.h>
#include <FreeStack.h>
#include <LovyanGFX.hpp>
LGFX lcd;

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

/*
void lcdShowFiles(const char *foldername) {
 File dir;
 File file;
 
 //const char foldername[] = "/TFT";

  if (!dir.open(foldername)){
    Serial.println("open folder failed");
    }
  else {
    Serial.print(foldername);
    Serial.println("------------");
    while (file.openNext(&dir, O_RDONLY)) {
        char fn[32];
        file.getName(fn, 32);
        Serial.print(fn);
        int iLen = strlen(fn);
        for (int i=0; i<(32-iLen); i++)
            Serial.print(" ");
        Serial.println(file.fileSize());
       
        if (lcd_type == LCD_FILE_NAME)
            lcd.println(fn);
        else {
            String sF = fn;
            if (sF.endsWith(".jpg"))  {
                lcd.drawJpgFile(SD, filename);
                delay(3000);
                }
        file.close();    
        }
    }
   dir.close();
}
*/
extern bool GetImageSize(File f, int16_t *x, int16_t*y);
extern int gifPlay( char* gifPath );

void showFiles(File dir, int dly)
{
  int16_t w=0,h=0,centerX=0,centerY=0;
  File entry;

  while (true) {
    //Serial.print(F("\t FreeStack: "));
    int iFree = FreeStack();
    //Serial.println(iFree);
    if (iFree < 10000) {
      Serial.println("\nLow freestack reset starting...");
      delay(100);
      NVIC_SystemReset();
      }

    entry =  dir.openNextFile();
    if (! entry) {
        break;
        }
    //Get size from binary
    centerX=0;centerY=0;
    if ( GetImageSize(entry, &w, &h)) {
      if (w < lcd.width()) centerX = (lcd.width()-w) >>1;
      if (h < lcd.height()) centerY = (lcd.height()-h) >>1;  
      Serial.printf("(%4dx%4d) [%6d]", w, h, iFree);
      }

    lcd.clear();
    //GIF------------------------------------------------------------
    if ( strstr(entry.name(), ".gif") ) 
        gifPlay(entry.name());
    //JPG------------------------------------------------------------ 
    else if ( strstr(entry.name(), ".jpg") )  
        lcd.drawJpgFile(SD, entry.name(), centerX, centerY);
    //PNG------------------------------------------------------------      
    else if ( strstr(entry.name(), ".png") )
        lcd.drawPngFile(SD, entry.name(), centerX, centerY);
    else
        Serial.printf("Not a valid file:");
    
    Serial.println(entry.name());
    lcd.setCursor(10,10);
    lcd.setFont(&Font2);
    lcd.print(entry.name());

    entry.close();
    delay(dly);
    }
}

void setup(void)
{
  lcd.init();

  Serial.begin(115200);

  lcd.setColorDepth(24);

  lcd.setColor(TFT_WHITE);
  lcd.startWrite();
  lcd.setAddrWindow(0, 0, lcd.width(), lcd.height());
  for (int y = 0; y < lcd.height(); ++y)
  {
    for (int x = 0; x < lcd.width(); ++x)
    {
      lcd.writeColor( lcd.color888(x << 1, x + y, y << 1), 1);
    }
  }
  lcd.print("JPG test\n");
  lcd.endWrite();

  SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 50000000);
 
  delay(1000);
  /*
  pinMode(WIO_BUZZER, OUTPUT);
  tone(WIO_BUZZER, NOTE_C4, 300);
  tone(WIO_BUZZER, NOTE_E4, 300);
  tone(WIO_BUZZER, NOTE_G4, 300);
  tone(WIO_BUZZER, NOTE_C5, 1000);
  delay(2000);
  noTone(WIO_BUZZER);
  */
}

void loop(void)
{
  
  //lcd.drawJpgFile(SD, filename, random(-20,20), random(-20, 20));
  //lcdShowFiles("/TFT");
  File root = SD.open("/TFT");
  if (root) {
    showFiles(root, 250); //pause 250ms for each file
    root.close();
    }
  else
    Serial.println("root folder open failed!");  
}
