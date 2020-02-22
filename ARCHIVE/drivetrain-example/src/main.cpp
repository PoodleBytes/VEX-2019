/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Wed Sep 25 2019                                           */
/*    Description:  Adjusting Speed                                           */
/*                                                                            */
/*    This program demonstrates how to change the speed of a drivetrains      */
/*    drive and turn actions.                                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drivetrain           drivetrain    1, 10, D        
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // Drives robot forward 6 inches at the default 50% velocity
  Drivetrain.driveFor(forward, 6, inches);
  wait(1, seconds);

  // Drives the robot in reverse for 6 inches at 90% velocity
  Drivetrain.setDriveVelocity(90, percent);
  Drivetrain.driveFor(reverse, 6, inches);
  wait(1, seconds);

  // Turns the robot 90 degrees to the right at the default 50% velocity
  Drivetrain.turnFor(right, 90, degrees);
  wait(1, seconds);

  // Turns the robot to the left at 25% velocity
  Drivetrain.setTurnVelocity(25, percent);
  Drivetrain.turnFor(left, 90, degrees);
}
