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
bool buzz_on = 0;
int dist2Cube = 160; // mm to grab cube

// MOVEMENT / CONTROL
void rDrive(double l_deg, double r_deg, double l_speed, double r_speed, bool wait); // DRIVE BY RELATIVE DISTANCE
void sDrive(double l_speed, double r_speed);                       // drive by spinning (speed L, speed r)
void rLift(double deg, double speed, bool wait); // LIFT BY RELATIVE DISTANCE (deg, speed, wait)
void aLift(double deg, double speed, bool wait); // LIFT BY ABSOLUTE DISTANCE (deg, speed, wait)
void rClaw(double deg, double speed, bool wait); // LIFT BY ABSOLUTE DISTANCE (deg, speed, wait)
void openClaw(int deg);              // open claw (deg)
void closeClaw(double speed);           // close claw (speed)
void drive2Target(double dist);        // drive to target (distance in mm)
void grabCube( int num_Cubes);    //GRAB CUBE - (# OF CUBES)

void autonomous(void) {
   //position claw - DO NOT REMOVE
  homeClaw();  

  /* BLUE -    4 POINT AUTONOMOUS - GRAB CUBE & STACK -  */
  // ** - TURNS OPPOSITE FOR RED/BLUE

  //DRIVE TO FIRST CUBE
  drive2Target(dist2Cube);
  //GRAB CUBE
  closeClaw(70);
  //Drive forwards
  drive2Target(700);
  //LIFT CUBE TO PLACE ON 4-STACK                    
  rLift(220, 40, 1);            
  //DRIVE TO 4-STACK
  drive2Target(1400);   
  //LOWER LIFT TO PUT CUBE ONTOP OF STACK
  rLift(-50,40,1);            
  //OEPN CLAW
  openClaw(60);
  //SLOWLY LOWER CLAW TO GRAB BOTTOM CUBE
  aLift(20, 20, 1);    
  //GRAB STACK       
  closeClaw(75);
  //LIFT STACK
  rLift(70, 40, 1);          
  //** TURN RIGHT 180 DEG TO FACE STARTING WALL
  rDrive(-500, 500, 40, 40, 1); 
  //DRIVE TORWARDS WALL
  drive2Target(300);    //300mm ~ 12"
  //** TURN LEFT 90 DEG TOWARD CORNER
  rDrive(-30, 30, 40, 40, 1); 
  //DRIVE TORWARDS WALL
  drive2Target(200);    //200mm ~ 8"
  //LOWER (HOME) CLAW 
  aLift(0,25,1);
  //OPEN CLAW
  openClaw(40);
  //RUN AWAY!!!
  rDrive(-1000, -1000, 80, 80, 1);

}//END AUTOMOUS


