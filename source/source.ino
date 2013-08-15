/*
  pins ~10, ~9, ~5, 4 are all LED's
  10 = Notify recorded pin
  9 = All is loaded pin
  5 = Loading
  4 = Power on;
*/
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <dht11.h>
#include <SD.h>

int Temp, Light, Humidity, hCheck;
//LEDS
#define PowerOnPin 4
#define LoadingPin 5
#define ReadyPin 9
#define NotifyPin 10
#define GPSLockPin 6
//Sensors
#define tempPin 0
#define lightPin 1
#define tiltPin 6
#define humidityPin 7

dht11 DHT11;
const int chipSelect = 8;

SoftwareSerial mySerial(3,2);
Adafruit_GPS GPS(&mySerial);

boolean SDLoaded = true;

#define GPSECHO  true

// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;
void useInterrupt(boolean);

void setup(){
   Serial.begin(115200);
   Serial.print("Setting LED's...");
   
   GPS.begin(9600);
   
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
   
   // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  // the nice thing about this code is you can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS for you. that makes the
  // loop code a heck of a lot easier!
  useInterrupt(true);
   
   mySerial.println(PMTK_Q_RELEASE);
}

void loop(){
  sensors();
  scanGPS();
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
  
  saveToSD("tem:" + (String)Temp);
  saveToSD("lux:" + (String)Light); 
  saveToSD("hum:" + (String)Humidity);
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
// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
  // writing direct to UDR0 is much much faster than Serial.print 
  // but only one character can be written at a time. 
#endif
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } 
  else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}


uint32_t timer = millis();
void scanGPS(){
    // in case you are not using the interrupt above, you'll
  // need to 'hand query' the GPS, not suggested :(
  if (! usingInterrupt) {
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO)
      if (c) Serial.print(c);
  }

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) { 
    timer = millis(); // reset the timer

    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); 
    Serial.print(':');
    Serial.print(GPS.minute, DEC); 
    Serial.print(':');
    Serial.print(GPS.seconds, DEC); 
    Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); 
    Serial.print('/');
    Serial.print(GPS.month, DEC); 
    Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); 
    Serial.print((int)GPS.fix);
    Serial.print(" quality: "); 
    Serial.println((int)GPS.fixquality); 
  
  
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", "); 
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      saveToSD("Minute: ",GPS.minute);
      saveToSD("Altitude: ", GPS.altitude);
      saveToSD("Latitude: ", GPS.latitude);
      saveToSD("longitude: ", GPS.lon);
      saveToSD("Lat: ", GPS.lat);
      saveToSD("Lon: ", GPS.lon);
}
    if(GPS.fix){
       digitalWrite(GPSLockPin, HIGH);
    }else{
       digitalWrite(GPSLockPin, LOW); 
    }
}
void saveToSD(String args){
  File dataFile = SD.open("oneHour.txt", FILE_WRITE);
  
    dataFile.print(args + ",");
  
  dataFile.close();
  
  digitalWrite(NotifyPin, HIGH);
}
void saveToSD(String pre, float args){
  File dataFile = SD.open("oneHour.txt", FILE_WRITE);
    dataFile.print(pre);
    dataFile.print(args);
    dataFile.print(",");
  
  dataFile.close();
  
  digitalWrite(NotifyPin, HIGH);
}
