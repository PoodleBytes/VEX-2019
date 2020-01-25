
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Claw                 motor         6               
// L_Lift               motor         5               
// L_Rear               motor         1               
// R_Rear               motor         2               
// L_Front              motor         9               
// R_Front              motor         10              
// R_Lift               motor         4               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  L_Claw.setBrake(hold);
  R_Claw.setBrake(hold);
  L_Lift.setBrake(hold);
  R_Lift.setBrake(hold);
    
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {

L_Drive.resetPosition();
R_Drive.resetPosition();
//drive backward 23"
L_Drive.rotateTo(-720, vex::rotationUnits::deg, 80, vex::velocityUnits::pct, false);
R_Drive.rotateTo(-720, vex::rotationUnits::deg, 80, vex::velocityUnits::pct, true);

L_Drive.stop(hold);
R_Drive.stop(hold);

L_Drive.rotateTo(0, vex::rotationUnits::deg, 80, vex::velocityUnits::pct, false);
R_Drive.rotateTo(0, vex::rotationUnits::deg, 80, vex::velocityUnits::pct, true);

L_Drive.stop(hold);
R_Drive.stop(hold);
}


void usercontrol(void) {
  // User control code here, inside the loop

  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.
      L_Drive.spin(directionType::fwd, (Controller1.Axis3.value() + Controller1.Axis4.value()), velocityUnits::pct);
      R_Drive.spin(directionType::fwd, (Controller1.Axis3.value() - Controller1.Axis4.value()), velocityUnits::pct);

      Lift.spin(directionType::fwd, (Controller1.Axis2.value()), velocityUnits::pct);

      if(Controller1.ButtonR1.pressing()){    
          Claw.spin(vex::directionType::fwd, 25, vex::velocityUnits::pct);
      }else if(Controller1.ButtonR2.pressing()){
          Claw.spin(vex::directionType::rev, 25
          , vex::velocityUnits::pct); 
      }else{
          Claw.stop(brakeType::hold);
      }
    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
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
