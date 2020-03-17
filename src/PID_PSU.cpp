#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PID_v1.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// PIN A4 and A5 for I2C

int setPwm = 0;
int maxVoltage = 15;   //System's max voltage
int maxCurrent = 1;    //System's max current
int setVoltageIn = A0; // Values set from potentiometers 
int setCurrentIn = A1;
int realVolatgeIn = A2; // Voltage and current detected from pin and max471
int realCurrentIn = A3;
int PWM = 3;
int rawCurrent = 0;

boolean isOn = false;

// Electricity On/Off logo
const unsigned char power [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 
	0x00, 0x06, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 
	0x00, 0x78, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x01, 0xff, 0x80, 0x00, 
	0x03, 0xff, 0x80, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 
	0x00, 0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

float getSetPwm(){
  int pwmtest = analogRead(setVoltageIn);
  return pwmtest*(302/1024.0);
}
// Return Scaled voltage, max = 15V
float getSetVoltage(){
  int setVoltage = analogRead(setVoltageIn);
  return setVoltage*(maxVoltage/1024.0);
}
// Return Scaled current, max = 1A
float getSetCurrent(){
  int setCurrent = analogRead(setCurrentIn);
  return setCurrent*(maxCurrent/1024.0);
}
// Get feedback voltage from voltage divider
float getRealVolatge(){
  int rawVoltage = analogRead(realVolatgeIn);
  return rawVoltage*(maxVoltage/1024.0);  
}
//Get feedback current from max471
float getRealCurernt(){
  int rawCurrent = analogRead(realCurrentIn);
  //Serial.println(rawCurrent*(5.0/1024.0),3);
  return rawCurrent*(5.0/1024.0);
}
// Prints data on OLED screen
void printData(){
  display.drawLine(86,60,86,28,WHITE);
  display.drawBitmap(10,0,power,25,22,WHITE);
  display.setCursor(55,16); 
  display.print("V       I"); 
  display.setCursor(0,40);    
  display.print("Set  : ");  
  display.print(getSetVoltage());
  display.print("  " );
  display.print(getRealCurernt());           
  display.setCursor(0,60);    
  display.println("Real:"); 
  display.display();
}


void setText(){
  display.setTextColor(WHITE); 
  display.setTextSize(1);   
  display.setFont(&FreeSansBold9pt7b);  
}



void setup(){
  Serial.begin(115200);
  TCCR2B = TCCR2B & (B11111000) | (B00000001);      // pin 3 PWM frequency of 31372.55 Hz
  //if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  //  Serial.println(F("SSD1306 allocation failed"));
  pinMode(setVoltageIn,INPUT);
  pinMode(setCurrentIn,INPUT);
  pinMode(realVolatgeIn,INPUT);
  pinMode(realCurrentIn,INPUT);
  pinMode(PWM,OUTPUT);
  setText();
}

void loop(){
  // put your main code here, to run repeatedly:
  //display.clearDisplay();      
  //printData(); 
  setPwm = getSetPwm();
  setPwm = constrain(setPwm,0,255);
  Serial.println(setPwm);
  analogWrite(PWM,setPwm);
  
  //Test code
  /*digitalWrite(PWM,LOW);
  delay(10000);
  digitalWrite(PWM,HIGH);
  delay(10000);
  */
 
}




