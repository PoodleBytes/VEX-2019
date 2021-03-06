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
bool buzz_on = 1;
int dist2Cube = 175; // mm to grab cube
double degr;
double head;
double ang;

// MOVEMENT / CONTROL
void rDrive(double l_deg, double r_deg, double l_speed, double r_speed, bool wait); // DRIVE BY RELATIVE DISTANCE
void sDrive(double l_speed, double r_speed);                       // drive by spinning (speed L, speed r)
void rLift(double deg, double speed, bool wait); // LIFT BY RELATIVE DISTANCE (deg, speed, wait)
void aLift(double deg, double speed, bool wait); // LIFT BY ABSOLUTE DISTANCE (deg, speed, wait)
void rClaw(double deg, double speed, bool wait); // LIFT BY ABSOLUTE DISTANCE (deg, speed, wait)
void openClaw(int deg);              // open claw (deg)
void closeClaw(double speed);           // close claw (speed)
void drive2Target(double target);        // drive to target (distance in mm)
void drive2Cube(void);
void grabCube( int num_Cubes);    //GRAB CUBE - (# OF CUBES)
void turnTo(int degrees, double maxVoltage);

void autonomous(void) {
   //position claw - DO NOT REMOVE
  //homeClaw();  
//drive2Target(dist2Cube);

//Drive.driveFor(forward, 12, distanceUnits::in, 40, velocityUnits::pct);
//Drive.turnFor(right, 90, rotationUnits::deg);
turnTo(90,25);
wait(2,sec);
turnTo(-90,25);
//  Go.driveFor(directionType::fwd, 6, distanceUnits::in, 50, velocityUnits::pct);
//  Go.turnFor(90, rotationUnits::deg, 30, velocityUnits::pct);

}//END AUTOMOUS


void usercontrol(void) {
  
  // START TASK for LIFT
  vex::task t(tLift); // start task which updates controller screen
  t.setPriority(6);

  buzz_on = 1;

  while (1) {
    // DRIVE

    if (abs(Controller1.Axis1.value() + Controller1.Axis2.value()) < 120 && abs(Controller1.Axis1.value() - Controller1.Axis2.value()) < 120) {
      adjSpeed = 0.3; // ADJUST LOW-SPEED SENSITIVITY  0.2 LESS SENSITIVE, 0.5 MORE SO
    } else {
      adjSpeed = 1.0;
    }
    
    //LEFT / RIGHT DRIVE
    L_Drive.spin(directionType::fwd,(Controller1.Axis1.value() + Controller1.Axis2.value()) * adjSpeed, velocityUnits::pct);
    R_Drive.spin(directionType::fwd,(Controller1.Axis1.value() - Controller1.Axis2.value()) * adjSpeed,  velocityUnits::pct);

    // MICRO-MOVES
    if (Controller1.ButtonX.pressing()) { // move fwd
      sDrive(15, 15);
    } else if (Controller1.ButtonB.pressing()) { // move back
      sDrive(-15, -15);
    } else if (Controller1.ButtonY.pressing()) { // turn left
      sDrive(-15, 15);
    } else if (Controller1.ButtonA.pressing()) { // turn right
      sDrive(15, -15);
    } 

    vex::task::sleep(100); // Sleep the task for a short amount of time to prevent wasted resources.

  } // end while
}

void turnTo(int d, double speed){
  Drive.setTurnVelocity(speed, pct);

  Drive.turnFor(d, deg);
 
  L_Drive.stop();
  R_Drive.stop();
}//end turnTo

