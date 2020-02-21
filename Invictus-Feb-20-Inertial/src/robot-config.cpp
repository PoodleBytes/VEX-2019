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