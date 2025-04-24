#include <Arduino.h>
#include "Enes100.h"




const int MOTOR1A = 4;  // Subject to change
const int MOTOR1B = 2;  // Subject to change
const int MOTOR1C = A2;  // Subject to change
const int MOTOR2C = A1;  // Subject to change
const int MOTOR2A = 3;  // Subject to change
const int MOTOR2B = 5;  // Subject to change
const int USTrigger = 7; // ^^^
const int USEcho = 8;
const int WifiTX = 1;
const int WifiRX = 0;







int lowPosition = 0;
int mediumPosition = 1;
int highPosition = 2;

//true if it started in the bottom square, false if it started in the top
bool startedAtBottom;

int sensorID = 5;

//Things that need to happen. All start false. As they get done set to true

//Got to material
bool reachedOtherSquare;
bool spunToZero;




//Will be used for sensor conditions.********

 //There is no obstacle right in front of the otv
boolean conditionOne;
boolean conditionTwo;
        
//Has not passed any set of obstacles
boolean conditionThree;
boolean conditionFour;
boolean facingForward;


void setup() {
        
    pinMode(MOTOR1A, OUTPUT);
    pinMode(MOTOR1B, OUTPUT);
    pinMode(MOTOR2A, OUTPUT);
    pinMode(MOTOR2B, OUTPUT);
    pinMode(MOTOR2C, OUTPUT);
    pinMode(MOTOR1C, OUTPUT);
    pinMode(USTrigger, OUTPUT);
    pinMode(USEcho, INPUT);
    //The arduino will either start in the bottom or top square randomly
    Enes100.begin("Material Motion", MATERIAL, 6, 1120, WifiTX, WifiRX);
    startedAtBottom = false;
    
    if(Enes100.getY() < 1)
    {
        startedAtBottom = true;
    }
    
    reachedOtherSquare = false;
    
    spunToZero = false;
    
    //SET SENSOR conditions
    conditionOne = !(readDistanceSensor(sensorID) < 0.3);
    conditionTwo = (readDistanceSensor(sensorID) < 0);
    
    conditionThree = !passedFirstObstacleSet();
    conditionFour = !passedSecondObstacleSet();
    
    facingForward = (Enes100.getTheta() < 0.1 && Enes100.getTheta() > -0.1);
    
}

void loop() {
    delay(10);
    
    
    int cont = 0;
    
    //If it hasnt reached the other square yet
    if(!reachedOtherSquare)
    {
        //If it starts at the low position, spin to theta = pi/2
        //If it starts at high spin to theta = -pi/2
        if(startedAtBottom)
        {
            spinTohalfPi();
        }
        else
        {
            spinToNegativeHalfPi();
            
        }
        // end of started up/down if else statement
        
        //Moves to the other square if the otv has 
        moveToOtherSquare();
        
        
        //Move toward first set of obstacles
        
    }
    
    
    if(!spunToZero)
    {
        //After reaching the other square, stop the otv
        stopOTV();
        
        spinToZero();
        
        //To prevent method form continuing to run
        stopOTV();
        
        spunToZero = true;
    }
    
    //The otv has now reached the other side
    
    updateSensorConditions();

    //This happens before the first set of obstacles
    if(!passedFirstObstacleSet() && !passedSecondObstacleSet() && facingForward)
    {
           
        //TO SHOW WHAT UPDATESENDORCONDITIONS DOES
        //There is no obstacle right in front of the otv
        //conditionOne = !(readDistanceSensor(sensorID) < 0.3);
        //conditionTwo = (readDistanceSensor(sensorID) < 0);
        
        //It hasnt passed the first second obstacle set
        //conditionThree = !passedFirstObstacleSet();
       // conditionFour = !passedSecondObstacleSet();
       
       updateSensorConditions();
        
         while((conditionOne || conditionTwo) && conditionThree && conditionFour
         && readBumpSensor(3) == false)
         {
                moveForward(25);
                //// println(readDistanceSensor(sensorID));
                
                updateSensorConditions();
                
                //// println(conditionThree);
         }
         
        stopOTV();
    }

    boolean spunOnce = false;
    boolean spunSecondTime = false;



    //If the otv disocvers obstacles after the first set, it needs 
    //move down or up (depending on where it started) and check if other slots
    //are free
    //This if for the first obstacle set
    if(((readDistanceSensor(sensorID) < 0.3  && 
    !(readDistanceSensor(sensorID) < 0)) || readBumpSensor(3) == true))
    {
        
        if(readBumpSensor(3) == true)
        {
            //moves backward
            // println("MOVING BACKWARD");
            
            moveBackward(25);
            
            delay(1000);
            
            stopOTV();
            
        }
        
        navigateToMiddle();
        
        //temporaraly negates this variable to spin to 0 more effectively in this case
        startedAtBottom = !startedAtBottom;
        
  
  //If spunonce
            spinToZero();
            stopOTV();
        
        spunOnce = true;
        
        // // println("HERE!!");
        // // println(readDistanceSensor(sensorID));
        
        //If there is still an obstacle there, go to bottom and proceed forward
        if(readDistanceSensor(sensorID) < 0.3 && !(readDistanceSensor(sensorID) < 0))
        {
            navigateToBottom;
            
            if(!spunSecondTime)
            {
                spinToZero();
                stopOTV();
            }

            spunSecondTime = true;
            
        }
        else
        {
            //In this case, the middle obstacle of the 1st set is free
            
            updateSensorConditions();
            
            
            //conditoonone && condition 2 = distance sensor blocked
            
           
            //At this point, we have moved past the first obstacle set but not
            //Passed the second. Either there was a obstacle there or we are just 
            //past it. THIS IS NOW FOR THE SECOND SET OF OBSTSTACLES
            goToSecondObstacleSet();
            
            //If there is an obstacle at the middle here
            if(readDistanceSensor(sensorID) < 0.3 && 
            !(readDistanceSensor(sensorID) < 0))
            {
                navigateToTop(); 
                spinToZero();
                
                if(readDistanceSensor(sensorID) < 0.3)
                {
                    navigateToBottom();
                  
                    goToEnd();
                }
                else
                {
                    goToEnd();
                }
            }
            else
            {
                goToEnd();
            }
        }
    }
    else
    {
        goToSecondObstacleSet();
        
        if(readDistanceSensor(sensorID) < 0.3 && !(readDistanceSensor(sensorID) < 0))
        {
            navigateToMiddle();
            spinToZero();
            
            if(readDistanceSensor(sensorID) < 0.3 && !(readDistanceSensor(sensorID) < 0))
            {
                navigateToBottom();
                spinToZero();
                goToEnd();
            }
        }
        else
        {
            goToEnd();
        }
    }
    
}

