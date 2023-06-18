/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This example demonstrates basic scale output. See the calibration sketch to get the calibration_factor for your
 specific load cell setup.
 
 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE
 
 The HX711 does one thing well: read load cells. The breakout board is compatible with any wheat-stone bridge
 based load cell which should allow a user to measure everything from a few grams to tens of tons.

 Arduino pin 2 -> HX711 CLK
 3 -> DAT
 5V -> VCC
 GND -> GND
 
 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
 
*/

#include "HX711.h"  //This library can be obtained here http://librarymanager/All#Avia_HX711

#define bowl_calibration_factor -23950.0      //This value is obtained using the SparkFun_HX711_Calibration sketch
#define feed_box_calibration_factor -23950.0  //This value is obtained using the SparkFun_HX711_Calibration sketch


#define BOWL_DOUT_PIN 4
#define BOWL_SCK_PIN 2

#define FEED_BOX_DOUT_PIN 23
#define FEED_BOX_SCK_PIN 22

extern HX711 bowl_scale;
extern HX711 feed_box_scale;
extern bool is_scale_enable = true;

void HX711_setup() {
  Serial.println("Initialize HX711...");

  bowl_scale.begin(BOWL_DOUT_PIN, BOWL_SCK_PIN);
  bowl_scale.set_scale(bowl_calibration_factor);  //This value is obtained by using the SparkFun_HX711_Calibration sketch
  bowl_scale.tare();                 //Assuming there is no weight on the scale at start up, reset the scale to 0

  feed_box_scale.begin(FEED_BOX_DOUT_PIN, FEED_BOX_SCK_PIN);
  feed_box_scale.set_scale(feed_box_calibration_factor);  //This value is obtained by using the SparkFun_HX711_Calibration sketch
  feed_box_scale.set_offset(-602988);                            //Assuming there is no weight on the scale at start up, reset the scale to 0
}

void HX711_print() {
  Serial.print("Bowl Reading: ");
  Serial.print(bowl_scale.get_units(3), 3);  //scale.get_units() returns a float
  Serial.print("g");                         //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();

  Serial.print("Feed Box Reading: ");
  Serial.print(feed_box_scale.get_units(), 1);  //scale.get_units() returns a float
  Serial.print("Kg");                           //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();
}