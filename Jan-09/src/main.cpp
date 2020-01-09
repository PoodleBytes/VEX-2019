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
double dist_mm;        //sonar distance in mm
int deadBand = 10; // range below which joystick is ignored
bool display = 1; // set to true (1) will displays encoders on controller, false will not
int dist2Cube = 160; // mm to grab cube

// MOVEMENT / CONTROL
void rDrive(double, double, double, double, bool); // DRIVE BY RELATIVE DISTANCE
void sDrive(double, double);                       // drive by spinning (speed L, speed r)
void mDrive(int);                                  // drive sideways (speed)
void rLift(double, double, bool); // LIFT BY RELATIVE DISTANCE (deg, speed, wait)
void aLift(double, double, bool); // LIFT BY ABSOLUTE DISTANCE (deg, speed, wait)
void openClaw(int);              // open claw (deg)
void closeClaw(double);           // close claw (speed)
void drive2Target(double);        // drive to target (distance in mm)
void grabCube( int);    //GRAB CUBE - (# OF CUBES)

void autonomous(void) {
  // position claw - DO NOT REMOVE
  homeClaw();              // NEED TO VERIFY!!
  closeClaw(50);          //grab preload cube
  grabCube(1);         //EXP - GRAB NEXT - SINGLE  CUBE
  // more code....
  // turn toward wall......
  drive2Target(300); // Driver to 12" (300mm) from wall

} // end autonomous

void usercontrol(void) {
  // ONE TIME COMMANDS RAN B4 USER CONTROL
  // START TASK for LIFT
  vex::task t(tLift); // start task which updates controller screen
  t.setPriority(6);

  //START TASK TO READ SONAR (DISTANCE)
  vex::task dst(read_sonar);
  dst.setPriority(6);

  // START DISPLAY ENCODER TO CONTROLLER TASK
  if (display) {
    vex::task dsp(updateScreen); // start task
    dsp.setPriority(3);          // set low priority
  }

  while (1) {
    // DRIVE

    if (abs(Controller1.Axis1.value() + Controller1.Axis2.value()) < 120 && abs(Controller1.Axis1.value() - Controller1.Axis2.value()) < 110) {
      adjSpeed = 0.3; // ADJUST LOW-SPEED SENSITIVITY  0.2 LESS SENSITIVE, 0.5 MORE SO
    } else {
      adjSpeed = 1.0;
    }
    
    //LEFT / RIGHT DRIVE
    L_Drive.spin(directionType::fwd,(Controller1.Axis1.value() + Controller1.Axis2.value()) * adjSpeed, velocityUnits::pct);
    R_Drive.spin(directionType::fwd,(Controller1.Axis1.value() - Controller1.Axis2.value()) * adjSpeed,  velocityUnits::pct);

    //MIDDLE DRIVE
    if(abs(Controller1.Axis4.value()) > 25){    //'deadband for raising claw 
      M_Drive.spin(directionType::fwd, Controller1.Axis4.value() * 0.4, velocityUnits::pct);}

    // MICRO-MOVES
    if (Controller1.ButtonX.pressing()) { // move fwd
      sDrive(15, 15);
    } else if (Controller1.ButtonB.pressing()) { // move back
      sDrive(-15, -15);
    } else if (Controller1.ButtonY.pressing()) { // turn left
      sDrive(-15, 15);
    } else if (Controller1.ButtonA.pressing()) { // turn right
      sDrive(15, -15);
    } else if (Controller1.ButtonLeft.pressing()) { // move right
      mDrive(-5);
    } else if (Controller1.ButtonRight.pressing()) { // move right
      mDrive(5);
    }

    vex::task::sleep(100); // Sleep the task for a short amount of time to prevent wasted resources.

  } // end while
}

