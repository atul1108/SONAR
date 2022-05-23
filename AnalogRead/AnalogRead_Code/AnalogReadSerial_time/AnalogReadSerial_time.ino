/*
  AnalogReadSerial with Current data and time(using Current Timestamp or Processing)
  1. Reads two analog input on pin A0 & A2, prints the result to the serial monitor.
  2. You can send the text on the Serial Monitor to set the clock to noon 5 Jan  2019
  T1357041600
  Designer: Atul Kumar singh 
 https://github.com/atul1108
 https://www.linkedin.com/in/atul-kumar-singh-118412204
*/
#include <TimeLib.h>
#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
    while (!Serial) ; // Needed for Leonardo only
  pinMode(13, OUTPUT);
  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue_one = analogRead(A0);
  int sensorValue_two = analogRead(A1);
  if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus()!= timeNotSet) {
    digitalClockDisplay();  
    // print out the value you read:
    Serial.print(sensorValue_one); // First sensor value
    Serial.print(" ");
    Serial.print(sensorValue_two); // Second sensor value
    Serial.println();
  }
  delay(1000);        // delay in between reads for stability
}
void digitalClockDisplay(){
  // digital clock display of the time

  Serial.print(year()); 
  Serial.print("/"); 
  Serial.print(month());
  Serial.print("/");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" "); 
}
void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}

