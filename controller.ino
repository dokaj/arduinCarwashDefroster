/* I2C LCD with Arduino example code. More info: https://www.makerguides.com */
// Include the libraries:
// LiquidCrystal_I2C.h: https://github.com/johnrickman/LiquidCrystal_I2C
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <ds3231.h>

///// temp sensors /////
// MAX6675 temp sensor for extreme temp
//#include "max6675.h"

// DHT sensor for normal temp
#include <DHT.h>
/////



/////////// LCD display ///////////
// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 20x4 LCD.


/////////// MAX6675 temp sensor ///////////
//int soPin = 8;// SO=Serial Out
//int csPin = 9;// CS = chip select CS pin
//int sckPin = 10;// SCK = Serial Clock pin
// float actualTemp = 0.0;
//MAX6675 tempModule(sckPin, csPin, soPin);// create instance object of MAX6675


/////////// DHT temp sensor ///////////
//Constants
#define DHTPIN 2            //what pin we're connected to
#define DHTTYPE DHT21       //DHT 21  (AM2301)
DHT dht(DHTPIN, DHTTYPE);   //Initialize DHT sensor for normal 16mhz Arduino
//Variables
float actualHumidity;  //Stores humidity value
float actualTemp; //Stores temperature value


/////////// 4 Relay Module ///////////
int relayPin3 = 6; //This is the Arduino Pin that will control Relay #3
int relayPin4 = 7; //This is the Arduino Pin that will control Relay #4

struct ts actualTime;
char bufferForTime[10];


/////////// Limits ///////////
const int timeLimitMin = 6;
const int timeLimitMax = 20;
const int tempLimit = 3;

void setup() {
  /////////// Real time clock module ///////////
  // Set clock (uncomment this to set clock
  Serial.begin(9600);
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  //actualTime.hour=22; 
  //actualTime.min=49;
  //actualTime.sec=0;
  //actualTime.mday=2;
  //actualTime.mon=2;
  //actualTime.year=2022;
  //DS3231_set(actualTime);

  
  /////////// Initiate the LCD ///////////
  lcd.init();
  lcd.backlight();
  lcd.createChar(3, Check);

  /////////// Temp sensors ///////////
  //// MAX6675 ////
  // Serial.begin(9600);// initialize serial monitor with 9600 baud
  // Serial.println("MAX6675 temp sensor"); 
  
  //// DHT ////
  Serial.begin(9600);
  dht.begin();
  

  /////////// 4 Relay Module ///////////
  pinMode(relayPin3, OUTPUT);
  pinMode(relayPin4, OUTPUT);
}




void loop() {
   // actualTemp = tempModule.readCelsius(); // MAX6675
   DS3231_get(&actualTime);
   
   //Read data and store it to variables hum and temp - DHT
   actualHumidity = dht.readHumidity();
   actualTemp= dht.readTemperature();
   
 
   lcd.setCursor(0, 0); //Set the cursor on the third column and the second row (counting starts at 0!).
   lcd.print(actualTemp,0);
   lcd.print((char)223);
   lcd.print("C ");
   if (actualTemp <= tempLimit)
   {
    lcd.print("OK ");
   }
   else 
   {
    lcd.print("NO ");
   }
   //lcd.print(actualHumidity,0);
   //lcd.print("%");



   sprintf(bufferForTime, "%02d:%02d", actualTime.hour, actualTime.min);
   lcd.print(bufferForTime);
   
   if ((actualTime.hour >= timeLimitMin) && (actualTime.hour <= timeLimitMax))
   {
    lcd.print(" OK");
   }
   else 
   {
    lcd.print(" NO");
   }
   

   // Defrosting for carwash
   if ((actualTemp <= tempLimit) && (actualTime.hour >= timeLimitMin) && (actualTime.hour <= timeLimitMax))
   {
      digitalWrite(relayPin3, HIGH);
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("Fagymentesites");
      
   }
   else
   {
     digitalWrite(relayPin3, LOW);
     delay(1000);
     lcd.clear();
     lcd.setCursor(0,1);
     lcd.print("   Kikapcsolva  ");
   }


   
}