int tLift(void) { // ARM & CLAW TASK
  while (1) {
    if (Controller1.Axis3.value() > deadBand && Lift.position(vex::rotationUnits::deg) < 470) { // RAISE LIFT W/SOFT LIMIT OF 500 DEG
      Lift.spin(vex::directionType::fwd, Controller1.Axis3.value() * adjLift, vex::velocityUnits::pct);
    } else if (Controller1.Axis3.value() < deadBand * -1.0 && Lift.position(vex::rotationUnits::deg) > 0) { // SLOWLY-LOWER LIFT W/SOFT LIMITS AT 0 DEG
      Lift.spin(vex::directionType::fwd, Controller1.Axis3.value() * (adjLift * 0.2), vex::velocityUnits::pct);
    } else if (Controller1.ButtonL1.pressing() && Claw.rotation(deg) < 0) { // claw open
          Claw.spin(vex::directionType::fwd, 75, vex::velocityUnits::pct);
    } else if (Controller1.ButtonL2.pressing() && Claw.current(vex::percentUnits::pct) < 50) { // claw close
          Claw.spin(vex::directionType::rev, 75, vex::velocityUnits::pct);
    } else if (Controller1.ButtonR1.pressing()) { // claw close
      closeClaw(70);
    } else if (Controller1.ButtonR2.pressing()) { // claw open
      openClaw(0);
    } else if (Controller1.ButtonUp.pressing()) { // move 'bot as arm lifts to minimize offset
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

    if (dist_mm < dist2Cube + 10 && dist_mm > dist2Cube - 10 & Lift.rotation(rotationUnits::deg) > 50.0) { // distance to target within 20mm
      Controller1.rumble(".");                 // short rumble
      wait(1, seconds);
    } else if (dist_mm < dist2Cube - 10 && Lift.rotation(rotationUnits::deg)> 50.0) { // too close
      Controller1.rumble("--");                                    // long rumble
      wait(1, seconds);
    }

    vex::this_thread::sleep_for(100);
  }
  return 0;
} // end read_sonar(void);           //read sonar task

void rDrive(double lDeg, double rDeg, double l, double r, bool b) { // drive by relative distance
  rDeg = rDeg * -1;
  L_Drive.rotateFor(lDeg * adjField, vex::rotationUnits::deg, l, vex::velocityUnits::pct, false);
  R_Drive.rotateFor(rDeg * adjField, vex::rotationUnits::deg, r, vex::velocityUnits::pct, b);
} // end rDrive

void sDrive(double lSpeed, double rSpeed) { // drive by spin
  rSpeed = rSpeed * -1;
  L_Drive.spin(vex::directionType::fwd, lSpeed, vex::velocityUnits::pct);
  R_Drive.spin(vex::directionType::fwd, rSpeed, vex::velocityUnits::pct);
} // end sDrive

void drive2Target(double D) { // drive by spin

  double numDegToTarget = ((dist_mm - D) / 0.887);          // distance * (wheel dia / 360 deg = 0.887 mm/deg)
  double numDegToDrive;     // track # of degrees motors turned 
  int speed;    //set motor speed
  bool b = 1;
 
  // reset motor encoders & set braking
  L_Drive.resetRotation();
  R_Drive.resetRotation();
  
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

void mDrive(int speed) { // middle drive by spin

  M_Drive.spin(vex::directionType::fwd, speed, vex::velocityUnits::pct);

} // end sDrive

void aLift(double deg, double speed, bool b) { // position lift by absolute position
  Lift.rotateTo(deg, vex::rotationUnits::deg, speed, vex::velocityUnits::pct, false); // This command must be non blocking.

  if (b) { // b = tue means wait for motors stop spinning or timeout
    while (Lift.isSpinning()) {
    }
  }
} // end aArm

void rLift(double deg, double speed, bool b) { // position lift by relative position
  Lift.rotateFor(deg, vex::rotationUnits::deg, speed, vex::velocityUnits::pct, false); // This command must be non blocking.

  if (b) { // b = tue means wait for motors stop spinning or timeout
    while (Lift.isSpinning()) {
    }
  }
} // end eArm

void closeClaw(double speed) {
  do {
    Claw.spin(vex::directionType::fwd, speed, vex::velocityUnits::pct);
  } while (Claw.current(vex::percentUnits::pct) < 50);
  Claw.stop(hold);
} // end rClaw

void openClaw(int d) {
  Claw.setVelocity(75, pct);
  Claw.spinTo(d, deg); // spin to home position
  Claw.stop(brake);
} // end rClaw

void homeClaw(void) {
  Claw.rotateFor(40, vex::rotationUnits::deg, 75, vex::velocityUnits::pct,true); // slightly close claw
  Lift.rotateFor(25, vex::rotationUnits::deg, 40, vex::velocityUnits::pct,true); // lift claw to mid-cube
  Lift.resetRotation(); // make position home
  Claw.resetRotation(); // make position home
} // end  homeClaw(void);

void grabCube(int n){
  int liftTo = 0;

  switch(n)
  {
    case 1:
      liftTo = 50;
    case 2:
      liftTo = 100;
    case 3:
      liftTo = 150;
    case 4:
      liftTo = 200;
    case 5:
      liftTo = 250;
    default:
      break;
  } 

  aLift(liftTo, 40, 1);        // lift claw so sensor can 'see'
  drive2Target(dist2Cube); // NEW - drive to about dist2Cube from next cube
  openClaw(Claw.rotation(deg)-20);  //open claw current position - 20 deg
  aLift(0, 20, 1);  //lower lift to start (home) position
  closeClaw(50);  //close claw 50% speed
  aLift(50, 40, 1);        // lift claw so sensor can 'see'
}

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
    Controller1.Screen.print("L: %4i R: %4i", L_Drive.rotation(rotationUnits::deg), R_Drive.rotation(rotationUnits::deg)); // LEFT, RIGHT MOTOR
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print("L: %4i C: %4i", Lift.rotation(rotationUnits::deg), Claw.rotation(rotationUnits::deg)); // LIFT CLAW
    Controller1.Screen.setCursor(3, 1);
    Controller1.Screen.print("Distance: %5i", dist_mm);


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
