#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSansBold9pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int maxVoltage = 15;   //System's max voltage
int maxCurrent = 1;    //System's max current
int setVoltageIn = A0;
int setCurrentIn = A1;
int realVolatge = A2;
int realCurrent = A3;
int PWM = A6;

boolean isOn = false;

const unsigned char power [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 
	0x00, 0x06, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 
	0x00, 0x78, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x01, 0xff, 0x80, 0x00, 
	0x03, 0xff, 0x80, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 
	0x00, 0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



float getSetVoltage(){
  int setVoltage = analogRead(setVoltageIn);
  float voltage = setVoltage*(15/1024.0);
  return voltage;
}

void printData(){
  display.setCursor(0,0);
  display.drawBitmap(0,0,power,25,22,WHITE);
  display.setCursor(28,16); 
  display.print("Set     Real"); 
  display.setCursor(0,40);    
  display.print("V:  ");  
  display.println(getSetVoltage());         
  display.setCursor(3,60);    
  display.println("I:"); 
  display.display();
}


void setText(){
  display.setTextColor(WHITE); 
  display.setTextSize(1);   
  display.setFont(&FreeSansBold9pt7b);  
}



void setup(){
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
    Serial.println(F("SSD1306 allocation failed"));
  setText();
}

void loop(){
  // put your main code here, to run repeatedly:
  display.clearDisplay();      
  printData(); 
  
  //Serial.print(value)
  delay(100);
}




