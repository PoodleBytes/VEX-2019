#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors - PORT 8 BAD
controller Controller1 = controller(primary);
motor L_Drive = motor(PORT19, ratio18_1, false);
motor R_Drive = motor(PORT9, ratio18_1, false);

motor LL_Lift = motor(PORT16, ratio36_1, false);
motor LR_Lift = motor(PORT4, ratio36_1, true);
motor UL_Lift = motor(PORT14, ratio36_1, true);
motor UR_Lift = motor(PORT2, ratio36_1, false);

motor Claw = motor(PORT11, ratio36_1, true);
sonar Dist = sonar(Brain.ThreeWirePort.C);    //C is Sensor Output, D is Input

motor_group Lift(UL_Lift, LL_Lift,UR_Lift, LR_Lift); // makes 2 lift motors act as one

drivetrain Go = drivetrain(L_Drive, L_Drive, 319.19, 390.525, 279.4, mm, 1);//track, wbase





// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}