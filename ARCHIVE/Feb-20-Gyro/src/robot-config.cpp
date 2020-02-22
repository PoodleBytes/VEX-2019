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

motor Claw = motor(PORT11, ratio36_1, true);

sonar Dist = sonar(Brain.ThreeWirePort.C);    //C is Sensor Output, D is Input

motor_group Lift(UL_Lift, LL_Lift,UR_Lift, LR_Lift); // makes 2 lift motors act as one

inertial Gyro = inertial(PORT1);
smartdrive Drive = smartdrive(L_Drive, R_Drive, Gyro,319.19, 390.525, 279.4, mm, 1);  //track, wbase




// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  Brain.Screen.print("Device initialization...");
  Brain.Screen.setCursor(2, 1);
  // calibrate the drivetrain gyro
  wait(200, msec);
  Gyro.calibrate();
  Brain.Screen.print("Calibrating Gyro for Drivetrain");
  // wait for the gyro calibration process to finish
  while (Gyro.isCalibrating()) {
    wait(25, msec);
  }
  // reset the screen now that the calibration is complete
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1,1);
  wait(50, msec);
  Brain.Screen.clearScreen();
}