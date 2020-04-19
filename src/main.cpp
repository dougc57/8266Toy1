#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <NeoPixelBus.h>
#include <Ticker.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>    
#include "../include/User_Setup.h"


#define ENCODERA  16
#define ENCODERB  5
#define BUILTIN_LED1 1
#define NEO_PIN 1

#define I2C_CLK 4
#define I2C_DATA 2

#define colorSaturation 64

bool encodera_value = LOW;
bool encoderb_value = LOW;
bool ccw = false;
bool cw = false;
int cwStates[4] = {10, 0, 11, 1};
int ccwStates[4] = {1, 11, 0, 10};
int state;
int priorState = 0;
const int MPU6050_addr=0x68;
float p = 3.1415926;
int16_t AccX,AccY,AccZ,Temp,GyroX,GyroY,GyroZ;
Ticker timerInt;
Ticker displayUpdate;

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);


NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang400KbpsMethod> strip(1, NEO_PIN);
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

void setState() 
{
  priorState = state;
  state = digitalRead(ENCODERA)*10 + digitalRead(ENCODERB);
}
void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}
void printAcc() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.setTextWrap(false);
    tft.setCursor(0, 10);
    tft.println("AccX = ");
    tft.print(AccX);
    tft.setCursor(0, 40);
    tft.println("AccY = ");
    tft.print(AccY);
    tft.setCursor(0, 70);
    tft.println("AccZ = ");
    tft.print(AccZ);
  };
  
  
void setup() {
  // Serial.begin(115200);
  // Serial.println();
  Wire.begin(I2C_DATA, I2C_CLK);
  Wire.beginTransmission(MPU6050_addr);

  Wire.write(0x6B);

  Wire.write(0);

  Wire.endTransmission(true);

  pinMode(BUILTIN_LED1, OUTPUT); // Initialize the BUILTIN_LED1 pin as an output
  // Convert RXD mand TXD to GPIO
  // pinMode(ENCODERA, FUNCTION_3);
  // pinMode(NEO_PIN, FUNCTION_0);

  pinMode(ENCODERA, INPUT);
  pinMode(ENCODERB, INPUT);
  pinMode(NEO_PIN, OUTPUT);
  pinMode(I2C_CLK, OUTPUT);
  pinMode(I2C_DATA, OUTPUT);
  // pinMode(TFT_CS, OUTPUT);
  pinMode(TFT_DC, OUTPUT);
  // int i = 0;
  tft.begin();
  tft.setRotation(2);
  
  
  WiFi.mode(WIFI_STA);
  WiFi.begin("coopersac", "mollysophie916");
  // this resets all the neopixels to an off state
  strip.Begin();
  strip.ClearTo(black);
  strip.Show();
  strip.SetPixelColor(0, blue);
  strip.Show();

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    // Serial.print(".");
    strip.SetPixelColor(0, red);
    strip.Show();
  }
  // Serial.println();
  strip.SetPixelColor(0, green);
  strip.Show();
  timerInt.attach_ms(1, setState);
  displayUpdate.attach(0.5, printAcc);
  
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("Toy");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

   tft.setTextSize(1);

  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_GREEN, TFT_BLACK);



  tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);

  tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 16, 2);

  tft.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 32, 2);

  tft.drawString("abcdefghijklmnopqrstuvw", 0, 48, 2);

  

  // Use this initializer if you're using a 1.8" TFT
  // tft.init();   // initialize a ST7735S chip
  // // large block of text
  tft.fillScreen(TFT_BLACK);
  testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", 0xffff);
  delay(1000);
  tft.fillScreen(TFT_BLACK);

}
  // TFT 7735 rountines

//   void testlines(uint16_t color) {
//   tft.fillScreen(TFT_BLACK);
//   for (int16_t x=0; x < tft.width(); x+=6) {
//     tft.drawLine(0, 0, x, tft.height()-1, color);
//   }
//   for (int16_t y=0; y < tft.height(); y+=6) {
//     tft.drawLine(0, 0, tft.width()-1, y, color);
//   }