int tLift(void) { // ARM & CLAW TASK
  while (1) {
    if (Controller1.Axis3.value() > deadBand && Lift.position(vex::rotationUnits::deg) < 470) { // RAISE LIFT W/SOFT LIMIT OF 500 DEG
      Lift.spin(vex::directionType::fwd, Controller1.Axis3.value() * adjLift, vex::velocityUnits::pct);
    } else if (Controller1.Axis3.value() < deadBand * -1.0 && Lift.position(vex::rotationUnits::deg) > 0) { // SLOWLY-LOWER LIFT W/SOFT LIMITS AT 0 DEG
      Lift.spin(vex::directionType::fwd, Controller1.Axis3.value() * (adjLift * 0.2), vex::velocityUnits::pct);
    } else if (Controller1.ButtonL2.pressing()) { // claw open
          Claw.spin(vex::directionType::rev, 75, vex::velocityUnits::pct);
    } else if (Controller1.ButtonL1.pressing()) { // claw close
          if(Claw.current(vex::percentUnits::pct) < 70){
              Claw.spin(vex::directionType::fwd, 75, vex::velocityUnits::pct);}          
    } else if (Controller1.ButtonR1.pressing()) { 
      drive2Target(dist2Cube);
    } else if (Controller1.ButtonR2.pressing()) { // claw open
      grabCube(4);
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

  double delay = 500;

  while (1) {
    dist_mm = Dist.distance(vex::distanceUnits::mm);
    ang = Gyro.angle();
    degr = Gyro.rotation();
    head = Gyro.heading();
    //BUZZ JOYSTICK - 
    if(buzz_on){
      if (dist_mm < dist2Cube + 10 && dist_mm > dist2Cube - 10 && Lift.rotation(rotationUnits::deg) > 60.0) { // distance to target within 20mm
        Controller1.rumble(".");                 // short rumble
        delay = delay * 2;
        vex::this_thread::sleep_for(delay);
      } else if (dist_mm < dist2Cube - 10 && Lift.rotation(rotationUnits::deg)> 60.0) { // too close
        Controller1.rumble("--");                                    // long rumble
        delay = delay * 2;
        vex::this_thread::sleep_for(delay);
      } else{delay = 500;}
    }
    vex::this_thread::sleep_for(100);
  }
  return 0;
} // end read_sonar(void);           //read sonar task

void rDrive(double lDeg, double rDeg, double l, double r, bool b) { // drive by relative distance
  wait(0.2, seconds);   //let robot settle
  L_Drive.rotateFor(lDeg * adjField, vex::rotationUnits::deg, l, vex::velocityUnits::pct, false);
  R_Drive.rotateFor(rDeg * adjField, vex::rotationUnits::deg, r, vex::velocityUnits::pct, b);
} // end rDrive

void sDrive(double lSpeed, double rSpeed) { // drive by spin
  L_Drive.spin(vex::directionType::fwd, lSpeed, vex::velocityUnits::pct);
  R_Drive.spin(vex::directionType::fwd, rSpeed, vex::velocityUnits::pct);
  wait(0.2, seconds);   //let robot settle
} // end sDrive

void drive2Cube(){
  double deg2target = (dist_mm - dist2Cube - 50) / 0.866666;
  
  rDrive(deg2target, deg2target, 30,30, 0);
  wait(300,msec);
}

void drive2Target(double target) { // drive by spin

  //if robot's moving let it settle
  if(L_Drive.isSpinning() || R_Drive.isSpinning()){
    L_Drive.stop(hold);
    R_Drive.stop(hold);
    wait(200,msec);}


// if(dist_mm <=0 || dist_mm > 99999){
//   waitUntil(dist_mm > 0 || dist_mm < 99999);}

  wait(200,msec);
  L_Drive.setBrake(hold);
  R_Drive.setBrake(hold);
  double deg2target = (dist_mm - target - 40) / 0.866666;
  
  rDrive(deg2target, deg2target, Dist.distance(vex::distanceUnits::mm)/target * 8 ,Dist.distance(vex::distanceUnits::mm)/target * 8 , 1);


} // end drive2Target

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

void rClaw(double deg, double speed, bool b) { // position lift by relative position
  Claw.rotateFor(deg, vex::rotationUnits::deg, speed, vex::velocityUnits::pct, false); // This command must be non blocking.

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
  Claw.resetRotation(); // make position home

  Lift.rotateFor(20, vex::rotationUnits::deg, 10, vex::velocityUnits::pct,true); // lift claw to mid-cube
  Lift.resetRotation(); // make position home

} // end  homeClaw(void);

void grabCube(int n){
  
  if(n == 0 || n >5){n = 1;}
  
  int liftTo =  60 * n;
    
  if(Lift.rotation(deg) < 80){aLift(80, 40, 1);}        // lift claw so sensor can 'see' 

  aLift(liftTo, 60, 1);        // lift claw to top of stack

  drive2Target(dist2Cube); // NEW - drive to about dist2Cube from next cube
  rLift(-40,20,1);  //lower cube to almost touch targer cube
  openClaw(75);   //open claw a little
  aLift(0,20,1);
  closeClaw(30); 


}

// SYSTEM SET-UP
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // START DISPLAY ENCODER TO CONTROLLER TASK
  if (display) {
      vex::task dsp(updateScreen); // start task
      dsp.setPriority(3);          // set low priority
    }
  
  //START TASK TO READ SONAR (DISTANCE)
  vex::task dst(read_sonar);
  dst.setPriority(6);

  // set motor defayults
  setUpMotor(L_Drive, 'H');
  setUpMotor(R_Drive, 'H');
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
    Controller1.Screen.print("A: %4.0f D: %4.0f", ang, degr); // LEFT, RIGHT MOTOR
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print("H: %4.0f", head); // LIFT CLAW
    Controller1.Screen.setCursor(3, 1);
    Controller1.Screen.print("Distance: %5.0f", dist_mm);

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
    wait(500, msec);
  }
}
