/*---------------------------------------------------------------------------*/
//  NOTES.TXT FOR NOTES / USEFUL SETTINGS
/*---------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices in ROBOT-CONFIG

void autonomous(void) {
   //position claw - DO NOT REMOVE
homeClaw();  


closeClaw(80);
aLift(75,50,1);

drive2Target(dist2Cube, 25, 1);
/*
aLift(0,10,0);
openClaw(-25, 40);
closeClaw(80);
aLift(70,40,1);

Drive.turnFor(right, 100, rotationUnits::deg);
Drive.driveFor(fwd, 4,distanceUnits::in,1);

aLift(0,15,1);
openClaw(-25, 55);
Drive.driveFor(reverse, 5, distanceUnits::in,1);
Drive.turnFor(left, 175, rotationUnits::deg);
Drive.driveFor(fwd, 2,distanceUnits::in,1);
closeClaw(60);

aLift(250,50,0);
Drive.turnFor(left, 5, rotationUnits::deg);
Drive.driveFor(fwd, 650,distanceUnits::mm,1);
openClaw(-35, 65);//*/



//varMotorDist(dist2Cube, 1);

}//END AUTOMOUS


void usercontrol(void) {

  
// if(!vexCompetitionStatus()){//If not competitionswitch or field control connected
//     bool isAllowed = false;
//     while(!isAllowed){
//       if (Controller1.ButtonA.pressing() && Controller1.ButtonB.pressing()) isAllowed = true; // If both buttons pressing
//       vex::task::sleep(200);
//       }
//     }
  
  // START TASK for LIFT
  vex::task t(tLift); // start task which updates controller screen
  t.setPriority(6);

  buzz_on = 1;

  while (1) {
    // DRIVE

    // ADJUST LOW-SPEED SENSITIVITY  0.2 LESS SENSITIVE, 0.5 MORE SO
    (abs(Controller1.Axis1.value() + Controller1.Axis2.value()) < 120 && abs(Controller1.Axis1.value() - Controller1.Axis2.value()) < 120) ?
      adjSpeed = 0.3  :  adjSpeed = 1.0;
 
    
    //LEFT / RIGHT DRIVE
    L_Drive.spin(directionType::fwd,(Controller1.Axis1.value() + Controller1.Axis2.value()) * adjSpeed, velocityUnits::pct);
    R_Drive.spin(directionType::fwd,(Controller1.Axis1.value() - Controller1.Axis2.value()) * adjSpeed * -1,  velocityUnits::pct);

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
    } else if (Controller1.ButtonR2.pressing()) { // claw open
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

void varMotorRot(double target, bool b){

//zero endoders
L_Drive.resetRotation();
L_Drive.resetRotation();

double V;
//target / encoder > 0.25 ? 8 : target / encoder * 16

while( L_Drive.rotation(deg) < target ) {
    if(L_Drive.rotation(deg) / target > 0.5){
      V = 8;
    } else{
      V = target / L_Drive.rotation(deg) * 4;
    }
    
    L_Drive.spin(fwd, V, voltageUnits::volt);
    R_Drive.spin(fwd, V, voltageUnits::volt);
}

if (b) { // b = tue means wait for motors stop spinning or timeout
  while (R_Drive.isSpinning() || L_Drive.isSpinning()) {
  }
}

L_Drive.stop(hold);
R_Drive.stop(hold);


}//end varMotorRotation

void varMotorDist(double target, bool b){

wait(200,msec);

double V;

  while( target < dist_mm ) {
    if(dist_mm > 2 * target){
      V = 8;
    } else{
      V = (dist_mm - target) / dist_mm *2;
    }
    
    L_Drive.spin(fwd, V, voltageUnits::volt);
    R_Drive.spin(fwd, V, voltageUnits::volt);

  }

  if (b) { // b = tue means wait for motors stop spinning or timeout
    while (R_Drive.isSpinning() || L_Drive.isSpinning()) {
    }
  }

  L_Drive.stop(hold);
  R_Drive.stop(hold);

}//end varMotor


void drive2Target(double target, double s, bool b) { // drive to target distance

  //if robot's moving let it settle
  if(L_Drive.isSpinning() || R_Drive.isSpinning()){
    L_Drive.stop(hold);
    R_Drive.stop(hold);
    wait(200,msec);}

  wait(200,msec);
//IF THERE IS NOTHING IN RANGE QUITE
  if(dist_mm <=0 || dist_mm > 99999){return;}

//raise lift if low
  if(Dist.distance(vex::distanceUnits::mm) < 150){aLift(80, 30, 1);} 

  double mm2target = (Dist.distance(vex::distanceUnits::mm) - target - 20 );  // 0.866666;
  Drive.setDriveVelocity(s, pct);
  Drive.driveFor(fwd, mm2target, mm);

  if (b) { // b = tue means wait for motors stop spinning or timeout
    while (R_Drive.isSpinning() || L_Drive.isSpinning()) {
    }
  }

  L_Drive.stop(hold);
  R_Drive.stop(hold);

} // end drive2Target

void aLift(double deg, double speed, bool b) { // position lift by absolute position
  Lift.rotateTo(deg, vex::rotationUnits::deg, speed, vex::velocityUnits::pct, b); // This command must be non blocking.

  if (b) { // b = tue means wait for motors stop spinning or timeout
    while (Lift.isSpinning()) {
    }
  }
} // end aArm

void rLift(double deg, double speed, bool b) { // position lift by relative position
  Lift.rotateFor(deg, vex::rotationUnits::deg, speed, vex::velocityUnits::pct, b); // This command must be non blocking.

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
  } while (Claw.current(vex::percentUnits::pct) < 80);
  Claw.stop(hold);
} // end rClaw

void openClaw(int d, int s) {
  Claw.setVelocity(s, pct);
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
  int liftTo =  70 * n;
    
  aLift(liftTo, 35, 1);        // lift claw to top of stack
  drive2Target(dist2Cube, 25, 1);
  aLift(0,10,0);
  openClaw(-35, 25);
  wait(200,msec);
  closeClaw(60);
  aLift(70,30,1);
}

// SYSTEM SET-UP
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE 

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
  Brain.Screen.clearScreen();
  
  vexcodeInit();

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
    Controller1.Screen.print("mm: %5.0f A: %3.0f", dist_mm, Gyro.angle(deg));

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
