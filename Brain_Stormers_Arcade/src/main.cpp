/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Clawbot Competition Template                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drivetrain           drivetrain    1, 10           
// Controller1          controller                    
// ClawMotor            motor         3               
// ArmMotor             motor         8               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*  You may want to perform some actions before the competition starts.      */
/*---------------------------------------------------------------------------*/
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  Brain.Screen.print("Hi! We are Brain Stormers!!!!");  //print message on Brain
  wait(1,seconds);
}

/*---------------------------------------------------------------------------*/
/*                              Autonomous                                   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // drive forward and back 25"
  Drivetrain.setDriveVelocity(50, percent); // set drive speed 50%
  wait(3, seconds);                         // wait 3 seconds
  Drivetrain.driveFor(25, inches);          // go forward 25"
  wait(3, seconds);                         // wait 3 seconds
  Drivetrain.driveFor(-25, inches);         // back-up 25"
  wait(1, seconds);
  Drivetrain.stop();
} // end of autonomous

/*---------------------------------------------------------------------------*/
/*                              User Control Task                            */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {    
  // User control code here, inside the loop
  while (1) {

    if(abs(Controller1.Axis3.value())>5){  
        ArmMotor.spin(vex::directionType::fwd, Controller1.Axis3.value()/2, vex::velocityUnits::pct); //lift arm at half of left joystick's value
    }else{
      ArmMotor.stop(hold);  //hold arm's current position
    }

    // claw control
    if (Controller1.ButtonR1.pressing()) {
      ClawMotor.spin(vex::directionType::fwd, 60, vex::velocityUnits::pct);   //close claw 60% speed
    } else if (Controller1.ButtonR2.pressing()) {
      ClawMotor.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);  //open claw full-speed
    } else {
      ClawMotor.stop(hold); // stop motor but hold position
    }

    //arm control
    if (Controller1.ButtonL1.pressing()) {
      ArmMotor.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct);    //raise arm half speed
    } else if (Controller1.ButtonL2.pressing()) {
      ArmMotor.spin(vex::directionType::rev, 50, vex::velocityUnits::pct);  //lower arm half speed
    } else {
      ArmMotor.stop(hold); // stop motor but hold position
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  } //end infinite loop
} //end user control

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
