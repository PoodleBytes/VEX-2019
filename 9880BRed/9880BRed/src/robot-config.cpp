#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor liftR = motor(PORT5, ratio18_1, false);
motor liftL = motor(PORT9, ratio18_1, true);
motor spinL = motor(PORT6, ratio18_1, true);
motor spinR = motor(PORT4, ratio18_1, false);
motor jointTurn = motor(PORT7, ratio36_1, true);
motor driveLF = motor(PORT20, ratio18_1, false);
motor driveRF = motor(PORT10, ratio18_1, true);
// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}