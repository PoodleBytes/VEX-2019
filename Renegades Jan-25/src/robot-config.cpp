#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor L_Lift = motor(PORT5, ratio36_1, false);
motor R_Lift = motor(PORT6, ratio36_1, true);  //suspect!!

motor L_Claw = motor(PORT7, ratio18_1, false);
motor R_Claw = motor(PORT8, ratio18_1, true);

motor LF_Drive = motor(PORT1, ratio18_1, false);
motor RF_Drive = motor(PORT10, ratio18_1, true);
motor LR_Drive = motor(PORT9, ratio18_1, false);
motor RR_Drive = motor(PORT3, ratio18_1, true);

motor_group L_Drive(LF_Drive, LR_Drive);
motor_group R_Drive(RF_Drive, RR_Drive);
motor_group Claw(L_Claw, R_Claw);
motor_group Lift(L_Lift, R_Lift);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}