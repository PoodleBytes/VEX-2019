
#include "math.h"
#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// PROTOTYPE FUNCTION
void pre_auton(void);           // SETTINGS FOR MOTORS, SCREEN ETC
void setUpMotor(motor(M), int); // default motor settings
void dspMotor();                // TEST MOTOR
void writeToLog();

std::ofstream ofs;

void autonomous(void) {} // end autonomous

void usercontrol(void) {
  // ONE TIME COMMANDS RAN B4 USER CONTROL
  while (1) {
    // DRIVE

    vex::task::sleep(100); // Sleep the task for a short amount of time to
                           // prevent wasted resources.

  } // end while
}

// SYSTEM SET-UP
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // set motor defayults
  setUpMotor(L_Drive, 10);
  setUpMotor(R_Drive, 10);
  setUpMotor(L_Lift, 10);
  setUpMotor(R_Lift, 10);
  setUpMotor(Claw, 10);

} // end pre_auton

// monitor motor current / temp
int monitorMotor() {
  double tmp_L_Drive = L_Drive.temperature();
  double amp_L_Drive = L_Drive.current(amp) / 1000;
  double tmp_R_Drive = R_Drive.temperature();
  double amp_R_Drive = R_Drive.current(amp) / 1000;
  double tmp_L_Lift = L_Lift.temperature();
  double amp_L_Lift = R_Lift.current(amp) / 1000;
  double tmp_R_Lift = R_Lift.temperature();
  double amp_R_Lift = R_Lift.current(amp) / 1000;
  double tmp_Claw = Claw.temperature();
  double amp_Claw = Claw.current(amp) / 1000;

  while (1) {
    wait(10, seconds);
    if (L_Drive.temperature() > tmp_L_Drive)
      tmp_L_Drive = L_Drive.temperature();
    if (L_Drive.current(amp) / 1000 > amp_L_Drive)
      amp_L_Drive = L_Drive.current(amp) / 1000;
    if (R_Drive.temperature() > tmp_R_Drive)
      tmp_R_Drive = R_Drive.temperature();
    if (R_Drive.current(amp) / 1000 > amp_R_Drive)
      amp_R_Drive = R_Drive.current(amp) / 1000;
    if (L_Lift.temperature() > tmp_L_Lift)
      tmp_L_Lift = L_Lift.temperature();
    if (L_Lift.current(amp) / 1000 > amp_L_Lift)
      amp_L_Lift = L_Lift.current(amp) / 1000;
    if (R_Lift.temperature() > tmp_R_Lift)
      tmp_R_Lift = R_Lift.temperature();
    if (R_Lift.current(amp) / 1000 > amp_R_Lift)
      amp_R_Lift = R_Lift.current(amp) / 1000;
    if (Claw.temperature() > tmp_Claw)
      tmp_Claw = Claw.temperature();
    if (Claw.current(amp) / 1000 > amp_Claw)
      amp_Claw = Claw.current(amp) / 1000;
  } // end while

   return 1;
} // end monitorMotor

void writeToLog(){
    if( Brain.SDcard.isInserted() ) {
      // create a file with long filename
      int f = 1;
      std::string fn = intToChar(f);
      ofs.open(fn, std::ofstream::out);
      ofs << "lorem ipsum\r\n";
      ofs << "this was a test of a file with long file name\r\n";
      ofs.close();

      Brain.Screen.printAt(10, 40, "done");
    }
    else {
      Brain.Screen.printAt(10, 40, "No SD Card");        
    }
}//end writeToLog

// DEFAULT MOTOR SETTINGS
void setUpMotor(motor(M), int D) {
  M.value();
  M.setMaxTorque(90, percentUnits::pct); // set torque
  M.setTimeout(3, timeUnits::sec);       // set motor timeout
  M.resetRotation();                     // reset encoders
  M.setStopping(hold);                   // hold
  M.spinTo(D, deg, 15, velocityUnits::pct);
  dspMotor();
  wait(2, seconds);
}

void dspMotor() {
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("Drive L: %3d R: %3d", L_Drive.current(amp) / 1000,
                           R_Drive.current(amp) / 1000); // L&R DRIVE
  Controller1.Screen.setCursor(2, 1);
  Controller1.Screen.print("Lift  L: %3d R: %3d", L_Lift.current(amp) / 1000,
                           R_Lift.current(amp) / 1000); // L&R LIFT
  Controller1.Screen.setCursor(2, 1);
  Controller1.Screen.print("Claw: %3d",
                           Claw.current(amp) / 1000); // CLAW

} // END TEST MOTOR

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
