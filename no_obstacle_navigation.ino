#include "Arduino.h"
#include "Enes100.h"
#include "Tank.h"

// ----------------------------------------------------------
// Helper functions
// ----------------------------------------------------------

// Inline absolute value function for floats.
float myAbs(float x) {
  return (x < 0) ? -x : x;
}

/**
 * @brief Normalize an angle (in radians) to the range [-PI, PI].
 */
float normalizeAngle(float theta) {
  while (theta <= -PI) theta += 2.0 * PI;
  while (theta >  PI)  theta -= 2.0 * PI;
  return theta;
}

/**
 * @brief Compute the normalized difference (target - current) in the range [-PI, PI].
 */
float angleDiff(float current, float target) {
  float diff = target - current;
  while (diff <= -PI) diff += 2.0 * PI;
  while (diff >  PI)  diff -= 2.0 * PI;
  return diff;
}

/**
 * @brief Return true if current heading is within 'tolerance' radians of target.
 */
bool closeEnough(float current, float target, float tolerance) {
  float diff = angleDiff(current, target);
  return (myAbs(diff) < tolerance);
}

// ----------------------------------------------------------
// Rotation Methods (Swapped Names)
// ----------------------------------------------------------

/**
 * @brief Rotate the tank right-to-angle.
 * 
 * NOTE: This method was formerly called rotateLeftToAngle(). 
 * It now has the name rotateRightToAngle() and makes the tank turn clockwise.
 * That is, it sets the left motor to run backward and the right motor to run forward.
 */
void rotateRightToAngle(float targetTheta, float coarseTolerance, float finalTolerance, int coarsePWM, int finePWM) {
  Tank.begin();
  
  while (!closeEnough(normalizeAngle(Enes100.getTheta()), targetTheta, finalTolerance)) {
    float currentTheta = normalizeAngle(Enes100.getTheta());
    float diff = angleDiff(currentTheta, targetTheta);
    float absDiff = myAbs(diff);
    int pwmSpeed = (absDiff > coarseTolerance) ? coarsePWM : finePWM;
    
    // Turning clockwise:
    Tank.setLeftMotorPWM(-pwmSpeed);
    Tank.setRightMotorPWM(pwmSpeed);
    delay(20);
  }
  
  // Stop motors once target is reached.
  Tank.setLeftMotorPWM(0);
  Tank.setRightMotorPWM(0);
}

/**
 * @brief Rotate the tank left-to-angle.
 * 
 * NOTE: This method was formerly called rotateRightToAngle().
 * It now has the name rotateLeftToAngle() and makes the tank turn counterclockwise.
 * That is, it sets the left motor to run forward and the right motor to run backward.
 */
void rotateLeftToAngle(float targetTheta, float coarseTolerance, float finalTolerance, int coarsePWM, int finePWM) {
  Tank.begin();
  
  while (!closeEnough(normalizeAngle(Enes100.getTheta()), targetTheta, finalTolerance)) {
    float currentTheta = normalizeAngle(Enes100.getTheta());
    float diff = angleDiff(currentTheta, targetTheta);
    float absDiff = myAbs(diff);
    int pwmSpeed = (absDiff > coarseTolerance) ? coarsePWM : finePWM;
    
    // Turning counterclockwise:
    Tank.setLeftMotorPWM(pwmSpeed);
    Tank.setRightMotorPWM(-pwmSpeed);
    delay(20);
  }
  
  // Stop motors once target is reached.
  Tank.setLeftMotorPWM(0);
  Tank.setRightMotorPWM(0);
}

// ----------------------------------------------------------
// Main Code
// ----------------------------------------------------------

void setup() {
  Enes100.begin("Simulator", DATA, 3, 8, 9);
  Enes100.println(Enes100.getTheta());
}

void loop() {
  delay(10);
  Enes100.println("Hello, World! (from loop)");
  delay(1000);

  // Read the starting y-coordinate and normalized current heading.
  float startY = Enes100.getY();
  float normTheta = normalizeAngle(Enes100.getTheta());

  // ---------------------------
  // Y-Direction Movement
  // ---------------------------
  if (startY < 1.0) {
    // For y < 1m, target heading is π/2 (~1.57 radians).
    float targetTheta = 1.57;
    
    // If facing closer to 0 (within [-π/2, π/2], i.e. "facing more right"),
    // call rotateRightToAngle() (which turns clockwise) to reach π/2.
    if (normTheta >= -1.57 && normTheta <= 1.57) {
      rotateRightToAngle(targetTheta, 0.20, 0.02, 25, 5);
    }
    // Otherwise (facing more left), call rotateLeftToAngle() (which turns counterclockwise).
    else {
      rotateLeftToAngle(targetTheta, 0.20, 0.02, 25, 5);
    }

    // Move forward until Y >= 1.5.
    while (Enes100.getY() < 1.5) {
      Tank.setLeftMotorPWM(100);
      Tank.setRightMotorPWM(100);
      delay(20);
    }
  }
  else {
    // For y > 1m, target heading is -π/2 (~ -1.57 radians).
    float targetTheta = -1.57;
    
    // If facing closer to 0 (within [-π/2, π/2], i.e. "facing more right"),
    // call rotateLeftToAngle() (which turns counterclockwise) to reach -π/2.
    if (normTheta >= -1.57 && normTheta <= 1.57) {
      rotateLeftToAngle(targetTheta, 0.20, 0.02, 25, 5);
    }
    // Otherwise (facing more left), call rotateRightToAngle() (which turns clockwise).
    else {
      rotateRightToAngle(targetTheta, 0.20, 0.02, 25, 5);
    }

    // Move forward until Y <= 0.5.
    while (Enes100.getY() > 0.5) {
      Tank.setLeftMotorPWM(100);
      Tank.setRightMotorPWM(100);
      delay(20);
    }
  }

  // Stop motors after completing the Y-direction move.
  Tank.setLeftMotorPWM(0);
  Tank.setRightMotorPWM(0);

  // ---------------------------
  // Rotation to Face X-Direction (Angle 0)
  // ---------------------------
  if (startY < 1.0) {
    rotateLeftToAngle(0, 0.20, 0.02, 25, 5);
  } else {
    rotateRightToAngle(0, 0.20, 0.02, 25, 5);
  }

  // ---------------------------
  // X-Direction Movement
  // ---------------------------
  // Move forward until x reaches 3.2m.
  while (Enes100.getX() < 3.2) {
    Tank.setLeftMotorPWM(100);
    Tank.setRightMotorPWM(100);
    delay(20);
  }

  // Stop motors after reaching x = 3.2m.
  while(Enes100.getX() > 1){
      Tank.setLeftMotorPWM(0);
      Tank.setRightMotorPWM(0);
  }

  // Delay before the next iteration.
  delay(20000);
}
