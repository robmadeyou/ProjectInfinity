/*
  pins ~10, ~9, ~5, 4 are all LED's
  10 = Notify recorded pin
  9 = All is loaded pin
  5 = Loading
  4 = Power on;
*/
#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <dht11.h>
#include <SD.h>

int Temp, Light, Humidity, hCheck;
//LEDS
#define PowerOnPin 4
#define LoadingPin 5
#define ReadyPin 9
#define NotifyPin 8
#define GPSLockPin 6
//Sensors
#define tempPin 0
#define lightPin 1
#define tiltPin 6
#define humidityPin 7

dht11 DHT11;
const int chipSelect = 10;

TinyGPS gps;
SoftwareSerial ss(3, 2);

boolean SDLoaded = true;

void setup(){
   Serial.begin(4800);
   Serial.print("Setting LED's...");
   
   ss.begin(4800);
   
   pinMode(PowerOnPin, OUTPUT);
   pinMode(LoadingPin, OUTPUT);
   pinMode(ReadyPin, OUTPUT);
   pinMode(NotifyPin, OUTPUT);
   pinMode(GPSLockPin, OUTPUT);
   
   digitalWrite(PowerOnPin, HIGH);
   delay(100);
   digitalWrite(LoadingPin, HIGH);
   Serial.println("..Done!");
   Serial.print("Initializing SD card....");
   if(!SD.begin(chipSelect)){
      Serial.println("...Failed!");
      SDLoaded = false;
   }else{
     SDLoaded = true;
     Serial.println("...Done!");
   }
   
   delay(1000);
   digitalWrite(ReadyPin, HIGH);
   if(SDLoaded == false){
     digitalWrite(PowerOnPin, HIGH);
     digitalWrite(LoadingPin, LOW);
     digitalWrite(ReadyPin, LOW); 
   }
}

void loop(){
  sensors();
  gpsLoop();
}

void sensors(){
  Temp = (5.0 * analogRead(tempPin) * 100.0) / 1024;
  delay(10);
  Light = toLux(analogRead(lightPin));
  hCheck = DHT11.read(humidityPin);
  if(hCheck != 0)
    Humidity = 255; //Must be an error
  else
    Humidity = DHT11.humidity;
  
  saveStringToSD("tem:" + (String)Temp);
  saveStringToSD("lux:" + (String)Light); 
  saveStringToSD("hum:" + (String)Humidity);
  delay(30);
  digitalWrite(NotifyPin, LOW);
  delay(970);
}

int toLux(int adc)
{
//  return (map(adc, 0, 1023, 900, 0)); simple linear model
  if (adc > 975)
    return 1;
  else if (adc > 768)
    return 1 + 0.04 * (adc - 768);
  else if (adc > 341)
    return 10 + 0.16 * (adc - 341);
  else if (adc > 293)
    return 80 + 0.82 * (adc - 293);
  else if (adc > 236)
    return 120 + 0.53 * (adc - 236);
  else if (adc > 171)
    return 150 + 1.52 * (adc - 171);
  else if (adc > 156)
    return 250 + 3.46 * (adc - 156);
  else
    return 300 + 5.6 * (adc - 67);
}

void saveStringToSD(String args){
  File dataFile = SD.open("oneHour.txt", FILE_WRITE);
  dataFile.print(args + ",");
  Serial.println(args);
  dataFile.close();
  
  digitalWrite(NotifyPin, HIGH);
}
void gpsLoop(){
  boolean newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    if(newData){
      digitalWrite(GPSLockPin, HIGH);
    }
}
