#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor Claw = motor(PORT6, ratio18_1, false);
motor L_Lift = motor(PORT5, ratio18_1, false);
motor L_Rear = motor(PORT1, ratio18_1, false);
motor R_Rear = motor(PORT2, ratio18_1, false);
motor L_Front = motor(PORT9, ratio18_1, false);
motor R_Front = motor(PORT10, ratio18_1, false);
motor R_Lift = motor(PORT4, ratio18_1, true);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}