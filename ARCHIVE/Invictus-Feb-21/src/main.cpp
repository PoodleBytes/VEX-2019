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
// Drivetrain           drivetrain    2, 3, 20        
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// Creates a competition object that allows access to Competition methods.
vex::competition Competition;

// LONNIE - YOU HAVE TO SET WHEELBAS AND TRACK ON LINE 23 IN ROBOT-CONDIF.CPP

void autonomous(void) { 
 
  DriveTrain.driveFor(forward, 12, distanceUnits::in, 40, velocityUnits::pct);
  DriveTrain.turnFor(right, 90, rotationUnits::deg);
  
}
 // end autonomous

void usercontrol(void) {
  // ONE TIME COMMANDS RAN B4 USER CONTROL

  // START TASK for LIFT
  vex::task t(tLift); // start task which updates controller screen
  t.setPriority(6);   // medium priority

  while (1) {

    if (abs(Controller1.Axis1.value() + Controller1.Axis2.value()) < 120 &&
        abs(Controller1.Axis1.value() - Controller1.Axis2.value()) < 120) {
      adjSpeed = 0.8;
    } else
      adjSpeed = 1.0;
    // DRIVE
    L_Drive.spin(vex::directionType::fwd,
                 (Controller1.Axis2.position() + Controller1.Axis1.position()) *
                     adjSpeed * 1,
                 vex::velocityUnits::pct);
    R_Drive.spin(vex::directionType::fwd,
                 (Controller1.Axis2.position() - Controller1.Axis1.position()) *
                     adjSpeed * 1,
                 vex::velocityUnits::pct);
    vex::task::sleep(100); // Sleep the task for a short amount of time to
                           // prevent wasted resources.

  } // end while
}

int tLift(
    void) { // ARM & Stand TASK L2= out take  R2 = intake   L1 = up  R1 = down
  while (1) {
    if (Controller1.ButtonR1.pressing() && L_Lift.rotation(deg) > -50 ) {
      Intake.spin(vex::directionType::fwd, 90, vex::velocityUnits::pct);
      Lift.spin(directionType::fwd ,15 , vex::velocityUnits::pct);

    } else if (Controller1.ButtonR1.pressing() && L_Lift.rotation(deg)< -50) {
      Intake.spin(vex::directionType::fwd, 90, vex::velocityUnits::pct);

    } else if (Controller1.ButtonR2.pressing()) {
      Intake.spin(vex::directionType::rev, 90, vex::velocityUnits::pct);

    } else if (Controller1.ButtonL2.pressing()) {

        Lift.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct);

    } else if (Controller1.ButtonL1.pressing()) {
        while(L_Lift.rotation(deg) > -410) {
          Lift.spin(vex::directionType::rev, 70, vex::velocityUnits::pct);}

    } else if (Controller1.ButtonDown.pressing()){  
        while(L_Stand.rotation(deg) < -25) {
          Stand.spin(vex::directionType::fwd, 45, vex::velocityUnits::pct);}

    } else if (Controller1.ButtonUp.pressing()) {
      raiseStand();
         Intake.stop(brakeType::coast);

    } else if (Controller1.ButtonX.pressing()) {
      DriveTrain.drive(vex::directionType::fwd, 19, vex::velocityUnits::pct);

    } else if (Controller1.ButtonB.pressing()) {
      DriveTrain.drive(vex::directionType::rev, 30, vex::velocityUnits::pct);
        
    } else if (Controller1.ButtonA.pressing()) {
      unfold();

    } else {
      if (Lift.rotation(deg) < -50) {
        Lift.stop(brakeType::hold);
      } else if (Lift.rotation(deg) > -50) {
        Lift.stop(brakeType::coast);
      }
      if (Stand.rotation(deg) > -100) {
        Stand.stop(brakeType::coast);
      } else if (Stand.rotation(deg) < -40) {
        Stand.stop(brakeType::hold);
      }
      Intake.stop(brakeType::hold);
      
    }
   
    vex::task::sleep(20);
    }
  return 0;
} // end tArm

void raiseStand(void){
    if(Stand.rotation(deg)< -140 ){
     Stand.spin(vex::directionType::rev, 25, vex::velocityUnits::pct);
     Intake.stop(brakeType::coast);
     }
           
  else{ Stand.spin(vex::directionType::rev, 60, vex::velocityUnits::pct);
  
  }  
      
}//end raiseStand

