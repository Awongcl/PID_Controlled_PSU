#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <PID_v1.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_INA219.h>
#include <PID_v1.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_INA219 ina219;

// PIN A4 and A5 for I2C
// MCU Pins
int maxVoltage = 15;   //System's max voltage
int maxCurrent = 1;    //System's max current
int setVoltageIn = A0; // Values set from potentiometers 
int setCurrentIn = A1;
int pwmOut = 3;
// Set data
double setVoltage = 0;
double setCurrent = 0;
double pwm = 255; // For slow start
//INA219 data
double realVoltage = 0;
double realCurrent = 0;
double setPwm,realPwm,output;

int Delay= 500;
unsigned long previousMillis = 0; 
unsigned long currentMillis = 0;

//double Kp=200, Ki=8, Kd=1;
//PID myPID(&realVoltage,&setPwm,&setVoltage, Kp, Ki, Kd, DIRECT);

// Electricity On/Off logo
const unsigned char power [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 
	0x00, 0x06, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 
	0x00, 0x78, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x01, 0xff, 0x80, 0x00, 
	0x03, 0xff, 0x80, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 
	0x00, 0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Update set values
void updateSet(){
  // Update setvoltage 0-15V
  int rawVoltage = analogRead(setVoltageIn);
  setVoltage = (1024-rawVoltage)*(maxVoltage/1024.0);
  // Update setcurrent 0-1V
  int rawCurrent = analogRead(setCurrentIn);
  setCurrent = ((1024-rawCurrent)*(maxCurrent/1024.0))*1000; // Convert to mA
}

// update real values from ina219
void updateReal(){
  realVoltage = ina219.getBusVoltage_V();
  realCurrent = ina219.getCurrent_mA();
}

// Prints data on OLED screen
void printData(){
  display.drawLine(50,60,50,28,WHITE);
  display.drawBitmap(100,0,power,25,22,WHITE);
  display.setCursor(10,16); 
  display.print("  S       R"); 
  display.setCursor(0,40);      
  display.print(setVoltage,1);
  display.setCursor(55,40); 
  display.print(realVoltage,1);
  display.print(" V");           
  display.setCursor(0,60);     
  display.print(setCurrent,0);
  display.setCursor(55,60);   
  display.print(realCurrent,1);
  display.print("mA");   
  display.display();
}

// Set fonts for display
void setText(){
  display.setTextColor(WHITE); 
  display.setTextSize(1);   
  display.setFont(&FreeSans9pt7b);  
}





void setup(){
  Serial.begin(9600);
  ina219.begin();
  currentMillis = millis();
  TCCR2B = TCCR2B & (B11111000) | (B00000001);      // pin 3 PWM frequency of 31372.55 Hz
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
    Serial.println(F("SSD1306 allocation failed"));
  pinMode(setVoltage,INPUT);
  pinMode(setCurrent,INPUT);
  pinMode(pwmOut,OUTPUT);
  setText();
}

void loop(){
  display.clearDisplay();
  updateSet();
  updateReal();
  if(realCurrent < setCurrent){
    if(realVoltage < setVoltage){
      pwm -= 1;
      pwm = constrain(pwm, 0, 255);
    }
    else if(realVoltage > setVoltage){
      if(realVoltage - setVoltage > 1.5)
        pwm += 50;
      pwm += 1;
      pwm = constrain(pwm, 0, 255);
    }
  }
  if(realCurrent > setCurrent){
    pwm += 1;
    pwm = constrain(pwm, 0, 255);
  }
 
  /*Serial.print(output);
  Serial.print(" ");
  pwm = map(output,0,15,0,255);
  pwm = constrain(pwm, 0, 255);
  Serial.println(pwm);
  */
  analogWrite(pwmOut,pwm);
  currentMillis = millis();
  if(currentMillis - previousMillis >= Delay){  
    previousMillis += Delay;
    printData(); 
  }
 
}




