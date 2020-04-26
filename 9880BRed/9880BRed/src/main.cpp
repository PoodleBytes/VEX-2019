/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Cod3d                                          */
/*    Created:      Mon Oct 21 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// driveRF              motor         20
// driveLF              motor         10
// R_Lift               motor         6
// L_Lift               motor         7
// L_Intake             motor         4
// R_Intake             motor         5
// Stand                motor         8
// C_Intake             motor         11
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// Creates a competition object that allows access to Competition methods.
vex::competition Competition;

// FUNCTIONS
void pre_auton(void); // SETTINGS FOR MOTORS, SCREEN ETC
void setUpMotor(motor(M));
int updateScreen(); // DISPLAY ENCODER TASK


// VARItABLES
double adjSpeed = 1.0; // adjust drive sensitivity
double adjField = 1.0; // adjust automouse drive() degrees match conditions

// MOVEMENT / CONTROL
int tLift();                                 // arm-relatecd tasks / buttons
void aDrive(double, double, int, int, bool); // DRIVE BY RELATIVE DISTANCE
void aLift(double, int, bool);               // ARM BY RELATIVE DISTANCE
void aIntake(double, int, bool);             // INTAKE RELATIVE DISTANCE
void aStand(double, int, bool);              // STAND BY RELATIVE DISTANCE
motor_group Intake(spinL, spinR);
motor_group Lift(liftL, liftR);
motor_group Drive(driveLF, driveRF);

void autonomous(void) {

  Intake.setVelocity(90, pct);
  Intake.spin(reverse);
  
  jointTurn.stop(coast);
  aDrive(1100, 1100, 15, 15, 1);
  Intake.stop(hold);
  aDrive(-740, -740, 25, 25, 1);
  Drive.stop(brake);
  aDrive(350, -350, 25, 25, 1); // TURN
  Drive.stop(brake);
  aDrive(450, 450, 25, 25, 1); // drive to corner
  Drive.stop(coast);
  aStand(-600, 95, 1);
  Intake.setVelocity(10, pct);
  wait(10, msec);
  Intake.stop(brake);
  jointTurn.stop(hold);
  aDrive(70, 70, 30, 30, 1);
  Drive.stop(coast);
  Intake.setVelocity(10, pct);
  wait(15, msec);
  Intake.stop(brake);
  aDrive(-500, -500, 25, 25, 1);
  jointTurn.stop(coast);
  Intake.stop(coast);
  Lift.stop(coast);
  Drive.stop(coast);

  // Start of Skills Auto
  /*aStand(30, 30,1);
  aLift(-200, 80, 1);
  Intake.setVelocity(47, pct);
  Intake.spin(fwd);
  aLift(200, 60, 1);
  Stand.stop(coast);
  aDrive(2260, 2260, 25, 25, 1);*/
}
// end autonomous

void usercontrol(void) {
  // ONE TIME COMMANDS RAN B4 USER CONTROL

  // START TASK for LIFT
  vex::task t(tLift); // start task which updates controller screen
  t.setPriority(6);   // medium priority

  vex::task upScr(updateScreen); // start task
  upScr.setPriority(4);          // set low priority

  while (1) {

    /*if (abs(Controller1.Axis1.value() + Controller1.Axis2.value()) < 119 &&
        abs(Controller1.Axis1.value() - Controller1.Axis2.value()) < 119) {
      adjSpeed = 0.25;
      
    }*/
      adjSpeed = 1.0;
    // DRIVE
    driveLF.spin(vex::directionType::fwd,
                 (Controller1.Axis3.position() + Controller1.Axis4.position()) * 0.5,
                 vex::velocityUnits::pct);
    driveRF.spin(vex::directionType::fwd,
                 (Controller1.Axis3.position() - Controller1.Axis4.position()) * 0.5,
                 vex::velocityUnits::pct);
    vex::task::sleep(100); // Sleep the task for a short amount of time to
                           // prevent wasted resources.

  } // end while
}

