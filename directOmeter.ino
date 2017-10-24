#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_NeoPixel.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 6, NEO_GRB + NEO_KHZ800);

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

int topLED = 9;
int compassOffset = 1; // to offset the LED from the LSM303 orientation
int ledDir = 2;
int lastDir;
int tempDir = 0;
int dirLED_r = 0;
int dirLED_g = 0;
int dirLED_b = 255;

// Pi for calculations
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

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);

  normX = (event.magnetic.x+offX)*gainX;
  normY = (event.magnetic.y+offY)*gainY;
  normZ = (event.magnetic.z+offZ)*gainZ;

  //calculating the angle of the vector y, x
  float compassHeading = (atan2(normY, normX) * 180) / Pi;

  // normalizing to 0 - 360
  if (compassHeading < 0)
  {
    compassHeading = compassHeading + 360;
  }
  compassHeading = 360 - compassHeading;
  
  if ((millis() - lastDisplayTime) > 500)  // display once/second
  {
    Serial.println();
    Serial.print("Mag X: "); Serial.print(normX);
    Serial.print(" Mag Y: "); Serial.print(normY);
    Serial.print(" Mag Z: "); Serial.print(normZ);
    Serial.print(" heading:  "); Serial.print(compassHeading);
    lastDisplayTime = millis();
  

    if ((compassHeading > 348.75)||(compassHeading < 11.25)) {
        tempDir = topLED;
    }
    
    for(int i = 1; i < 16; i++){
      float pieSliceCenter = 45/2*i;
      float pieSliceMin = pieSliceCenter - 11.25;
      float pieSliceMax = pieSliceCenter + 11.25;
      if ((compassHeading >= pieSliceMin)&&(compassHeading < pieSliceMax)) {
          tempDir = topLED + i;
      }
    }
  
    // normalize LED position
    if (tempDir > 15) {
      ledDir = tempDir - 16;
    }
  
    else if (tempDir < 0) {
      ledDir = tempDir + 16;
    } 
    else {
      ledDir = tempDir;
    }
  
    if (lastDir != ledDir) {
      strip.setPixelColor(lastDir, strip.Color(0, 0, 0));
      strip.setPixelColor(ledDir, strip.Color(dirLED_r, dirLED_g, dirLED_b));
      strip.show();
      Serial.print(" LED: "); Serial.print(ledDir);
      lastDir = ledDir;
    }
  }
}
