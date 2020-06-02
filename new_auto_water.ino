// TO CHANGE THE DATE AND TIME GO TO FILE>EXAMPLES>DS1307RTC>SETTIME>UPLOAD 
// AND THEN UPLOAD READTEST TO CHECK


//SD card Saving
#include <SPI.h>
#include <SD.h>

//DS3231  
#include <Wire.h>
#include <DS3231.h>
DS3231 Clock;
bool Century=false;
bool h12;
bool PM;

//Sensor Constants  
const int hygrometer = A0;  //Hygrometer sensor analog pin output at pin A0 of Arduino
int value;

//SD Constants
File file;
const int chipSelect = 10;
int relay_pin = 8;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "cESqkQLRVe8Bot9Asi4SfN2BjUaEOyiH";

void setup(){
//Water Pump
  pinMode(relay_pin, OUTPUT);
//SD Card
  pinMode(chipSelect, OUTPUT);
  Serial.begin(9600);

//Clock
  // Start the I2C interface
  Wire.begin();

//SD card 
//Comment out if not connecting to computer
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
  
  Serial.print("Initializing SD card...");
  
  if (!SD.begin()) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

   // re-open the file for reading:
  file = SD.open("test.txt");
  if (file) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (file.available()) {
      Serial.write(file.read());
    }
    // close the file:
    file.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop(){
//  Blynk.run();
//  
// This allows for the update of variables for time or accessing the individual elements.
  Serial.print("Current Date / Time: ");
  Serial.print(Clock.getDate());
  Serial.print("/");
  Serial.print(Clock.getMonth(Century));
  Serial.print("/");
  Serial.print(Clock.getYear());
  Serial.print(" ");
  Serial.print(Clock.getHour(h12, PM));
  Serial.print(":");
  Serial.print(Clock.getMinute());
  Serial.print(":");
  Serial.print(Clock.getSecond());
  Serial.print(" ");
  Serial.print("T=");
  Serial.println(Clock.getTemperature(), 2);
  
  
//  // When the plant is watered well the sensor will read a value 380~400, I will keep the 400 
//  // value but if you want you can change it below. 
//  
  value = analogRead(hygrometer);   //Read analog value 
  value = constrain(value,400,1023);  //Keep the ranges!
  value = map(value,400,1023,100,0);  //Map value : 400 will be 100 and 1023 will be 0
  Serial.println(value);          
          
//Saving to SD CARD 
  if (Clock.getSecond() == 30) {
    file = SD.open("test.txt",FILE_WRITE);  
    if (file) {
        Serial.println("data saved: "); 
        Serial.print(Clock.getDate());
        Serial.print("/");
        Serial.print(Clock.getMonth(Century));
        Serial.print(",");
        Serial.print(Clock.getHour(h12, PM));
        Serial.print(":");
        Serial.print(Clock.getMinute());
        Serial.print(":");
        Serial.print(Clock.getSecond());
        Serial.print(",");
        Serial.println(value);
        file.print(Clock.getDate());
        file.print("/");
        file.print(Clock.getMonth(Century));
        file.print(",");
        file.print(Clock.getHour(h12, PM));
        file.print(":");
        file.print(Clock.getMinute());
        file.print(":");
        file.print(Clock.getSecond());
        file.print(",");
        file.println(value);
        file.close();
        delay(5000);
    } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
    }
  }
  
  
//Activating the Water Pump
  if (((Clock.getHour(h12, PM) == 7) and (Clock.getMinute() == 0) and (Clock.getSecond() == 0)) or ((Clock.getHour(h12, PM) == 17) and (Clock.getMinute() == 0) and (Clock.getSecond() == 0)))
  {
      //Activate Water
      digitalWrite(relay_pin,HIGH);
      //For 15 seconds
      delay(15000);
      //De-activate Water
      digitalWrite(relay_pin,LOW);
  }
}

////Blynk LCD Screen
//void send_moisture_value() {
//  lcd.print(0,0,"Moisture: ");
//  lcd.print(0,1,value);    
//}
