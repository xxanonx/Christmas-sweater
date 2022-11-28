#include <LittleFS.h>
/*
 * EEPROM will include:
 * first byte, are settings user defined(1) or default(0) When doing a setting reset all of EEPROM will be set to 0
 * all text will be defined by a char 2(start of txt) at the begining and a 3(end of txt) at the end.
 * 
 * texts:
 *  1st: user ssid 
 *  2nd: user password(none if nothing inside 2&3)
 *  3rd: user lighting pattern  (fade up/down on red from 0 to 120= fud(r, 0-120, duration))
 *                              (strobe blue and red = stb(br(inverse maybe ! inbetween), speed, duration))
 *                              (turn on yellow = trn(y,1))
 *                              (wait for 10 secs = wt(10))
 *                              (play song = pla(track number)) (maybe can play random song or play randomly)
 *  4th: saved manual settings (red at 255, blue at 0, green at 125 = r255,b0,g125)
 */

 
//DEFAULT SETTINGS
//const char default_ssid[] PROGMEM = "ESP-test";
//const char default_password[] PROGMEM = "123456789";


//PERMINANT DEFAULT
/*number of LED's (default = )
 *speaker enabled(1) or not(0) (default = )
 *pattern of LED's (rgb, ry, or brg) (default = )
 */
//change setting acording to sweater
const int num_of_LED PROGMEM = 1;
const bool isSpeaker PROGMEM = false;
const char LED_pattern[] PROGMEM = "b";

//functions
//String ssid = String(default_ssid);
//String password = String(default_password);