//HELPER METHODS HERE
void goToEnd()
{
    spinToZero();
    
    while(Enes100.getX() < 2.9)
    {
        moveForward(30);
    }
    
    //If were not already at the top half we go there an spin back to 0
    if(Enes100.getY() < 1.2)
    {
        navigateToTop();
        spinToZero();
    }
    
    while(Enes100.getX() < 3.6)
    {
        moveForward(30);
    }
}

//Goes up to point where it is at or before 2nd obstacle set,depending if obstacle present
void goToSecondObstacleSet()
{
     while(!passedSecondObstacleSet() && (conditionOne || conditionTwo))
    {
        // // println("MOVING TO SECOND OBSTACLE SET");
        // // println(readBumpSensor(3));
        moveForward(30);
    }
            
    
}

void navigateToTop()
{
    spinTohalfPi();
    
    while(Enes100.getY() < 1.5)
    {
        moveForward(30);
    }
}

void navigateToBottom()
{
    spinToNegativeHalfPi();
    
    while(Enes100.getY() > 0.5)
    {
        moveForward(30);
    }
}

void navigateToMiddle()
{
    if(Enes100.getY() > 1)
    {
       spinToNegativeHalfPi();
       
       while(Enes100.getY() > 1)
       {
            moveForward(20);
       }

       
       stopOTV();
    }
    else
    {
        spinTohalfPi();
        
        while(Enes100.getY() < 1)
        {
            moveForward(20);
        }

        stopOTV();
    }
} 

void updateSensorConditions()
{    
      //There is no obstacle right in front of the otv
        conditionOne = !(readDistanceSensor(sensorID) < 0.3);
        
        //If the otv is facing up or down we dont want it to start again just because it hasnt hit obstacles
        conditionTwo = (readDistanceSensor(sensorID) < 0);
        
        //Has not passed any set of obstacles
        conditionThree = !passedFirstObstacleSet();
        conditionFour = !passedSecondObstacleSet();
        
        //to make sure it only starts when 
        facingForward = (Enes100.getTheta() < 0.1 && Enes100.getTheta() > -0.1);
    
}

int getPositionNum()
{
    if(Enes100.getY() < 0.73)
    {
        return lowPosition;
    }
    else if(Enes100.getY() < 1.25)
    {
        return mediumPosition;
    }
        
    return highPosition;
}

bool readBumpSensor(int x){

  return false;
    
}

//Moves forward with specified specified specified speed
void moveForward(int speed)
{
    setLeftMotorPWM(speed);
    setRightMotorPWM(speed);
}

void moveBackward(int speed)
{
    int newspeed = -1*speed;
    moveForward(newspeed);
}

