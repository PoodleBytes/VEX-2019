/*----------------------------------------------------------------------------*/
//  First Autonomous Vesion
//  NOTES.TXT FOR NOTES / USEFUL SETTINGS
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
//PROTOTYPE FUNCTION 
void pre_auton(void);       //SETTINGS FOR MOTORS, SCREEN ETC
void setUpMotor(motor(M));  //default motor settings
int updateScreen();			    //DISPLAY ENCODER TASK
int tLift();              //arm-relatecd tasks / buttons
void openClaw(void); //time in seconds
void closeClaw(double); //time in seconds

//VARIABLES
double adjField = 1.0;  //adjust automouse drive() degrees match conditions
double adjSpeed = 1.0;   //adjust drive sensitivity
double adjLift = 1.0;    //adjust arm's sensitivity
int deadBand = 10;      //range below which joystick is ignored
bool encode = 1;        //true displays encoders

//MOVEMENT / CONTROL
void rDrive(double, double, int, int, bool);  //DRIVE BY RELATIVE DISTANCE
void rLift(double, int, bool);                //ARM BY RELATIVE DISTANCE 
void aLift(double, int, bool);                //ARM BY ABSOLUTE DISTANCE
void sDrive(int, int);


void autonomous( void ) {
  
  //  TEST ONLY //
  closeClaw(0.75);  //closes claw
  rLift(360,75,1);  //LIFT ARM ABOUT 10"
  rDrive(-360,360,50,50,1); //TURN LEFT 
  rDrive(360,360,50,50,1); //DRIVE FORWARD ABOUT 12" (one rotation)
  openClaw(); //opens claw to start position

}//end autonomous

void usercontrol( void ) {
    //ONE TIME COMMANDS RAN B4 USER CONTROL
    
    //START TASK for LIFT
    vex::task t(tLift); //start task which updates controller screen

    //START DISPLAY ENCODER TO CONTROLLER TASK
    if(encode){
      vex::task upScr(updateScreen); //start task 
      upScr.setPriority(4);  //set low priority
    }

    while (1){  
      //DRIVE

      if(abs(Controller1.Axis1.value() + Controller1.Axis2.value())<80 && abs(Controller1.Axis1.value() - Controller1.Axis2.value())< 80){
        adjSpeed = 0.4;
      } else {adjSpeed = 1.0;}

      L_Drive.spin(directionType::fwd, (Controller1.Axis1.value() + Controller1.Axis2.value())*adjSpeed, velocityUnits::pct);
      R_Drive.spin(directionType::fwd, (Controller1.Axis1.value() - Controller1.Axis2.value())*adjSpeed, velocityUnits::pct);

      if(Controller1.ButtonX.pressing()){ //move fwd
          sDrive(-15,15); 
      }else if(Controller1.ButtonB.pressing()){   //move back
          sDrive(15,-15); 
      }else if(Controller1.ButtonY.pressing()){   //move left
          sDrive(-15,-15); 
      }else if(Controller1.ButtonA.pressing()){ //move right
        sDrive(15,15);
      }      

      vex::task::sleep(100); //Sleep the task for a short amount of time to prevent wasted resources. 
        
    }//end while
}


int tLift(void){ //ARM & CLAW TASK
    while(1){
      if(abs(Controller1.Axis3.value())>deadBand){  
        L_Lift.spin(vex::directionType::fwd, Controller1.Axis3.value()*adjLift, vex::velocityUnits::rpm);
        R_Lift.spin(vex::directionType::fwd, Controller1.Axis3.value()*adjLift, vex::velocityUnits::rpm);
      }else if(Controller1.ButtonL1.pressing()){    
          Claw.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct);
      }else if(Controller1.ButtonL2.pressing()){
          Claw.spin(vex::directionType::rev, 50, vex::velocityUnits::pct); 
      }else{
          L_Lift.stop(brakeType::hold);
          R_Lift.stop(brakeType::hold);
          Claw.stop(brakeType::hold);
      }
      vex::task::sleep(5);
    }   
    return 0;
}//end tLift