//   tft.fillScreen(TFT_BLACK);
//   for (int16_t x=0; x < tft.width(); x+=6) {
//     tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
//   }
//   for (int16_t y=0; y < tft.height(); y+=6) {
//     tft.drawLine(tft.width()-1, 0, 0, y, color);
//   }

//   tft.fillScreen(TFT_BLACK);
//   for (int16_t x=0; x < tft.width(); x+=6) {
//     tft.drawLine(0, tft.height()-1, x, 0, color);
//   }
//   for (int16_t y=0; y < tft.height(); y+=6) {
//     tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
//   }

//   tft.fillScreen(TFT_BLACK);
//   for (int16_t x=0; x < tft.width(); x+=6) {
//     tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
//   }
//   for (int16_t y=0; y < tft.height(); y+=6) {
//     tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
//   }
// }



// void testfastlines(uint16_t color1, uint16_t color2) {
//   tft.fillScreen(TFT_BLACK);
//   for (int16_t y=0; y < tft.height(); y+=5) {
//     tft.drawFastHLine(0, y, tft.width(), color1);
//   }
//   for (int16_t x=0; x < tft.width(); x+=5) {
//     tft.drawFastVLine(x, 0, tft.height(), color2);
//   }
// }

// void testdrawrects(uint16_t color) {
//   tft.fillScreen(TFT_BLACK);
//   for (int16_t x=0; x < tft.width(); x+=6) {
//     tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
//   }
// }

// void testfillrects(uint16_t color1, uint16_t color2) {
//   tft.fillScreen(TFT_BLACK);
//   for (int16_t x=tft.width()-1; x > 6; x-=6) {
//     tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
//     tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
//   }
// }

// void testfillcircles(uint8_t radius, uint16_t color) {
//   for (int16_t x=radius; x < tft.width(); x+=radius*2) {
//     for (int16_t y=radius; y < tft.height(); y+=radius*2) {
//       tft.fillCircle(x, y, radius, color);
//     }
//   }
// }

// void testdrawcircles(uint8_t radius, uint16_t color) {
//   for (int16_t x=0; x < tft.width()+radius; x+=radius*2) {
//     for (int16_t y=0; y < tft.height()+radius; y+=radius*2) {
//       tft.drawCircle(x, y, radius, color);
//     }
//   }
// }

// void testtriangles() {
//   tft.fillScreen(TFT_BLACK);
//   int color = 0xF800;
//   int t;
//   int w = tft.width()/2;
//   int x = tft.height()-1;
//   int y = 0;
//   int z = tft.width();
//   for(t = 0 ; t <= 15; t+=1) {
//     tft.drawTriangle(w, y, y, x, z, x, color);
//     x-=4;
//     y+=4;
//     z-=4;
//     color+=100;
//   }
// }

// void testroundrects() {
//   tft.fillScreen(TFT_BLACK);
//   int color = 100;
//   int i;
//   int t;
//   for(t = 0 ; t <= 4; t+=1) {
//     int x = 0;
//     int y = 0;
//     int w = tft.width()-2;
//     int h = tft.height()-2;
//     for(i = 0 ; i <= 16; i+=1) {
//       tft.drawRoundRect(x, y, w, h, 5, color);
//       x+=2;
//       y+=3;
//       w-=4;
//       h-=6;
//       color+=1100;
//     }
//     color+=100;
//   }
// }

