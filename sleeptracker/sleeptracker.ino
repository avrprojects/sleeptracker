#include <Wire.h>

#include <ADXL345.h>

#include <SdFat.h>
#include <SdFatUtil.h>
#include <ctype.h>


Sd2Card card;
SdVolume volume;
SdFile root, file;

char buffer[128];

ADXL345 accel;
int acc_data[3];


void setup(void)
{
  delay(1000);

  /* Init serial port */
  Serial.begin(57600);

  /* Init accelerometer */
  accel.powerOn();
  accel.setRangeSetting(2);
  accel.setRate(800.0);

  /* Init SD card */
  pinMode(10, OUTPUT);
  card.init();
  volume.init(card);
  root.openRoot(volume);
}


void loop(void)
{
  int16_t ms = millis();
  accel.readAccel(acc_data);

  if (accel.status) {
    file.open(root, "Run.txt", O_CREAT | O_APPEND | O_WRITE);
    sprintf(buffer, "%d %d %d %d\n", ms, acc_data[0], acc_data[1], acc_data[2]);
    file.print(buffer);
    file.close();
  } else {
    Serial.println("ERROR: ADXL345 data read error");
  }
}

/*
  Local Variables:
  mode: c++
  End:
*/
