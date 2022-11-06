#include "Servo.h"

// Pin 1 is thumb, 2 is pointer, etc.
#define SERVO_PIN1 3   // PWM Motor 1 Pin
#define SERVO_PIN2 5   // PWM Motor 2 Pin
#define SERVO_PIN3 6   // PWM Motor 3 Pin
#define SERVO_PIN4 10   // PWM Motor 4 Pin
#define SERVO_PIN5 11   // PWM Motor 5 Pin 

#define FLEX_PIN1 15 // Flex sensor 1
#define FLEX_PIN1 16 // Flex sensor 2
#define FLEX_PIN1 17 // Flex sensor 3
#define FLEX_PIN1 18 // Flex sensor 4
#define FLEX_PIN1 19 // Flex sensor 5

#define MIN_VALUE 0   // Minimum Servo position
#define MAX_VALUE 180 // Maximum Servo position

// These are all going to be sent from the app below
//#define mode1 4
//#define mode2 7
//#define mode3 8
//#define reset 2
//#define pauseMotor 13
//#define select 12

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
int value_set = 0;
int value_servo = 0; // servo analog value
int value_servo_old = 0; // Used to hold old servo value to look for change.
bool motorSpeed = 0;
int count = 0;
bool start = 0;
bool initialize;
bool pause = 0;
// Using a 32 max int, we will have a 5 bit system to tell which motors we want running. DECODE THIS INT
unsigned int motorSelect = 0;
unsigned long previousMillis = 0;
unsigned long interval = 1000;
void setup() {
  servo1.attach(SERVO_PIN1); // assigns PWM pin to the servo object
  servo2.attach(SERVO_PIN2); // assigns PWM pin to the servo object
  servo3.attach(SERVO_PIN3); // assigns PWM pin to the servo object
  servo4.attach(SERVO_PIN4); // assigns PWM pin to the servo object
  servo5.attach(SERVO_PIN5); // assigns PWM pin to the servo object
  initialize = 1;
  pinMode(reset, INPUT);
  pinMode(mode1, INPUT);
  pinMode(mode2, INPUT);
  pinMode(mode3, INPUT);
  pinMode(select, INPUT);
  pinMode(pauseMotor, INPUT);
  Serial.begin(9600);     // Set Serial Monitor window comm speed
}

void loop() {
  if(millis() - previousMillis > interval){
    if(digitalRead(mode1)){
      Serial.print("SLOW");
      count = 50;
      start = 1;
    }
    if(digitalRead(mode2)){
      Serial.print("MEDIUM");
      count = 100;
      start = 1;
    }
    if(digitalRead(mode3)){
      Serial.print("FAST");
      count = 200;
      start = 1;
    }
    if(digitalRead(reset) || initialize==1){
      Serial.println("reset");
      servo1.write(0);
      servo2.write(0);
      servo3.write(0);
      servo4.write(0);
      servo5.write(0);
      start = 0;
      initialize = 0;
      value_set=0;
    }
    if(digitalRead(pauseMotor)){
      pause = !pause;
      Serial.println("pause invert");
    }
    if(start){
      if((count>0 && count<1023) && value_set<1023-count){
        if(pause==0){
          value_set+=count;
          Serial.print("value:");
          Serial.println(value_set);              
        }
      }
    }
    if(digitalRead(select)){
      motorSelect++;      
      if(motorSelect==32){
        motorSelect=0;
      }
      Serial.print("select:");
      Serial.println(motorSelect);
    } else {
      value_servo = 0;
    }
    value_servo = map(value_set, 0, 1023, MIN_VALUE, MAX_VALUE); // remap pot value to servo value
    if(pause==0) {   // Only do something if there's a change in the servo position
      if(bitRead(motorSelect,0)==1){
        servo1.write(value_servo);
        Serial.println("ONE");
      } else {
        digitalWrite(3, LOW);
      }
      if(bitRead(motorSelect,1)==1){
        servo2.write(value_servo);
        Serial.println("TWO");
      }
      if(bitRead(motorSelect,2)==1){
        servo3.write(value_servo);
        Serial.println("THREE");
      }
      if(bitRead(motorSelect,3)==1){
        servo4.write(value_servo);
        Serial.println("FOUR");
      }
      if(bitRead(motorSelect,4)==1){
        servo5.write(value_servo);
        Serial.println("FIVE");
      }
      value_servo_old = value_servo;
      delay(25); // give servo time to move
    }
    previousMillis+=interval;
  }
}
