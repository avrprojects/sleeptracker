
#include <Wire.h>

#include <ADXL345.h>
//#include <TimerOne.h>

#include <SdFat.h>
#include <SdFatUtil.h>
#include <ctype.h>


Sd2Card card;
SdVolume volume;
SdFile root, file;

char buffer[128];

ADXL345 accel;
int acc_data[3];

int counter = 0;
int16_t targetMS = 0;

void setup(void)
{
  delay(1000);

  /* Init serial port */
  Serial.begin(57600);

  /* Init accelerometer */
  Serial.println("Init: Accelerometer");
  accel.powerOn();
  accel.setRangeSetting(2);
  accel.setRate(800.0);

  /* Init SD card */
  Serial.println("Init: SD");
  delay(100);
  pinMode(10, OUTPUT);
  card.init();
  volume.init(card);
  root.openRoot(volume);

  Serial.println("Init: LED");
  delay(100);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  /*Serial.println("Init: Timer");
  delay(100);
  Timer1.initialize(5e5); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Serial.println("Init: Timer interrupt");
  delay(100);*/
  
  targetMS = millis() + 250;
  //Timer1.attachInterrupt( doTimer ); // attach the service routine here
  
  Serial.println("Init complete.");
  delay(100);
}

/*void doTimer() {
    doStuff = true;
}*/

void loop(void)
{
  if (millis() > targetMS) {
    
    int16_t ms = millis();
    targetMS = ms + 250;
    accel.readAccel(acc_data);

    if (accel.status) {
      digitalWrite(13, HIGH);
      file.open(root, "Run3.txt", O_CREAT | O_APPEND | O_WRITE);
      sprintf(buffer, "%d %d %d %d\n", ms, acc_data[0], acc_data[1], acc_data[2]);

      file.print(buffer);
      file.close();
//      delay(200);
      digitalWrite(13, LOW);
    } else {
      Serial.println("ERROR: ADXL345 data read error");
    }
  }
  
}

/*
  Local Variables:
  mode: c++
  End:
*/
