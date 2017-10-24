#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

float normX, normY, normZ;

float offX=28.09;
float offY=-28.775;
float offZ=-13.52;

float gainX=1.212;
float gainY=1.063;
float gainZ=0.517;

float MagMinX, MagMaxX;
float MagMinY, MagMaxY;
float MagMinZ, MagMaxZ;

long lastDisplayTime;

// Pi for calculations - not the raspberry type
const float Pi = 3.14159;

void setup() {
  Serial.begin(9600);
  Serial.println("Magnetometer Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  lastDisplayTime = millis();
}

void loop() {
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);

  normX = (event.magnetic.x+offX)*gainX;
  normY = (event.magnetic.y+offY)*gainY;
  normZ = (event.magnetic.z+offZ)*gainZ;

  //calculating the angle of the vector y, x
  float heading = (atan2(normY, normX) * 180) / Pi;

  // normalizing to 0 - 360
  if (heading < 0)
  {
    heading = 360 + heading;
  }
  
  if ((millis() - lastDisplayTime) > 1000)  // display once/second
  {
    Serial.print("Mag X: "); Serial.print(normX);
    Serial.print("Mag Y: "); Serial.print(normY);
    Serial.print("Mag Z: "); Serial.print(normZ);
    Serial.print("heading:  "); Serial.print(heading);
    Serial.println();
    lastDisplayTime = millis();
  }
  

}
