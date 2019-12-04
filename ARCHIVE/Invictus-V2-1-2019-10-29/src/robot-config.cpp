#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor R_Drive = motor(PORT3, ratio18_1, true);
motor L_Drive = motor(PORT2, ratio18_1,false);
motor R_Lift = motor(PORT6, ratio36_1, false);
motor L_Lift = motor(PORT7, ratio36_1, true);
motor L_Intake = motor(PORT4, ratio18_1, false);
motor R_Intake = motor(PORT5, ratio18_1, true);
motor Stand = motor(PORT8, ratio36_1, true);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}