void unfold(){
 aLift(-240, 50, 1);
  Intake.setVelocity(100, pct);
  Intake.spin(directionType::rev); 
  aLift(290, 50, 1);
}

void aDrive(double lDeg, double rDeg, int l, int r,
            bool b) { // drive by relative distance

  L_Drive.rotateFor(lDeg * adjField, vex::rotationUnits::deg, l,
                    vex::velocityUnits::pct,
                    false); // This command must be non blocking.
  R_Drive.rotateFor(rDeg * adjField, vex::rotationUnits::deg, r,
                    vex::velocityUnits::pct,
                    false); // This command must be non blocking.

  if (b) { // b = tue means wait for motors stop spinning or timeout
    while (R_Drive.isSpinning() || L_Drive.isSpinning()) {
    }
  }
} // end

void aLift(double deg, int s, bool b) {
  L_Lift.rotateFor(deg, vex::rotationUnits::deg, s, vex::velocityUnits::pct,
                   false); // This command must be non blocking.
  R_Lift.rotateFor(deg, vex::rotationUnits::deg, s, vex::velocityUnits::pct,
                   b); // This command is blocking so the program will wait
                       // here until the right motor is done.

  if (b) { // b = tue means wait for motors stop spinning or timeout
    while (R_Lift.isSpinning() || L_Lift.isSpinning()) {
    }
  }
} // end

void aIntake(double deg, int s, bool b) {
  L_Intake.rotateFor(deg, vex::rotationUnits::deg, s, vex::velocityUnits::pct,
                     false); // This command must be non blocking.
  R_Intake.rotateFor(deg, vex::rotationUnits::deg, s, vex::velocityUnits::pct,
                     b); // This command is blocking so the program will wait
                         // here until the right motor is done.

  if (b) { // b = tue means wait for motors stop spinning or timeout
    while (R_Intake.isSpinning() || L_Intake.isSpinning()) {
    }
  }
} // end

void aStand(double deg, int s, bool b) {
  Stand.rotateFor(deg, vex::rotationUnits::deg, s, vex::velocityUnits::pct, b);
}

int updateScreen() {
  while (1) {
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print(
        "LD: %4.0f  RD: %4.0F", L_Drive.rotation(rotationUnits::deg), R_Drive.rotation(rotationUnits::deg)); // LEFT MOTOR
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print(
        "Angle: %3.0f", Gyro.angle(deg)); // RIGHT MOTOR
    Controller1.Screen.setCursor(3, 1);
    Controller1.Screen.print(
        "L: %4.2f S: %4.2f", L_Lift.rotation(rotationUnits::deg),
        Stand.rotation(rotationUnits::deg)); // LIFT AND STAND

    // RESET ENCODERS
    if (Controller1.ButtonA.pressing()) { // DRIVE ENCODERS
      L_Drive.resetRotation();
      R_Drive.resetRotation();
    }
    if (Controller1.ButtonY.pressing()) { // ARM AND GRABBER ENCODERS
      L_Lift.resetRotation();
      Stand.resetRotation();
    }

    // don't hog the cpu :)
    vex::this_thread::sleep_for(250);

  } // end while
  return 0;
} // END UPDATE SCREEN

// SYSTEM SET-UP
void pre_auton(void) {

  vex::task upScr(updateScreen); // start task
  upScr.setPriority(4);          // set low priority

  // set motor defayults
  setUpMotor(L_Drive);
  setUpMotor(R_Drive);
  setUpMotor(L_Lift);
  setUpMotor(R_Lift);
  setUpMotor(L_Intake);
  setUpMotor(R_Intake);
  setUpMotor(L_Stand);
  setUpMotor(R_Stand);

  // clear controller display
  Controller1.Screen.clearScreen();

    Brain.Screen.print("Device initialization...");
  Brain.Screen.setCursor(2, 1);
  // calibrate the drivetrain gyro
  wait(200, msec);
  Gyro.calibrate();
  Brain.Screen.print("Calibrating Gyro for Drivetrain");
  // wait for the gyro calibration process to finish
  while (Gyro.isCalibrating()) {
    wait(25, msec);
  }
  // reset the screen now that the calibration is complete
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1,1);
  wait(50, msec);
  Brain.Screen.clearScreen();


  vexcodeInit();
  
} // end pre_auton

// DEFAULT MOTOR SETTINGS
void setUpMotor(motor(M)) {
  M.setMaxTorque(95, percentUnits::pct); // set torque
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
