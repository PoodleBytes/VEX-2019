#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor L_Drive = motor(PORT16, ratio18_1, false);
motor R_Drive = motor(PORT15, ratio18_1, false);
motor L_Lift = motor(PORT9, ratio36_1, true);
motor R_Lift = motor(PORT2, ratio36_1, false);
motor Claw = motor(PORT5, ratio18_1, true);
motor LeftDriveSmart = motor(PORT20, ratio18_1, false);
motor RightDriveSmart = motor(PORT21, ratio18_1, true);
drivetrain Dt = drivetrain(L_Drive, R_Drive, 319.19, 381, 130, mm, 1);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}