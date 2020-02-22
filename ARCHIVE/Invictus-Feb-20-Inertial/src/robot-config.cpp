#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);

motor R_Lift = motor(PORT6, ratio36_1, false);
motor L_Lift = motor(PORT7, ratio36_1, true);
motor L_Intake = motor(PORT9, ratio36_1, false);
motor R_Intake = motor(PORT10, ratio36_1, true);
motor L_Stand = motor(PORT8, ratio36_1, false);
motor R_Stand = motor(PORT11, ratio36_1, true);

motor L_Drive = motor(PORT2, ratio18_1, false);
motor R_Drive = motor(PORT3, ratio18_1, true);
inertial Gyro = inertial(PORT20);
smartdrive DriveTrain = smartdrive(L_Drive, R_Drive, Gyro,319.19, 320, 165, mm, 1); //circumference, track, wheelbase, units

motor_group Intake(L_Intake, R_Intake);
motor_group Lift(L_Lift, R_Lift);
motor_group Stand(L_Stand, R_Stand);

// VARIABLES
double adjSpeed = 1.0; // adjust drive sensitivity
double adjField = 1.0; // adjust automouse drive() degrees match conditions

// VEXcode generated functions


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
}