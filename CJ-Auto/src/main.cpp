/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\VEX                                              */
/*    Created:      Fri Jan 10 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
}

// MOVEMENT / CONTROL
void rDrive(double, double, double, double, bool); // DRIVE BY RELATIVE DISTANCE

void autonomous(void) {
rDrive(-500,-500,20,20,1);
rDrive(500,500,20,20,1);
}