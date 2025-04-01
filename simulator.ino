#include "Arduino.h"
#include "Enes100.h"
#include "Tank.h"


void setup() {
    // Enes100.println(257);
    // Enes100.println(true);
    // Enes100.println("Hello, World!");
    // Enes100.println(1.0);
    // float x = Enes100.getX();
    // Enes100.println(x);
    // Enes100.println(Enes100.getY());
    // Enes100.println(Enes100.getTheta());
    // Enes100.println(Enes100.isVisible());
    // Enes100.println(Enes100.isConnected());
    
    // // A little test script that should put the simulator through its paces.
    // Enes100.begin("Simulator", CRASH_SITE, 3, 8, 9);
    // Enes100.mission(LENGTH, 1000);
    // Enes100.mission(HEIGHT, 1000);
    // Enes100.mission(DIRECTION, NORMAL_X);
    // Enes100.mission(DIRECTION, NORMAL_Y);
    
    // Enes100.begin("Simulator", MATERIAL, 3, 8, 9);
    // Enes100.mission(WEIGHT, HEAVY);
    // Enes100.mission(WEIGHT, MEDIUM);
    // Enes100.mission(WEIGHT, LIGHT);
    // Enes100.mission(MATERIAL_TYPE, FOAM);
    // Enes100.mission(MATERIAL_TYPE, PLASTIC);
    
    // Enes100.begin("Simulator", WATER, 3, 8, 9);
    // Enes100.mission(DEPTH, 1000);
    // Enes100.mission(WATER_TYPE, FRESH_UNPOLLUTED);
    // Enes100.mission(WATER_TYPE, FRESH_POLLUTED);
    
    // Enes100.begin("Simulator", SEED, 3, 8, 9);
    // Enes100.mission(PERCENTAGE, 100);
    // // In the simulator, Coordinate only works in this way. You cannot declare coordinate objects.
    // Enes100.mission(LOCATION, Coordinate(100, 100));
    // Enes100.mission(LOCATION, Coordinate(200, 200));
    Enes100.begin("Simulator", DATA, 3, 8, 9);
    Enes100.println(Enes100.getTheta());
    
    // delay(1000);
    // Tank.turnOffMotors();
}

void loop() {
    delay(10);
    Enes100.println("Hello, World! (from loop)");
    delay(1000);
    
    int cont = 0;
  //  Tank.begin();
    

    
    
    while(!(Enes100.getTheta() > 1.37 && Enes100.getTheta() < 1.77))
    {
        Enes100.println(Enes100.getTheta());
    //while(Enes100.getTheta() - 1 < .1 && Enes100.getTheta() - 1 > -0.1){
        Tank.begin();
        Tank.setLeftMotorPWM(25); 
        Tank.setRightMotorPWM(-25);
        
    }
    
    Tank.setLeftMotorPWM(0);
    Tank.setRightMotorPWM(0);
    
    if(Enes100.getTheta() < 1.57){
       while(!(Enes100.getTheta() > 1.56 && Enes100.getTheta() < 1.59)) {
            Tank.setLeftMotorPWM(-2); 
            Tank.setRightMotorPWM(2);
       }
    }
    else{
       while(!(Enes100.getTheta() > 1.56 && Enes100.getTheta() < 1.59)) {
           Tank.setLeftMotorPWM(2); 
            Tank.setRightMotorPWM(-2);
       }
    }

    
}