//Spints otv to 0 degrees
void spinToZero()
{
    while(!(Enes100.getTheta() < 0.2 && Enes100.getTheta() > -0.2))
    {
        if(startedAtBottom)
        {
            setLeftMotorPWM(25);
            setRightMotorPWM(-25);
        }
        else
        {
            setLeftMotorPWM(-25);
            setRightMotorPWM(25);
        }
    }
    
    if(Enes100.getTheta() < 0)
    {
        while(!(Enes100.getTheta() > -0.01)) 
        {
            setLeftMotorPWM(-2); 
            setRightMotorPWM(2);
        }
    }
    else
    {
        while(!(Enes100.getTheta() < 0.01)) 
        {
            setLeftMotorPWM(2); 
            setRightMotorPWM(-2);
        }
               
    }
    
}

void spinTohalfPi()
{
        while(!(Enes100.getTheta() > 1.3 && Enes100.getTheta() < 1.85))
        {
            // println(Enes100.getTheta());
          
            setLeftMotorPWM(40); 
            setRightMotorPWM(-40);
                
        }
            
        setLeftMotorPWM(0);
        setRightMotorPWM(0);
            
        if(Enes100.getTheta() < 1.57 && Enes100.getTheta() > 1.569)
        {
            while(!(Enes100.getTheta() > 1.56)) 
            {
                setLeftMotorPWM(-2); 
                setRightMotorPWM(2);
            }
        }
        else
        {
            while(!(Enes100.getTheta() < 1.58)) 
            {
                setLeftMotorPWM(2); 
                setRightMotorPWM(-2);
            }
               
        }
}

bool passedSecondObstacleSet()
{
    if(Enes100.getX() > 1.9)
    {
        return true;
    }
    
    return false;
}

bool passedFirstObstacleSet()
{
    if(Enes100.getX() > 1.3)
    {
        return true;
    }
    
    return false;
}

void spinToNegativeHalfPi()
{
    while(!(Enes100.getTheta() < -1.3 && Enes100.getTheta() > -1.85))
    {
        // println(Enes100.getTheta());
          
        setLeftMotorPWM(40); 
        setRightMotorPWM(-40);
                
    }
            
    setLeftMotorPWM(0);
    setRightMotorPWM(0);
            
    if(Enes100.getTheta() > -1.571 && Enes100.getTheta() < -1.569)
    {
        while(!(Enes100.getTheta() < -1.56)) 
        {
                setLeftMotorPWM(2); 
                setRightMotorPWM(-2);
        }
        
    }
    else
    {
        while(!(Enes100.getTheta() > -1.58)) 
        {
            setLeftMotorPWM(-2); 
            setRightMotorPWM(2);
        }
               
    }   
}

void moveToOtherSquare()
{
    if(startedAtBottom)
    {
        while(Enes100.getY() < 1.45)
        {
            setRightMotorPWM(45);
            setLeftMotorPWM(45); 
        }
            
        reachedOtherSquare = true;
    }
    else
    {
        while(Enes100.getY() > 0.55)
        {
            setRightMotorPWM(45);
            setLeftMotorPWM(45); 
        }
            
        reachedOtherSquare = true;
    }

}

bool Close(double val, double targetVal)
{
    if(val < targetVal + 0.05 && val > targetVal -0.05)
    {
        return true;
    }
    
    return false;
}

float readDistanceSensor(int num){


    // Ensure trigger pin is LOW before starting
    digitalWrite(USTrigger, LOW);
    delayMicroseconds(2);
    
    // Send a 10 microsecond HIGH pulse to trigger the sensor
    digitalWrite(USTrigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(USTrigger, LOW);
    
    // Read the duration of the incoming pulse on the echo pin (timeout 30ms)
    long duration = pulseIn(USEcho, HIGH, 30000);
    
    // Calculate the distance in centimeters:
    // Sound speed ~ 0.034 cm/microsecond; divide by 2 for round-trip.
    float distance = (duration * 0.034) / 2.0;
    return distance;

}


void setLeftMotorPWM(int speed){

	if (speed >= 0) { //Positive 

		digitalWrite(MOTOR1A, LOW);
		digitalWrite(MOTOR1B, HIGH);
		digitalWrite(MOTOR1C, speed);

	} else { //Negative

		digitalWrite(MOTOR1A, HIGH);
		digitalWrite(MOTOR1B, LOW);
		digitalWrite(MOTOR1C, -1*speed);	

	}


}

void setRightMotorPWM(int speed){

	if (speed >= 0) { //Positive 

		digitalWrite(MOTOR2A, HIGH);
		digitalWrite(MOTOR2B, LOW);
		digitalWrite(MOTOR2C, speed);

	} else { //Negative

		digitalWrite(MOTOR2A, LOW);
		digitalWrite(MOTOR2B, HIGH);
		digitalWrite(MOTOR2C, -1*speed);	

	}


}

void stopOTV(){

	digitalWrite(MOTOR2A, HIGH);
	digitalWrite(MOTOR2B, HIGH);

	digitalWrite(MOTOR1A, HIGH);
	digitalWrite(MOTOR1B, HIGH);

}

