# SAMD51_FlashLoaderSD [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)<br>
### Interesting projects based on extFlashLoader, with more apps added! <br>
You need to flash the loader menu from extFlashLoader examples first, then copy the unzipped /apps into SD /apps!<br>
Press/hold the KEY_A and reset the WioTerminal, the SD-Loader menu should start...<br>
(Works with WioTerminal(SAMD51+ILI9341))<br>
<br>


<br>
<br>



__SAMD51 Memory Map__ <br>
<img src="pic/SAMD51NVM.png" width=480><br>
The memory map for different NVM...<br>
<br><br><br>

__Zipped apps__ <br>
The zipped apps binary for the following projects! Unzip this and place /apps into SD /apps<br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/blob/main/PlatformIO.projects/wioTerminal__SdLauncher_Apps.zip
<br>
<br>

__Project SdUSB__ w/ TinyUSB<br>
Project to turn SD card into USB Mass Storage, for updating SD contents! <br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/tree/main/PlatformIO.projects/wioTerminal_SdUSB<br>
<br>
<br>
__Project SdSlideShow__ w/ JPEGDEC<br>
Project to decode/show JPEG files on SD, put jpeg files into SD /TFT floder! <br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/tree/main/PlatformIO.projects/wioTerminal_JPEGDEC<br>
<br>
<br>
__Project I2C Test__<br>
Project to test I2C0 && I2C1 <br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/tree/main/PlatformIO.projects/wioTerminal_I2C<br>
<br>
<br>
__Project Gyro Leveler__<br>
Project for gyro leveler (internal LIS3DHTR) <br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/tree/main/PlatformIO.projects/wioTerminal_Gyro<br>
<br>
<br>
__Project VcSel Ranger__<br>
Project for VcSel laser ranger (external VL53L0X) <br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/tree/main/PlatformIO.projects/wioTerminal_VL53L0X_Ranger<br>
<br>
<br>
__Project WiFi Analyzer__<br>
Project to display 2.4G/5G WiFi channels, need latest RPC firmware 2.0.4+ <br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/tree/main/PlatformIO.projects/wioTerminal_WiFiAnalyzer<br>
<br>
<br>
__Project WiFi NTP Clock__<br>
Project to disply a NTP clock, need latest RPC firmware 2.0.4+ <br>
https://github.com/jmysu/SAMD51_FlashLoaderSD/tree/main/PlatformIO.projects/wioTerminal_WiFi_NTP<br>
<br>
<br>

## References
  - [extFlashLoader](https://github.com/ciniml/ExtFlashLoader) WioTerminal extFlashLoader library from SD card.
  - [WioTerminal Flash Loader](https://github.com/ciniml/WioTerminalSimpleLoader) MPLAB Harmony Flash Loader from SD.
  - [LovYanGFX](https://github.com/lovyan03/LovyanGFX) Another nice library for Arduino TFT.
  - [JPEGDEC](https://github.com/bitbank2/JPEGDEC) Another optimized JPEG Decoder for Arduino.
