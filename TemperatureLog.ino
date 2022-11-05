/*
    Name:       Temperature sensor
    Created:  5/29/2022 4:03:12 PM
    Author:     Alex
*/

// Define User Types below here or use a .h file
//#include <LiquidCrystal_I2C.h>
//#include <ArduinoLowPower.h>
//#include <Wire.h>
#include <SPI.h> //sd card
#include <SD.h> //sd card
#include <DHT_U.h> //air moisture and temperature
#include <DHT.h> //air moisture and temperature


#define DHTPIN 2 //pin for air sensor
#define DHTTYPE DHT11 //air sensor type
File myFile; //object for file writing or reading
//Soil Moisture Sensor
const int dry = 582; // value for dry sensor
const int wet = 298; // value for wet sensor
//watering relay
int Water1 = 4;


// Define Functions below here or use other .ino or cpp files
DHT dht(DHTPIN, DHTTYPE);
//LiquidCrystal_I2C lcd(0x27, 20, 4);
//

// The setup() function runs once each time the micro-controller starts
void setup()
{
    //set relay as output pin
    pinMode(Water1, OUTPUT);

    //lcd setup
    /*lcd.init();
    lcd.backlight();*/

    //set up serial monitoring if serial is available
    if (Serial) {
        Serial.begin(115200);

        Serial.println(F("DHT11 test!!"));
        Serial.println(F("Initializing SD card..."));
        if (!SD.begin(10)) { //set CS pin
            Serial.println(F("SD initialization failed!"));
            while (1);
        }

        Serial.println(F("Initialization complete!"));
    }
    

    //work on the header of the file
    //file has to be opened, written, then closed
    if (!SD.exists("test.txt")) {
        myFile = SD.open("test.txt", FILE_WRITE);
        if (myFile) {
            myFile.println(F("# file header"));
            myFile.println(F("Humidity %\tTemperature C\tTemperature F\tHeat Index F\tHeat Index C\tSoil Moisture"));
            myFile.close();
        }
    }

    dht.begin();

}

// Add the main program code into the continuous loop() function
void loop()
{
  Serial.println("boop");
    delay(1000*2);//2 second delay only

    //get variables from the dht object
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);

    //check whether any of the variables are missing
    if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println(F("Failed to communicate with the sensor..."));
        return;
    }

    //calculate the heat indexes
    float hic = dht.computeHeatIndex(t, h, false);
    float hif = dht.computeHeatIndex(f, h, true);

    //soil moisture sensor
    int sens = analogRead(A0);
    int moisture = map(sens, wet, dry, 100, 0);

    //format and print out the results
    /*
    if (Serial) {
        Serial.print(F("Humidity:\t"));
        Serial.print(h);
        Serial.print(F("%\tTemperature:\t"));
        Serial.print(t);
        Serial.print(F("C\t"));
        Serial.print(f);
        Serial.print(F("F\tHeat Index:\t"));
        Serial.print(hic);
        Serial.print(F("C\t"));
        Serial.print(hif);
        Serial.print(F("F\tSoil:\t"));
        
    }
    */

    //print information
    /*lcd.setCursor(0, 0);
    lcd.print("Humidity:   " + String(h) + "%");
    lcd.setCursor(0, 1);
    lcd.print("Temp:       " + String(f) + "F");
    lcd.setCursor(0, 2);
    lcd.print("Heat Index: " + String(hif) + "F");
    lcd.setCursor(0, 3);
    lcd.print("Soil Moist: " + String(moisture));*/

    //work on the header of the file
    //file has to be opened, written, then closed
    myFile = SD.open("test.txt", FILE_WRITE);
    if (myFile) {
        myFile.print(h);
        myFile.print(F("\t"));
        myFile.print(t);
        myFile.print(F("\t"));
        myFile.print(f);
        myFile.print(F("\t"));
        myFile.print(hic);
        myFile.print(F("\t"));
        myFile.print(hif);
        myFile.print(F("\t"));
        myFile.println(moisture);
    }
    myFile.close();
}

//void triggerPump(int pumpPin)
//{
//    //turn off the lcd backlight so it doesn't have to draw too much power
//    lcd.noBacklight();
//    delay(100);
//
//    //set pumpPin high
//    digitalWrite(pumpPin, HIGH);
//    delay(1000);
//    digitalWrite(pumpPin, LOW);
//    delay(100);
//
//    //turn lcd backlight on again
//    lcd.backlight();
//}
