//Take a reading from a scale and post it to Adafruit.IO
//November 2016 - Afrooz Samaei

#include "config.h"
#include "HX711.h"

#define calibration_factor 8500.0 //This value is different for different scales. 
//It should be obtained by putting a known weight on the scale
//and changing the calibration factor until the scale shows the right weight.

//Defining the pins
#define DOUT  16
#define CLK  SCK

HX711 scale(DOUT, CLK);

//variable to hold the value from scale
int scaleVal;
int weight;

// set up the 'weightVal' feed
AdafruitIO_Feed *weightVal = io.feed("weightVal");


void setup() {

  // start the serial connection
  Serial.begin(115200);

  scale.set_scale(calibration_factor);
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0


  // wait for serial monitor to open
  while (! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  //get value from scale
  scaleVal = scale.get_units();
  weight = scaleVal * 0.453592;

  if (weight > 20) //check if someone is standing in the booth
  {
    // save weight to the 'weightVal' feed on Adafruit IO
    weightVal->save(weight);

  }

  Serial.print("weight -> ");
  Serial.println(weight);

}
