/*---------------------------------------------------------------------------*/
//  NOTES.TXT FOR NOTES / USEFUL SETTINGS
/*---------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
// PROTOTYPE FUNCTION
void pre_auton(void);            // SETTINGS FOR MOTORS, SCREEN ETC
void setUpMotor(motor(M), char); // default motor settings
int updateScreen();              // DISPLAY ENCODER TASK
int tLift();                     // arm-relatecd tasks / buttons
void homeClaw(void);             // set claw height and open position (home)
int read_sonar(void);            // read sonar task

// VARIABLES
double adjField = 1.0; // adjust automomouse for different fields
double adjSpeed = 1.0; // adjust drive sensitivity
double adjLift = 0.8;  // adjust arm's sensitivity
double dist_mm;
int deadBand = 10; // range below which joystick is ignored
bool encode =
    1; // set to true (1) will displays encoders on controller, false will not
int dist2Cube = 160; // mm to grab cube

// MOVEMENT / CONTROL
void rDrive(double, double, double, double, bool); // DRIVE BY RELATIVE DISTANCE
void sDrive(double, double);                       // drive by spinning
void mDrive(int);                                  // drive sideways
void rLift(double, double, bool); // ARM BY RELATIVE DISTANCE (deg, speed, wait)
void aLift(double, double, bool); // ARM BY ABSOLUTE DISTANCE
void openClaw(void);              // open claw
void closeClaw(double);           // close claw (speed)
void drive2Target(double);        // drive until (mm) from target

void autonomous(void) {
  // position claw - DO NOT REMOVE
  homeClaw();              // NEED TO CHANGE!!
  rLift(50, 40, 1);        // lift claw so sensor can 'see'
  drive2Target(dist2Cube); // NEW - drive to about dist2Cube from next cube
  closeClaw(50);
  // more code....
  // turn toward wall......
  drive2Target(300); // Driver to 12" (300mm) from wall

} // end autonomous

void usercontrol(void) {
  // ONE TIME COMMANDS RAN B4 USER CONTROL
  // START TASK for LIFT
  vex::task t(tLift); // start task which updates controller screen
  t.setPriority(6);
  vex::task r_d(read_sonar);
  r_d.setPriority(6);

  // START DISPLAY ENCODER TO CONTROLLER TASK
  if (encode) {
    vex::task upScr(updateScreen); // start task
    upScr.setPriority(3);          // set low priority
  }

  while (1) {
    // DRIVE

    if (abs(Controller1.Axis1.value() + Controller1.Axis2.value()) < 120 &&
        abs(Controller1.Axis1.value() - Controller1.Axis2.value()) < 120) {
      adjSpeed =
          0.3; // ADJUST LOW-SPPED SENSITIVITY  0.2 LESS SENSITIVE, 0.5 MORE SO
    } else {
      adjSpeed = 1.0;
    }

    L_Drive.spin(directionType::fwd,(Controller1.Axis1.value() + Controller1.Axis2.value()) * adjSpeed, velocityUnits::pct);
    R_Drive.spin(directionType::fwd,(Controller1.Axis1.value() - Controller1.Axis2.value()) * adjSpeed,  velocityUnits::pct);
    if(abs(Controller1.Axis4.value()) > 25){
      M_Drive.spin(directionType::fwd, Controller1.Axis4.value() * 0.4, velocityUnits::pct);}

    // MICRO-MOVES
    if (Controller1.ButtonX.pressing()) { // move fwd
      sDrive(-15, -15);
    } else if (Controller1.ButtonB.pressing()) { // move back
      sDrive(15, 15);
    } else if (Controller1.ButtonY.pressing()) { // move left
      sDrive(15, -15);
    } else if (Controller1.ButtonA.pressing()) { // move right
      sDrive(-15, 15);
    } else if (Controller1.ButtonLeft.pressing()) { // move right
      mDrive(-5);
    } else if (Controller1.ButtonRight.pressing()) { // move right
      mDrive(5);
    }

    vex::task::sleep(100); // Sleep the task for a short amount of time to
                           // prevent wasted resources.

  } // end while
}

int tLift(void) { // ARM & CLAW TASK
  while (1) {
    if (Controller1.Axis3.value() > deadBand &&
        Lift.position(vex::rotationUnits::deg) <
            470) { // RAISE LIFT W/SOFT LIMIT OF 500 DEG
      Lift.spin(vex::directionType::fwd, Controller1.Axis3.value() * adjLift,
                vex::velocityUnits::pct);
    } else if (Controller1.Axis3.value() < deadBand * -1.0 &&
               Lift.position(vex::rotationUnits::deg) >
                   0) { // SLOWLY-LOWER LIFT W/SOFT LIMITS AT 0 DEG
      Lift.spin(vex::directionType::fwd,
                Controller1.Axis3.value() * (adjLift * 0.2),
                vex::velocityUnits::pct);
    } else if (Controller1.ButtonL1.pressing()) { // claw open
        if (Claw.rotation(deg) < 0){
          Claw.spin(vex::directionType::fwd, 75, vex::velocityUnits::pct);}
    } else if (Controller1.ButtonL2.pressing()) { // claw close
        if(Claw.current(vex::percentUnits::pct) < 50){
          Claw.spin(vex::directionType::rev, 75, vex::velocityUnits::pct);}
    } else if (Controller1.ButtonR1.pressing()) { // claw open
      closeClaw(70);
    } else if (Controller1.ButtonR2.pressing()) { // claw close
      openClaw();
    } else if (Controller1.ButtonUp
                   .pressing()) { // move 'bot as arm lifts to minimize offset
      Lift.spin(vex::directionType::fwd, 30, vex::velocityUnits::pct);
    } else if (Controller1.ButtonDown.pressing()) {
      Lift.spin(vex::directionType::rev, 30, vex::velocityUnits::pct);
    } else {
      Lift.stop(brakeType::hold);
      Claw.stop(brakeType::hold);
    }
    vex::this_thread::sleep_for(100);
  }
  return 0;
} // end tLift

int read_sonar(void) { // read sonar task
  while (1) {
    dist_mm = Dist.distance(vex::distanceUnits::mm);

    if (dist_mm < dist2Cube + 10 &&
        dist_mm > dist2Cube - 10 & Lift.rotation(rotationUnits::deg) >
                                       50.0) { // distance to target within 20mm
      Controller1.rumble(".");                 // short rumble
      wait(1, seconds);
    } else if (dist_mm < dist2Cube - 10 &
                       Lift.rotation(rotationUnits::deg)> 50.0) { // too close
      Controller1.rumble("--");                                    // long rumble
      wait(1, seconds);
    }

    vex::this_thread::sleep_for(100);
  }
  return 0;
} // end read_sonar(void);           //read sonar task

void rDrive(double lDeg, double rDeg, double l, double r,
            bool b) { // drive by relative distance
  rDeg = rDeg * -1;
  L_Drive.rotateFor(lDeg * adjField, vex::rotationUnits::deg, l,
                    vex::velocityUnits::pct, false);
  R_Drive.rotateFor(rDeg * adjField, vex::rotationUnits::deg, r,
                    vex::velocityUnits::pct, b);
} // end rDrive

void sDrive(double lValue, double rValue) { // drive by spin
  lValue = lValue * -1;
  L_Drive.spin(vex::directionType::fwd, lValue, vex::velocityUnits::pct);
  R_Drive.spin(vex::directionType::fwd, rValue, vex::velocityUnits::pct);
} // end sDrive

void drive2Target(double D) { // drive by spin

  // reset motor encoders & set braking
  L_Drive.resetRotation();
  R_Drive.resetRotation();

  // calc #degrees for distanc
  double numDegToTarget = ((dist_mm - D) / 0.887);          // distance * (wheel dia / 360 deg = 0.887 mm/deg)
  double numDegToDrive;     // track # of degrees motors turned 
  int speed;    //set motor speed

  bool b = 1;
  while (b) {

    if (dist_mm > 99999) { b = 0;} // no object detected - quit

    while (dist_mm > (D + 100)) { // distance from object > target distance + buffer
      
      numDegToDrive = numDegToTarget - L_Drive.rotation(deg); //calculate remaining deg to turn

      if (numDegToDrive / numDegToTarget > 0.4) {  //more than 40% away from target
        speed = 60;                                 //go 60% speed
      } else {
        speed = (numDegToDrive / numDegToTarget * 100) + 20;    //less than 40% from target start slowing
      }

      L_Drive.spin(vex::directionType::fwd, speed, vex::velocityUnits::pct);
      R_Drive.spin(vex::directionType::rev, speed, vex::velocityUnits::pct);

    } // end while

    L_Drive.stop(brake);
    R_Drive.stop(brake);

    b = 0;    //exit function
  }
} // end drive2Target

void mDrive(int s) { // middle drive by spin

  M_Drive.spin(vex::directionType::fwd, s, vex::velocityUnits::pct);

} // end sDrive

void aLift(double deg, double s, bool b) { // position lift by absolute position
  Lift.rotateTo(deg, vex::rotationUnits::deg, s, vex::velocityUnits::pct,
                false); // This command must be non blocking.

  if (b) { // b = tue means wait for motors stop spinning or timeout
    while (Lift.isSpinning()) {
    }
  }
} // end aArm

void rLift(double deg, double s, bool b) { // position lift by relative position
  Lift.rotateFor(deg, vex::rotationUnits::deg, s, vex::velocityUnits::pct,
                 false); // This command must be non blocking.

  if (b) { // b = tue means wait for motors stop spinning or timeout
    while (Lift.isSpinning()) {
    }
  }
} // end eArm

void closeClaw(double s) {
  do {
    Claw.spin(vex::directionType::fwd, s, vex::velocityUnits::pct);
  } while (Claw.current(vex::percentUnits::pct) < 50);
  Claw.stop(hold);
} // end rClaw

void openClaw(void) {
  Claw.setVelocity(75, pct);
  Claw.spinTo(0, deg); // spin to home position
  Claw.stop(brake);
} // end rClaw

void homeClaw(void) {
  Claw.rotateFor(40, vex::rotationUnits::deg, 50, vex::velocityUnits::pct,
                 true); // slightly close claw
  Lift.rotateFor(42, vex::rotationUnits::deg, 25, vex::velocityUnits::pct,
                 true); // lift claw to mid-cube
  Lift.resetRotation(); // make position home
  Claw.resetRotation(); // make position home
} // end  homeClaw(void);

// SYSTEM SET-UP
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // set motor defayults
  setUpMotor(L_Drive, 'C');
  setUpMotor(R_Drive, 'C');
  setUpMotor(M_Drive, 'C');
  setUpMotor(UL_Lift, 'H');
  setUpMotor(LL_Lift, 'H');
  setUpMotor(UR_Lift, 'H');
  setUpMotor(LR_Lift, 'H');
  setUpMotor(Claw, 'H');

  Lift.setMaxTorque(90, percentUnits::pct); // set torque
  Lift.setStopping(brakeType::hold);        // braking
  Lift.setTimeout(3, timeUnits::sec);       // set motor timeout
  Lift.resetRotation();                     // reset encoders

  // clear controller display
  Controller1.Screen.clearScreen();
} // end pre_auton

// DEFAULT MOTOR SETTINGS
void setUpMotor(motor(M), char B) {
  M.setMaxTorque(95, percentUnits::pct); // set torque
  M.setTimeout(3, timeUnits::sec);       // set motor timeout
  M.resetRotation();                     // reset encoders
  if (B == 'C') {
    M.setStopping(brakeType::coast); // coast
  } else if (B == 'H') {
    M.setStopping(brakeType::hold); // braking
  } else if (B == 'B') {
    M.setStopping(brakeType::brake); // braking
  } else {
    M.setStopping(brakeType::coast); // braking
  }
}

int updateScreen() {
  while (1) {
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print(
        "Left: %4.1f", L_Drive.rotation(rotationUnits::deg)); // LEFT MOTOR
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print(
        "Right: %4.1f", R_Drive.rotation(rotationUnits::deg)); // RIGHT MOTOR
    Controller1.Screen.setCursor(3, 1);
    Controller1.Screen.print(
        "L: %4.2f S: %4.2f", Lift.rotation(rotationUnits::deg),
        Claw.rotation(rotationUnits::deg)); // LIFT AND STAND

    /*     Controller1.Screen.print(
            "dist_mm: %4.1f", dist_mm); // LEFT MOTOR
        Controller1.Screen.setCursor(2, 1);
        Controller1.Screen.print(
            "Found: %4.1f"); // RIGHT MOTOR
        Controller1.Screen.setCursor(3, 1);
        Controller1.Screen.print(
            "Value: %4.2f "); // LIFT AND STAND */

    // RESET ENCODERS
    /*
    if (Controller1.ButtonLeft.pressing()) { // DRIVE ENCODERS
      L_Drive.resetRotation();
      R_Drive.resetRotation();
    }
    if (Controller1.ButtonRight.pressing()) { // ARM AND GRABBER ENCODERS
      L_Lift.resetRotation();
      Claw.resetRotation();
    }
 */
    // don't hog the cpu :)
    vex::this_thread::sleep_for(250);

  } // end while
  return 0;
} // END UPDATE SCREEN

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
