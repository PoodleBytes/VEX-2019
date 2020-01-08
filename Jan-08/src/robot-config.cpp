#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors - PORT 8 BAD
controller Controller1 = controller(primary);
motor L_Drive = motor(PORT10, ratio18_1, false);
motor R_Drive = motor(PORT1, ratio18_1, false);
motor M_Drive = motor(PORT13, ratio18_1, false);
motor LL_Lift = motor(PORT6, ratio36_1, false);
motor LR_Lift = motor(PORT11, ratio36_1, true);
motor UL_Lift = motor(PORT17, ratio36_1, true);
motor UR_Lift = motor(PORT16, ratio36_1, false);
motor Claw = motor(PORT20, ratio36_1, true);
sonar Dist = sonar(Brain.ThreeWirePort.C);    //C is Sensor Output, D is Input

motor_group Lift(UL_Lift, LL_Lift,UR_Lift, LR_Lift); // makes 2 lift motors act as one


// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}