int tLift(void) { // ARM & Stand TASK L2= out take  R2 = intake   L1 = up  R1 = down
  while (1) {
    if (Controller1.ButtonR2.pressing()) {
      Intake.spin(vex::directionType::fwd, 70, vex::velocityUnits::pct);

    } else if (Controller1.ButtonR1.pressing()) {
      Intake.spin(vex::directionType::rev, 70, vex::velocityUnits::pct);

    } else if (Controller1.ButtonL2.pressing() ) {
      Lift.spin(vex::directionType::fwd, 40, vex::velocityUnits::pct);

    } else if (Controller1.ButtonL1.pressing() ) {
      Lift.spin(vex::directionType::rev, 30, vex::velocityUnits::pct);

    } else if (Controller1.ButtonDown.pressing()) {
      jointTurn.spin(vex::directionType::fwd, 85, vex::velocityUnits::pct);

    } else if (Controller1.ButtonUp.pressing()) {
      jointTurn.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
      //Lift.spin(vex::directionType::fwd,40,velocityUnits::pct);
      Intake.stop(brakeType::coast);

    } else if (Controller1.ButtonX.pressing()) {
      Drive.spin(vex::directionType::fwd, 17, vex::velocityUnits::pct);

    } else if (Controller1.ButtonB.pressing()) {
      Drive.spin(vex::directionType::rev, 17, vex::velocityUnits::pct);
      jointTurn.stop(brakeType::coast);

    
    } 

    else {
      if (Lift.rotation(deg) < -20) {
        Lift.stop(brakeType::hold);
      } else if (Lift.rotation(deg) > -20) {
        Lift.stop(brakeType::coast);
      }
      if (jointTurn.rotation(deg) > -200) {
        jointTurn.stop(brakeType::coast);
      } else if (jointTurn.rotation(deg) < -80) {
        jointTurn.stop(brakeType::hold);
      }
      Intake.stop(brakeType::hold);
    }
    vex::task::sleep(20);
  }
  return 0;
} // end tArm




void aDrive(double lDeg, double rDeg, int l, int r,
            bool b) { // drive by relative distance

  driveLF.rotateFor(lDeg * adjField, vex::rotationUnits::deg, l,
                    vex::velocityUnits::pct,
                    false); // This command must be non blocking.
  driveRF.rotateFor(rDeg * adjField, vex::rotationUnits::deg, r,
                    vex::velocityUnits::pct,
                    false); // This command must be non blocking.

  if (b) { // b = tue means wait for motors stop spinning or timeout
    while (driveRF.isSpinning() || driveLF.isSpinning()) {
    }
  }
} // end

void aLift(double deg, int s, bool b) {
  liftL.rotateFor(deg, vex::rotationUnits::deg, s, vex::velocityUnits::pct,
                  false); // This command must be non blocking.
  liftR.rotateFor(deg, vex::rotationUnits::deg, s, vex::velocityUnits::pct,
                  b); // This command is blocking so the program will wait
                      // here until the right motor is done.

  if (b) { // b = tue means wait for motors stop spinning or timeout
    while (liftR.isSpinning() || liftL.isSpinning()) {
    }
  }
} // end

void aIntake(double deg, int s, bool b) {
  spinL.rotateFor(deg, vex::rotationUnits::deg, s, vex::velocityUnits::pct,
                  false); // This command must be non blocking.
  spinR.rotateFor(deg, vex::rotationUnits::deg, s, vex::velocityUnits::pct,
                  b); // This command is blocking so the program will wait
                      // here until the right motor is done.

  if (b) { // b = tue means wait for motors stop spinning or timeout
    while (spinR.isSpinning() || spinL.isSpinning()) {
    }
  }
} // end

void aStand(double deg, int s, bool b) {
  jointTurn.rotateFor(deg, vex::rotationUnits::deg, s, vex::velocityUnits::pct,
                      b);
}

int updateScreen() {
  while (1) {
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print(
        "Left: %4.1f", driveLF.rotation(rotationUnits::deg)); // LEFT MOTOR
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print(
        "Right: %4.1f", driveRF.rotation(rotationUnits::deg)); // RIGHT MOTOR
    Controller1.Screen.setCursor(3, 1);
    Controller1.Screen.print(
        "L: %4.2f S: %4.2f", liftL.rotation(rotationUnits::deg),
        jointTurn.rotation(rotationUnits::deg)); // LIFT AND STAND

    // RESET ENCODERS
    if (Controller1.ButtonA.pressing()) { // DRIVE ENCODERS
      driveLF.resetRotation();
      driveRF.resetRotation();
    }
    if (Controller1.ButtonY.pressing()) { // ARM AND GRABBER ENCODERS
      liftL.resetRotation();
      jointTurn.resetRotation();
    }

    // don't hog the cpu :)
    vex::this_thread::sleep_for(250);

  } // end while
  return 0;
} // END UPDATE SCREEN

// SYSTEM SET-UP
void pre_auton(void) {
  // set motor defayults
  setUpMotor(driveLF);
  setUpMotor(driveRF);
  setUpMotor(liftL);
  setUpMotor(liftR);
  setUpMotor(spinL);
  setUpMotor(spinR);
  setUpMotor(jointTurn);

  // clear controller display
  Controller1.Screen.clearScreen();
} // end pre_auton

// DEFAULT MOTOR SETTINGS
void setUpMotor(motor(M)) {
  M.setMaxTorque(90, percentUnits::pct); // set torque
  M.setBrake(brakeType::coast);          // braking
  M.setTimeout(3, timeUnits::sec);       // set motor timeout
  M.resetRotation();                     // reset encoders
}

// Main will set up the competition functions and callbacks.
int main() {
  // Run the pre-autonomous function.
  pre_auton();
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  // Prevent main from exiting with an infinite loop.
  while (1) {
    vex::task::sleep(100); // Sleep the task for a short amount of time to
                           // prevent wasted resources.
  }
}