// void tftPrintTest() {
//   tft.setTextWrap(false);
//   tft.fillScreen(TFT_BLACK);
//   tft.setCursor(0, 30);
//   tft.setTextColor(TFT_RED);
//   tft.setTextSize(1);
//   tft.println("Hello World!");
//   tft.setTextColor(TFT_YELLOW);
//   tft.setTextSize(2);
//   tft.println("Hello World!");
//   tft.setTextColor(TFT_GREEN);
//   tft.setTextSize(3);
//   tft.println("Hello World!");
//   tft.setTextColor(TFT_BLUE);
//   tft.setTextSize(4);
//   tft.print(1234.567);
//   delay(1500);
//   tft.setCursor(0, 0);
//   tft.fillScreen(TFT_BLACK);
//   tft.setTextColor(TFT_WHITE);
//   tft.setTextSize(0);
//   tft.println("Hello World!");
//   tft.setTextSize(1);
//   tft.setTextColor(TFT_GREEN);
//   tft.print(p, 6);
//   tft.println(" Want pi?");
//   tft.println(" ");
//   tft.print(8675309, HEX); // print 8,675,309 out in HEX!
//   tft.println(" Print HEX!");
//   tft.println(" ");
//   tft.setTextColor(TFT_WHITE);
//   tft.println("Sketch has been");
//   tft.println("running for: ");
//   tft.setTextColor(TFT_MAGENTA);
//   tft.print(millis() / 1000);
//   tft.setTextColor(TFT_WHITE);
//   tft.print(" seconds.");
// }

// void mediabuttons() {
//   // play
//   tft.fillScreen(TFT_BLACK);
//   tft.fillRoundRect(25, 10, 78, 60, 8, TFT_WHITE);
//   tft.fillTriangle(42, 20, 42, 60, 90, 40, TFT_RED);
//   delay(500);
//   // pause
//   tft.fillRoundRect(25, 90, 78, 60, 8, TFT_WHITE);
//   tft.fillRoundRect(39, 98, 20, 45, 5, TFT_GREEN);
//   tft.fillRoundRect(69, 98, 20, 45, 5, TFT_GREEN);
//   delay(500);
//   // play color
//   tft.fillTriangle(42, 20, 42, 60, 90, 40, TFT_BLUE);
//   delay(50);
//   // pause color
//   tft.fillRoundRect(39, 98, 20, 45, 5, TFT_RED);
//   tft.fillRoundRect(69, 98, 20, 45, 5, TFT_RED);
//   // play color
//   tft.fillTriangle(42, 20, 42, 60, 90, 40, TFT_GREEN);
// }
// end TFT routines
  void setCW() 
  {
    strip.SetPixelColor(0, green);
    strip.Show();
    cw = true;
  }
  void setCCW() 
  {
    strip.SetPixelColor(0, red);
    strip.Show();
    cw = false;
  }

void loop() {
  delay(1);
  Wire.beginTransmission(MPU6050_addr);

  Wire.write(0x3B);

  Wire.endTransmission(false);

  Wire.requestFrom(MPU6050_addr,14,true);

  AccX=Wire.read()<<8|Wire.read();

  AccY=Wire.read()<<8|Wire.read();

  AccZ=Wire.read()<<8|Wire.read();

  Temp=Wire.read()<<8|Wire.read();

  GyroX=Wire.read()<<8|Wire.read();

  GyroY=Wire.read()<<8|Wire.read();

  GyroZ=Wire.read()<<8|Wire.read();

  // ArduinoOTA.handle();
  // encoder phases are normally
  //  when triggered they go low momentarily - the rc determines recovery time
  switch (state) {
      case 10:
        if (priorState == 0) {
          setCW();
        } else {
          if (priorState == 11) {
            setCCW();
          }
        }
      break;
      case 0:
        if (priorState == 1) {
          setCW();
        } else {
          if (priorState == 10) {
            setCCW();
          }
        }
        break;
      case 11:
        if (priorState == 1) {
          setCCW();
        } else {
          if (priorState == 10) {
            setCW();
          }
        }
        break;
      case 1:
        if (priorState == 0) {
          setCCW();
        } else {
          if (priorState == 11) {
            setCW();
          }
        }
        break;
      default:
        // Serial.print("ERROR: default case");
        break;
    }
  
}