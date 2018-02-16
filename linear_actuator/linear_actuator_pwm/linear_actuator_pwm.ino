#include <Servo.h>
#define BACKWARDS 0
#define FORWARDS 3000
#define STOP 1500
#define TOL 10

#define MOTOR1_MAX 660
#define MOTOR1_MIN 400

Servo actuator;  // create servo object to control a RoboClaw channel

int potentiometer_pin = 0; // Analog pin
int actuator_pin = 2; // PWM pin
float pot_value = 10000;//Not necessarily the same for both actuators

void setup() 
{ 
  Serial.begin(57600);
  actuator.attach(actuator_pin);  // attaches the RC signal on pin 5 to the servo object 
}


void moveToPercent(float target){

    float position = 100 * (analogRead(potentiometer_pin) - MOTOR1_MIN) 
                            / (MOTOR1_MAX - MOTOR1_MIN);

    Serial.println("Measuring");
    Serial.println(analogRead(potentiometer_pin));
    Serial.println((int) target);

    if (target > position){// move forwards
        actuator.writeMicroseconds(FORWARDS);        
    }else{
        actuator.writeMicroseconds(BACKWARDS);
    }

    while(abs(target - position) > TOL){

        Serial.println("Target: ");
        Serial.println(target);
        Serial.println("Raw");
        Serial.println(analogRead(potentiometer_pin));
        Serial.println("Position: ");
        Serial.println(position);

        
        if (target > position){// move forwards
            actuator.writeMicroseconds(FORWARDS);
            Serial.println("Moving forwards");
        }else{
            actuator.writeMicroseconds(BACKWARDS);
            Serial.println("Moving backwards");
        }


        position = 100 * (analogRead(potentiometer_pin) - MOTOR1_MIN) 
                                    / (MOTOR1_MAX - MOTOR1_MIN);
        delay(100);
    }
    actuator.writeMicroseconds(STOP);

}

void loop() 
{ 
  
   // moveToPercent(50);
    //delay(1000);

    moveToPercent(90);
    delay(1000);

    moveToPercent(10);
    delay(1000);
} 
