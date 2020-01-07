/*---------------------------------------------------------------------------*/
//  NOTES.TXT FOR NOTES / USEFUL SETTINGS
/*---------------------------------------------------------------------------*/
#include "vex.h"


// A global instance of competition
competition Competition;

void pre_auton(void);            // SETTINGS FOR MOTORS, SCREEN ETC

//VARIABLES
double adjSpeed = 1.0; // adjust drive sensitivity
double adjLift = 1.0;  // adjust arm's sensitivity
int deadBand = 10;     // range below which joystick is ignored
bool encode = 1; // set to true (1) will displays encoders on controller, false will not
double adjField = 1.0;   //tweek autonomous is set in functions.cpp

//FUNCTIONS / TASKS
// void setUpMotor(motor(M), char);
// int updateScreen();
int tLift();

// MOVEMENT / CONTROL
//void rDrive(double, double, double, double, bool); // DRIVE BY RELATIVE DISTANCE
// void rLift(double, double, bool);                  // ARM BY RELATIVE DISTANCE
// void aLift(double, double, bool);                  // ARM BY ABSOLUTE DISTANCE
// void sDrive(double, double);                       //drive by spinning
// void openClaw(void);                              // open claw
// void closeClaw(double);                           // close claw (speed)
// void homeClaw();                                  //initialize claw/lift 

void autonomous(void) {
  //position claw - DO NOT REMOVE
  homeClaw();  


  //auto - 4point blue - temp
  rDrive(140, 140, 40, 40, 1);    //drive to near cube
  closeClaw(70);                  
  rLift(220, 60, 1);            //lift cube
  rDrive(670, 670, 40, 40, 1);  //drive to stack
  openClaw();
  rLift(-215, 20, 1);           //lower lift
  closeClaw(75);
  rLift(40, 80, 1);             //lift stack
  rDrive(600, -600, 40, 40, 1); //turn toward corner
 
  
/*   // USE AUTONOMOUS.TXT TO COPY/PASTE AUTONOMOUS VELOW
  rDrive(1200, 1200, 75, 75, 1);
  rDrive(-160, -160, 50, 50, 1);
  rDrive(-420, 420, 40, 40, 1); //TURN LEFT 90DEG
  rDrive(195, 195, 30, 30, 1);
  closeClaw(50);
  rLift(30, 40, 1);
   rDrive(-500, 500, 50, 50, 1); */


} // end autonomous

void usercontrol(void) {
  // ONE TIME COMMANDS RAN B4 USER CONTROL
  
  // START TASK for LIFT
  vex::task t(tLift); // start task which updates controller screen

  // START DISPLAY ENCODER TO CONTROLLER TASK
  if (encode) {
    vex::task upScr(updateScreen); // start task
    upScr.setPriority(4);          // set low priority
  }

  while (1) {
    // DRIVE

    if (abs(Controller1.Axis1.value() + Controller1.Axis2.value()) < 120 &&
        abs(Controller1.Axis1.value() - Controller1.Axis2.value()) < 120) {
      adjSpeed =
          0.3; // ADJUST LOW-SPPED SENSITIVITY  0.2 LESS SENSITIVE, 0.5 MORE SO
    } else {
      adjSpeed = 1.0;
    }

    L_Drive.spin(directionType::fwd,(Controller1.Axis1.value() + Controller1.Axis2.value()) * adjSpeed,velocityUnits::pct);
    R_Drive.spin(directionType::fwd,(Controller1.Axis1.value() - Controller1.Axis2.value()) * adjSpeed,velocityUnits::pct);
  
    //MICRO-MOVES
    if (Controller1.ButtonX.pressing()) { // move fwd
      sDrive(15, 15);
    } else if (Controller1.ButtonB.pressing()) { // move back
      sDrive(-15, -15);
    } else if (Controller1.ButtonY.pressing()) { // move left
      sDrive(-15, 15);
    } else if (Controller1.ButtonA.pressing()) { // move right
      sDrive(15, -15);
    }

    vex::task::sleep(100); // Sleep the task for a short amount of time to prevent wasted resources.

  } // end while
}

int tLift(void) { // ARM & CLAW TASK
  while (1) {
    if (Controller1.Axis3.value() > deadBand && Lift.position(vex::rotationUnits::deg) < 500) { // RAISE LIFT W/SOFT LIMIT OF 500 DEG
      Lift.spin(vex::directionType::fwd, Controller1.Axis3.value() * adjLift,vex::velocityUnits::pct);
    } else if (Controller1.Axis3.value() < deadBand * -1.0 && Lift.position(vex::rotationUnits::deg) > 0) { // SLOWLY-LOWER LIFT W/SOFT LIMITS AT 0 DEG
      Lift.spin(vex::directionType::fwd, Controller1.Axis3.value() * (adjLift * 0.15),vex::velocityUnits::pct);
    } else if (Controller1.ButtonL1.pressing()) { // claw open
      Claw.spin(vex::directionType::fwd, 75, vex::velocityUnits::pct);
    } else if (Controller1.ButtonL2.pressing()) { // claw close
      Claw.spin(vex::directionType::rev, 75, vex::velocityUnits::pct);
    } else if (Controller1.ButtonR1.pressing()) { // claw open
      closeClaw(70);
    } else if (Controller1.ButtonR2.pressing()) { // claw close
      openClaw();
    } else if (Controller1.ButtonUp.pressing()) { // move 'bot as arm lifts to minimize offset
      Lift.spin(vex::directionType::fwd, 30, vex::velocityUnits::pct);
    } else if (Controller1.ButtonDown.pressing()) {
      Lift.spin(vex::directionType::rev, 30, vex::velocityUnits::pct);
    } else {
      Lift.stop(brakeType::hold);
      Claw.stop(brakeType::hold);
    }
    vex::task::sleep(5);
  }
  return 0;
} // end tLift

/***********************************************/

// SYSTEM SET-UP
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // set motor defayults
  setUpMotor(L_Drive, 'C');
  setUpMotor(R_Drive, 'C');
  setUpMotor(L_Lift, 'H');
  setUpMotor(R_Lift, 'H');
  setUpMotor(Claw, 'H');

  Lift.setMaxTorque(90, percentUnits::pct); // set torque
  Lift.setStopping(brakeType::hold);        // braking
  Lift.setTimeout(3, timeUnits::sec);       // set motor timeout
  Lift.resetRotation();                     // reset encoders

  // clear controller display
  Controller1.Screen.clearScreen();
} // end pre_auton

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