void rDrive(double lDeg, double rDeg, int l, int r, bool b){  // drive by relative distance
  rDeg = rDeg * -1;
  L_Drive.rotateFor(lDeg * adjField, vex::rotationUnits::deg,l, vex::velocityUnits::pct, false); 
  R_Drive.rotateFor(rDeg * adjField, vex::rotationUnits::deg,r, vex::velocityUnits::pct, b); 
}//end rDrive

void sDrive(int lValue, int  rValue){  // drive by spin
  L_Drive.spin(vex::directionType::fwd, lValue, vex::velocityUnits::pct);
  R_Drive.spin(vex::directionType::fwd, rValue, vex::velocityUnits::pct);
}//end rDrive

void aLift(double deg, int s, bool b){  //position lift by absolute position
    L_Lift.rotateTo(deg, vex::rotationUnits::deg,s, vex::velocityUnits::pct, false); //This command must be non blocking.
    R_Lift.rotateTo(deg, vex::rotationUnits::deg,s, vex::velocityUnits::pct, b); //This command is blocking so the program will wait here until the right motor is done.  
    
    if(b){  // b = tue means wait for motors stop spinning or timeout
      while(R_Lift.isSpinning()||L_Lift.isSpinning())
        {}
    }
}//end aArm

void rLift(double deg, int s, bool b){  //position lift by relative position
    L_Lift.rotateFor(deg, vex::rotationUnits::deg,s, vex::velocityUnits::pct, false); //This command must be non blocking.
    R_Lift.rotateFor(deg, vex::rotationUnits::deg,s, vex::velocityUnits::pct,b); //This command is blocking so the program will wait here until the right motor is done.  
    
    if(b){  // b = tue means wait for motors stop spinning or timeout
      while(R_Lift.isSpinning()||L_Lift.isSpinning())
        {}
    }
}//end eArm

void closeClaw(double t){
  Claw.spin(vex::directionType::fwd, 80, vex::velocityUnits::pct);
  wait(t,seconds);
  Claw.stop(hold);
}//end rClaw

void openClaw(void){
  Claw.setVelocity(75, pct);
  Claw.spinTo(0,deg);   //spin to home position
  Claw.stop(brake);
}//end rClaw


//SYSTEM SET-UP
void pre_auton(void) {  
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  //set motor defayults 
	setUpMotor(L_Drive);
  setUpMotor(R_Drive);  
  setUpMotor(L_Lift);
  setUpMotor(R_Lift);
  setUpMotor(Claw);
    
    //clear controller display
    Controller1.Screen.clearScreen();
} //end pre_auton  

//DEFAULT MOTOR SETTINGS
void setUpMotor(motor(M)){
    M.setMaxTorque(90,percentUnits::pct); //set torque
    M.setStopping(brakeType::hold);          //braking
    M.setTimeout(3,timeUnits::sec);       //set motor timeout
    M.resetRotation();                    //reset encoders
}

int updateScreen(){
    while(1){
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("Left: %4.1f", L_Drive.rotation(rotationUnits::deg));   //LEFT MOTOR 
        Controller1.Screen.setCursor(2,1);
        Controller1.Screen.print("Right: %4.1f",R_Drive.rotation(rotationUnits::deg));   //RIGHT MOTOR        
        Controller1.Screen.setCursor(3,1);
        Controller1.Screen.print("L: %4.2f S: %4.2f", L_Lift.rotation(rotationUnits::deg),Claw.rotation(rotationUnits::deg));   //LIFT AND STAND
		
	 //RESET ENCODERS
      if(Controller1.ButtonA.pressing()){   //DRIVE ENCODERS
          L_Drive.resetRotation();
          R_Drive.resetRotation();
      }
      if(Controller1.ButtonB.pressing()){   //ARM AND GRABBER ENCODERS
          L_Lift.resetRotation();
          Claw.resetRotation();
      }

        // don't hog the cpu :)
        vex::this_thread::sleep_for( 250 );    

        }//end while
    return 0;
}//END UPDATE SCREEN

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
