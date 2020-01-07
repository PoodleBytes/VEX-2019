#include "vex.h"

void rDrive(double lDeg, double rDeg, double l, double r, bool b) { // drive by relative distance
  rDeg = rDeg * -1;
  L_Drive.rotateFor(lDeg * adjField, vex::rotationUnits::deg, l,vex::velocityUnits::pct, false);
  R_Drive.rotateFor(rDeg * adjField, vex::rotationUnits::deg, r,vex::velocityUnits::pct, b);
} // end rDrive

void sDrive(double lValue, double rValue) { // drive by spin
  lValue = lValue * -1;
  L_Drive.spin(vex::directionType::fwd, lValue, vex::velocityUnits::pct);
  R_Drive.spin(vex::directionType::fwd, rValue, vex::velocityUnits::pct);
} // end rDrive

void aLift(double deg, double s, bool b) { // position lift by absolute position
  Lift.rotateTo(deg, vex::rotationUnits::deg, s, vex::velocityUnits::pct,false); // This command must be non blocking.

  if (b) { // b = tue means wait for motors stop spinning or timeout
    while (Lift.isSpinning()) {
    }
  }
} // end aArm

void rLift(double deg, double s, bool b) { // position lift by relative position
  Lift.rotateFor(deg, vex::rotationUnits::deg, s, vex::velocityUnits::pct,false); // This command must be non blocking.

  if (b) { // b = tue means wait for motors stop spinning or timeout
    while (Lift.isSpinning()) {
    }
  }
} // end eArm

int updateScreen() {
  while (1) {
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print(
        "Left: %4.1f", L_Drive.rotation(rotationUnits::deg)); // LEFT MOTOR
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print(
        "Right: %4.1f", R_Drive.rotation(rotationUnits::deg)); // RIGHT MOTOR
    Controller1.Screen.setCursor(3, 1);
    Controller1.Screen.print(
        "L: %4.2f S: %4.2f", L_Lift.rotation(rotationUnits::deg),
        Claw.rotation(rotationUnits::deg)); // LIFT AND STAND

    // RESET ENCODERS
    if (Controller1.ButtonLeft.pressing()) { // DRIVE ENCODERS
      L_Drive.resetRotation();
      R_Drive.resetRotation();
    }
    if (Controller1.ButtonRight.pressing()) { // ARM AND GRABBER ENCODERS
      L_Lift.resetRotation();
      Claw.resetRotation();
    }

    // don't hog the cpu :)
    vex::this_thread::sleep_for(250);

  } // end while
  return 0;
} // END UPDATE SCREEN

void closeClaw(double s) {  
  do {
    Claw.spin(vex::directionType::fwd, s, vex::velocityUnits::pct);
  } while (Claw.current(vex::percentUnits::pct) < 50);
  Claw.stop(hold);
} // end rClaw

void openClaw(void) {
  Claw.setVelocity(75, pct);
  Claw.spinTo(0, deg); // spin to home position
  Claw.stop(brake);
} // end rClaw

void homeClaw(void){
  Claw.rotateFor(40, vex::rotationUnits::deg, 50, vex::velocityUnits::pct,true);  //slightly close claw
  Lift.rotateFor(42, vex::rotationUnits::deg, 30, vex::velocityUnits::pct,true); //lift claw to mid-cube
  Lift.resetRotation();   //make position home
  Claw.resetRotation();   //make position home 
}//end  

// DEFAULT MOTOR SETTINGS
void setUpMotor(motor(M), char B) {
  M.setMaxTorque(90, percentUnits::pct); // set torque
  M.setTimeout(3, timeUnits::sec);       // set motor timeout
  M.resetRotation();                     // reset encoders
  if (B == 'C') {
    M.setStopping(brakeType::coast); // coast
  } else if (B == 'H') {
    M.setStopping(brakeType::hold); // braking
  } else if (B == 'B') {
    M.setStopping(brakeType::brake); // braking
  } else {
    M.setStopping(brakeType::coast); // braking
  }
}

