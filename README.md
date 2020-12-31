# SAMD51_FlashLoaderSD [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)<br>
### Interesting projects based on extFlashLoader, with more apps added! <br>
You need to flash the loader menu from extFlashLoader examples first, then copy the unzipped /apps into SD /apps!<br>
Press/hold the KEY_A and reset the WioTerminal, the SD-Loader menu should start...<br>
(Works with WioTerminal(SAMD51+ILI9341))<br>
### UF2 firmware update:<br>
Use [bin2uf2.py](https://github.com/jmysu/SAMD51_FlashLoaderSD/blob/main/PlatformIO.projects/bin2uf2.py) (_from Microsoft open source_) to convert SDMENU firmware.bin to sdmenu0x4000.uf2, drag and drop to UF2 bootloader!<br>
<img src="PlatformIO.projects/sdmenu_bin2uf2.jpg" width=800>

<br>
<br>



__SAMD51 Memory Map__ <br>
<img src="pic/SAMD51NVM.png" width=480> <img src="pic/SdLoader2020.gif" width=200><br>
The memory map for different NVM...<br>
<br><br><br>

__Zipped apps__ <br>
The zipped apps binary for the following projects! Unzip this and place /apps into SD /apps<br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/blob/main/PlatformIO.projects/apps.zip<br>

<br>
<br>

__Project SdUSB__ w/ TinyUSB<br>
Project to turn SD card into USB Mass Storage, for updating SD contents! <br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/tree/main/PlatformIO.projects/wioTerminal_SdUSB<br>
<img src="pic/SdLoaderApps/USB0.jpg" width=320> <img src="pic/SdLoaderApps/USB1.jpg" width=320> <br>
<br>
<br>
__Project SdSlideShow__ w/ JPEGDEC<br>
Project to decode/show JPEG/GIF files on SD, put jpeg/gif files into SD /TFT floder! <br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/tree/main/PlatformIO.projects/wioTerminal_JPEGDEC<br>
<img src="pic/SdLoaderApps/Jpeg0.jpg" width=320> <img src="pic/WioTerm1230.gif" width=320> <br>
<br>
<br>
__Project I2C Test__<br>
Project to test I2C0 && I2C1 <br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/tree/main/PlatformIO.projects/wioTerminal_I2C<br>
<img src="pic/SdLoaderApps/I2C0.jpg" width=320> <img src="pic/SdLoaderApps/I2C1.jpg" width=320> <br>
<br>
<br>
__Project Gyro Leveler__<br>
Project for gyro leveler (internal LIS3DHTR) <br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/tree/main/PlatformIO.projects/wioTerminal_Gyro<br>
<img src="pic/SdLoaderApps/Gyro0.jpg" width=320> <img src="pic/SdLoaderApps/Gyro1.jpg" width=320> <br>
<br>
<br>
__Project VcSel Ranger__<br>
Project for VcSel laser ranger (external VL53L0X) <br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/tree/main/PlatformIO.projects/wioTerminal_VL53L0X_Ranger<br>
<img src="pic/SdLoaderApps/Ranger0.jpg" width=320> <img src="pic/SdLoaderApps/Ranger1.jpg" width=320> <br>
<br>
<br>
__Project WiFi Analyzer__<br>
Project to display 2.4G/5G WiFi channels, need latest RPC firmware 2.0.4+ <br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/tree/main/PlatformIO.projects/wioTerminal_WiFiAnalyzer<br>
<img src="pic/SdLoaderApps/WiFiAnalyzer0.jpg" width=320> <img src="pic/SdLoaderApps/WiFiAnalyer1.jpg" width=320> <br>
<br>
<br>
__Project WiFi NTP Clock__<br>
Project to disply a NTP clock, need latest RPC firmware 2.0.4+ <br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/tree/main/PlatformIO.projects/wioTerminal_WiFi_NTP<br>
<img src="pic/SdLoaderApps/NTP0.jpg" width=320> <img src="pic/NTPTimer1225.gif" width=320> <br>
<br>
<br>

## References
  - [SAM BA 3](https://github.com/atmelcorp/sam-ba) Microchip/Atmel SAM boot assistant V3 (Win/Linux cmd line).
  - [bin2uf2](https://github.com/microsoft/uf2-samdx1) USB Mass Storage bootloader (based on UF2) for SAMD21 and SAMD51.
  - [extFlashLoader](https://github.com/ciniml/ExtFlashLoader) WioTerminal extFlashLoader library from SD card.
  - [WioTerminal Flash Loader](https://github.com/ciniml/WioTerminalSimpleLoader) MPLAB Harmony Flash Loader from SD.
  - [LovYanGFX](https://github.com/lovyan03/LovyanGFX) Another nice library for Arduino TFT.
  - [JPEGDEC](https://github.com/bitbank2/JPEGDEC) Another optimized JPEG Decoder for Arduino.
