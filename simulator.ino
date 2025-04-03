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
 * @brief Normalize the difference (target - current) to always be in [-PI, PI].
 *        This ensures we rotate in the shortest direction.
 */
float angleDiff(float current, float target) {
  float diff = target - current;

  // Bring diff into the range [-PI, PI]
  while (diff <= -PI) diff += 2.0 * PI;
  while (diff >   PI) diff -= 2.0 * PI;

  return diff;
}

/**
 * @brief Return true if current heading is within 'tolerance' radians of target.
 */
bool closeEnough(float current, float target, float tolerance) {
  float diff = angleDiff(current, target);
  return (myAbs(diff) < tolerance);
}

/**
 * @brief Rotate the tank to the desired target angle. Uses two speeds:
 *        - coarsePWM for large angle errors
 *        - finePWM for small angle errors
 *        Keeps turning until we are within 'finalTolerance' radians of target.
 *        Now, once the rotation direction is determined it is fixed,
 *        preventing the logic from flipping directions on overshoot.
 */
void rotateToAngle(float targetTheta, float coarseTolerance, float finalTolerance, int coarsePWM, int finePWM) 
{
  Tank.begin();
  
  // Determine the initial rotation direction and "lock it in"
  int direction = 0; // +1 for CCW (left), -1 for CW (right)
  float currentTheta = Enes100.getTheta();
  float initialDiff = angleDiff(currentTheta, targetTheta);
  direction = (initialDiff >= 0) ? 1 : -1;
  
  // Keep rotating until we're within finalTolerance
  while(!closeEnough(Enes100.getTheta(), targetTheta, finalTolerance)) {
    currentTheta = Enes100.getTheta();
    float diff = angleDiff(currentTheta, targetTheta);   // difference in [-PI, PI]
    // Use absolute difference
    float absDiff = (diff < 0 ? -diff : diff);
    int pwmSpeed = (absDiff > coarseTolerance) ? coarsePWM : finePWM;

    // Use the locked direction instead of rechecking diff sign
    if (direction > 0) {
      // Rotate counterclockwise: left motor forward, right motor backward
      Tank.setLeftMotorPWM(pwmSpeed);
      Tank.setRightMotorPWM(-pwmSpeed);
    } else {
      // Rotate clockwise: left motor backward, right motor forward
      Tank.setLeftMotorPWM(-pwmSpeed);
      Tank.setRightMotorPWM(pwmSpeed);
    }
    
    delay(20);  // Allow time for angle update
  }

  // Stop after finishing
  Tank.setLeftMotorPWM(0);
  Tank.setRightMotorPWM(0);
}

// ----------------------------------------------------------
// Main code
// ----------------------------------------------------------

void setup() {
  Enes100.begin("Simulator", DATA, 3, 8, 9);
  Enes100.println(Enes100.getTheta());
}

void loop() {
  // Basic print
  delay(10);
  Enes100.println("Hello, World! (from loop)");
  delay(1000);

  // Read the starting y-coordinate once at the beginning
  float startY = Enes100.getY();

  // Decide orientation and translation based on starting Y
  if (startY < 1.0) {
    // ------------------------------------------------------
    // 1) Rotate to face upward (~ +1.57 radians)
    // ------------------------------------------------------
    rotateToAngle(
      /*targetTheta=*/1.57, 
      /*coarseTolerance=*/0.20, 
      /*finalTolerance=*/0.02, 
      /*coarsePWM=*/25, 
      /*finePWM=*/5
    );

    // ------------------------------------------------------
    // 2) Move forward (increasing Y) until Y >= 1.5
    // ------------------------------------------------------
    while(Enes100.getY() < 1.5) {
      Tank.setLeftMotorPWM(100);
      Tank.setRightMotorPWM(100);
      delay(20);
    }
  }
  else {
    // ------------------------------------------------------
    // 1) Rotate to face downward (~ -1.57 radians)
    // ------------------------------------------------------
    rotateToAngle(
      /*targetTheta=*/-1.57, 
      /*coarseTolerance=*/0.20, 
      /*finalTolerance=*/0.02, 
      /*coarsePWM=*/25, 
      /*finePWM=*/5
    );

    // ------------------------------------------------------
    // 2) Move forward (decreasing Y) until Y <= 0.5
    // ------------------------------------------------------
    while(Enes100.getY() > 0.5) {
      Tank.setLeftMotorPWM(100);
      Tank.setRightMotorPWM(100);
      delay(20);
    }
  }

  // Stop motors after reaching the final Y
  Tank.setLeftMotorPWM(0);
  Tank.setRightMotorPWM(0);

  // Add a delay so we don't repeat immediately (or you can remove this)
  delay(20000);
}