void usercontrol(void) {
  // ONE TIME COMMANDS RAN B4 USER CONTROL
  // START TASK for LIFT
  vex::task t(tLift); // start task which updates controller screen
  t.setPriority(6);
  buzz_on = 1;

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

int tLift(void) { // ARM & CLAW TASK
  while (1) {
    if (Controller1.Axis3.value() > deadBand && Lift.position(vex::rotationUnits::deg) < 470) { // RAISE LIFT W/SOFT LIMIT OF 500 DEG
      Lift.spin(vex::directionType::fwd, Controller1.Axis3.value() * adjLift, vex::velocityUnits::pct);
    } else if (Controller1.Axis3.value() < deadBand * -1.0 && Lift.position(vex::rotationUnits::deg) > 0) { // SLOWLY-LOWER LIFT W/SOFT LIMITS AT 0 DEG
      Lift.spin(vex::directionType::fwd, Controller1.Axis3.value() * (adjLift * 0.2), vex::velocityUnits::pct);
    } else if (Controller1.ButtonL2.pressing() && Claw.rotation(deg) > 0) { // claw open
          Claw.spin(vex::directionType::rev, 75, vex::velocityUnits::pct);
    } else if (Controller1.ButtonL1.pressing() && Claw.current(vex::percentUnits::pct) < 50) { // claw close
          Claw.spin(vex::directionType::fwd, 75, vex::velocityUnits::pct);
    } else if (Controller1.ButtonR1.pressing()) { // claw close
      grabCube(1);
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

    //BUZZ JOYSTICK - 
    if(buzz_on){
      if (dist_mm < dist2Cube + 10 && dist_mm > dist2Cube - 10 && Lift.rotation(rotationUnits::deg) > 50.0) { // distance to target within 20mm
        Controller1.rumble(".");                 // short rumble
        delay = delay * 2;
        vex::this_thread::sleep_for(delay);
      } else if (dist_mm < dist2Cube - 10 && Lift.rotation(rotationUnits::deg)> 50.0) { // too close
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
  rDeg = rDeg * -1;
  L_Drive.rotateFor(lDeg * adjField, vex::rotationUnits::deg, l, vex::velocityUnits::pct, false);
  R_Drive.rotateFor(rDeg * adjField, vex::rotationUnits::deg, r, vex::velocityUnits::pct, b);
  wait(0.2, seconds);   //let robot settle
} // end rDrive

void sDrive(double lSpeed, double rSpeed) { // drive by spin
  rSpeed = rSpeed * -1;
  L_Drive.spin(vex::directionType::fwd, lSpeed, vex::velocityUnits::pct);
  R_Drive.spin(vex::directionType::fwd, rSpeed, vex::velocityUnits::pct);
  wait(0.2, seconds);   //let robot settle
} // end sDrive

void drive2Target(double stopHere) { // drive by spin

  if(L_Drive.isSpinning() || R_Drive.isSpinning()){wait(200,msec);}; //if robot's moving let it settle

  int speed = 20;    //set motor speed
  bool b = 1;
  
  // reset motor encoders & set braking
  L_Drive.resetRotation();
  R_Drive.resetRotation();

 while (b) {

    while (dist_mm > (stopHere)) { // distance from object > target distance + buffer
      
      //START DRIVING AT 20% TO MINIMIZE INITIAL JERK
      L_Drive.spin(vex::directionType::fwd, speed, vex::velocityUnits::pct);
      R_Drive.spin(vex::directionType::rev, speed, vex::velocityUnits::pct);
      
      wait(10,msec);

      if (dist_mm > 4 * stopHere) {  //more than 4x target
        speed = 60;                                 //go 60% speed
      } else if(dist_mm >= 3 * stopHere && dist_mm < 4 * stopHere){  // >3x but <4x target - slow a little
        speed = (40);    //less than 60% from target start slowing
      } else if(dist_mm >= 2 * stopHere && dist_mm < 3 * stopHere){
        speed = (30);    //less than 3x target start slowing
      } else{speed = 20;}

    } // end while

    L_Drive.stop(hold);
    R_Drive.stop(hold);

    //let robot settle
    wait(0.5,seconds);

    b = 0;    //exit function
  }
    L_Drive.setBrake(coast);
    R_Drive.setBrake(coast);
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

  Lift.rotateFor(25, vex::rotationUnits::deg, 40, vex::velocityUnits::pct,true); // lift claw to mid-cube
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

  // set motor defayults
  setUpMotor(L_Drive, 'B');
  setUpMotor(R_Drive, 'B');
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
    Controller1.Screen.print("L: %4.0f R: %4.0f", L_Drive.rotation(rotationUnits::deg), R_Drive.rotation(rotationUnits::deg)); // LEFT, RIGHT MOTOR
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print("L: %4.0f C: %4.0f", Lift.rotation(rotationUnits::deg), Claw.rotation(rotationUnits::deg)); // LIFT CLAW
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
  
  //START TASK TO READ SONAR (DISTANCE)
  vex::task dst(read_sonar);
  dst.setPriority(6);

  // START DISPLAY ENCODER TO CONTROLLER TASK
  if (display) {
    vex::task dsp(updateScreen); // start task
    dsp.setPriority(3);          // set low priority
  }

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(500, msec);
  }
}
