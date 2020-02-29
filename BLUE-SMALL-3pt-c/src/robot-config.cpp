#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors - PORT 8 BAD
controller Controller1 = controller(primary);

motor L_Drive = motor(PORT19, ratio18_1, false);
motor R_Drive = motor(PORT9, ratio18_1, true);

motor LL_Lift = motor(PORT16, ratio36_1, false);
motor LR_Lift = motor(PORT4, ratio36_1, true);
motor UL_Lift = motor(PORT14, ratio36_1, true);
motor UR_Lift = motor(PORT2, ratio36_1, false);

motor L_Claw = motor(PORT12, ratio36_1, true);
motor U_Claw = motor(PORT17, ratio18_1, true);

sonar Dist = sonar(Brain.ThreeWirePort.C);    //C is Sensor Output, D is Input

motor_group Lift(UL_Lift, LL_Lift,UR_Lift, LR_Lift); // makes 2 lift motors act as one
motor_group Claw(L_Claw, U_Claw);

inertial Gyro = inertial(PORT1);
smartdrive Drive = smartdrive(L_Drive, R_Drive, Gyro,319.19, 390.525, 279.4, mm, 1);  //track, wbase

// VARIABLES
double adjField = 1.0;
double adjSpeed = 1.0; // adjust drive sensitivity
double adjLift = 0.8;  // adjust arm's sensitivity
double dist_mm;        //sonar distance in mm
int deadBand = 10; // range below which joystick is ignored
bool display = 1; // set to true (1) will displays encoders on controller, false will not
bool buzz_on = 0;
int dist2Cube = 175; // mm to grab cube

// VEXcode generated function

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {


}