#include "Arduino.h"
#include "Enes100.h"
#include "Tank.h"

void setup() {
    Enes100.begin("Simulator", DATA, 3, 8, 9);
    Enes100.println(Enes100.getTheta());
}

void loop() {
    delay(10);
    Enes100.println("Hello, World! (from loop)");
    delay(1000);
    
    // Rotate until theta is approximately 1.57 (between 1.37 and 1.77)
    while(!(Enes100.getTheta() > 1.37 && Enes100.getTheta() < 1.77)) {
        Enes100.println(Enes100.getTheta());
        Tank.begin();
        Tank.setLeftMotorPWM(25); 
        Tank.setRightMotorPWM(-25);
    }
    
    // Stop motors after rotation
    Tank.setLeftMotorPWM(0);
    Tank.setRightMotorPWM(0);
    
    // Fine adjustment for theta if necessary
    if(Enes100.getTheta() < 1.57) {
       while(!(Enes100.getTheta() > 1.57 && Enes100.getTheta() < 1.58)) {
            Tank.setLeftMotorPWM(-1); 
            Tank.setRightMotorPWM(1);
       }
    } 
    else {
       while(!(Enes100.getTheta() > 1.57 && Enes100.getTheta() < 1.58)) {
           Tank.setLeftMotorPWM(1); 
           Tank.setRightMotorPWM(-1);
       }
    }
    
    // Move forward until y-coordinate reaches 1.5
    while(Enes100.getY() < 1.5) {
        Tank.setLeftMotorPWM(100);
        Tank.setRightMotorPWM(100);
    }
    
    // Stop motors after reaching y = 1.5
    Tank.setLeftMotorPWM(0);
    Tank.setRightMotorPWM(0);
}
