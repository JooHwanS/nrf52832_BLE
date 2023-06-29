#include <Arduino.h>
#line 1 "C:\\Users\\JH\\Desktop\\yeihwan\\bleuart2\\bleuart2.ino"
#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

// BLE Service
//BLEDfu  bledfu;  // OTA DFU service
//BLEDis  bledis;  // device information
BLEUart bleuart; // uart over ble
//BLEBas  blebas;  // battery

#line 11 "C:\\Users\\JH\\Desktop\\yeihwan\\bleuart2\\bleuart2.ino"
void setup();
#line 28 "C:\\Users\\JH\\Desktop\\yeihwan\\bleuart2\\bleuart2.ino"
void startAdv(void);
#line 58 "C:\\Users\\JH\\Desktop\\yeihwan\\bleuart2\\bleuart2.ino"
void loop();
#line 11 "C:\\Users\\JH\\Desktop\\yeihwan\\bleuart2\\bleuart2.ino"
void setup()
{
  Serial.begin(57600);

#if CFG_DEBUG
  // Blocking wait for connection when debug mode is enabled via IDE
  while ( !Serial ) yield();
#endif

  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.begin();
  Bluefruit.setTxPower(4);
  bleuart.begin();
  startAdv();

}

void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

int num;

void loop()
{
  num=analogRead(A4); //read strain sensor data
  bleuart.write(num); //sending data wirelessly
  delay(100); // 10Hz sampling rate, can be changed
}

