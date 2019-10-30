/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\John                                             */
/*    Created:      Mon Oct 21 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// R_Drive              motor         3               
// L_Drive              motor         2               
// R_Lift               motor         6               
// L_Lift               motor         7               
// L_Intake             motor         4               
// R_Intake             motor         5               
// Stand                motor         8               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;


//Creates a competition object that allows access to Competition methods.
vex::competition Competition;

//FUNCTIONS
void pre_auton(void);       //SETTINGS FOR MOTORS, SCREEN ETC
void setUpMotor(motor(M));
int updateScreen();			    //DISPLAY ENCODER TASK

//VARIABLES
double adjSpeed = .8;   //adjust drive sensitivity
double adjField = 1.0;  //adjust automouse drive() degrees match conditions

//MOVEMENT / CONTROL
int tLift();                                  //arm-relatecd tasks / buttons
void rDrive(double, double, int, int, bool);  //DRIVE BY RELATIVE DISTANCE
void rLift(double, int, bool);                //ARM BY RELATIVE DISTANCE 
void rIntake(double, int, bool);                //INTAKE RELATIVE DISTANCE
void rStand(double, int, bool);                            //STAND BY RELATIVE DISTANCE

void autonomous( void ) { 
  rLift(-400, 100 ,1);
  rLift(100, 50, 0);
  rDrive(1400, 800, 30, 30,0);
  rIntake(4000, 100, 1);
  rDrive(-300, 300, 45, 45, 1);
  rDrive(400, 400, 40, 40, 1);
  
  

  /*
	PUT AUTONOMOUS CODE HERE*/
  
}//end autonomous

void usercontrol( void ) {
    //ONE TIME COMMANDS RAN B4 USER CONTROL
    
    //START TASK for LIFT
    vex::task t(tLift); //start task which updates controller screen
    t.setPriority(6);  //medium priority

     vex::task upScr(updateScreen); //start task 
      upScr.setPriority(4);  //set low priority
 
    while (1){  
      //DRIVE
        L_Drive.spin(vex::directionType::fwd, Controller1.Axis3.position() * adjSpeed, vex::velocityUnits::pct);
        R_Drive.spin(vex::directionType::fwd, Controller1.Axis2.position() * adjSpeed, vex::velocityUnits::pct);
        vex::task::sleep(100); //Sleep the task for a short amount of time to prevent wasted resources. 
        
    }//end while
}



int tLift(void){ //ARM & Stand TASK L2= out take  R2 = intake   L1 = up  R1 = down
    while(1){
      if(Controller1.ButtonR1.pressing()){  
          L_Intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
          R_Intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
      }else if(Controller1.ButtonR2.pressing()){    
          L_Intake.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
          R_Intake.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
      }else if(Controller1.ButtonL2.pressing() && L_Lift.rotation(deg)<-30){
          L_Lift.spin(vex::directionType::fwd, 80, vex::velocityUnits::pct);
          R_Lift.spin(vex::directionType::fwd, 80, vex::velocityUnits::pct);
      }else if(Controller1.ButtonL1.pressing() && L_Lift.rotation(deg)>-300){
          L_Lift.spin(vex::directionType::rev, 80, vex::velocityUnits::pct);
          R_Lift.spin(vex::directionType::rev, 80, vex::velocityUnits::pct);
      }else if(Controller1.ButtonB.pressing()){
          //do something like call a function()
          Stand.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);  
      }else if(Controller1.ButtonX.pressing()){
          //do something like call a function()
          Stand.spin(vex::directionType::rev, 50, vex::velocityUnits::pct);           
      }else{
          L_Lift.stop(brakeType::hold);
          R_Lift.stop(brakeType::hold);
          L_Intake.stop(brakeType::hold);
          R_Intake.stop(brakeType::hold);
          Stand.stop(brakeType::coast);
      }
      vex::task::sleep(20);
    }   
    return 0;
}//end tArm

void rDrive(double lDeg, double rDeg, int l, int r, bool b){  // drive by relative distance
    lDeg = lDeg * adjField; //adjust distance based on field
    rDeg = rDeg * adjField;
    L_Drive.rotateFor(lDeg * adjField, vex::rotationUnits::deg,l, vex::velocityUnits::pct, false); //This command must be non blocking.
    R_Drive.rotateFor(rDeg* adjField, vex::rotationUnits::deg,r, vex::velocityUnits::pct, false); //This command must be non blocking.

    if(b){  // b = tue means wait for motors stop spinning or timeout
      while(R_Drive.isSpinning()||L_Drive.isSpinning())
        {}
    }
}//end rDrive

void rLift(double deg, int s, bool b){
    L_Lift.rotateTo(deg, vex::rotationUnits::deg,s, vex::velocityUnits::pct, false); //This command must be non blocking.
    R_Lift.rotateTo(deg, vex::rotationUnits::deg,s, vex::velocityUnits::pct, b); //This command is blocking so the program will wait here until the right motor is done.  
    
    if(b){  // b = tue means wait for motors stop spinning or timeout
      while(R_Lift.isSpinning()||L_Lift.isSpinning())
        {}
    }
}//end rLift

void rIntake(double deg, int s, bool b){
    L_Intake.rotateFor(deg, vex::rotationUnits::deg,s, vex::velocityUnits::pct, false); //This command must be non blocking.
    R_Intake.rotateFor(deg, vex::rotationUnits::deg,s, vex::velocityUnits::pct,b); //This command is blocking so the program will wait here until the right motor is done.  
    
    if(b){  // b = tue means wait for motors stop spinning or timeout
      while(R_Intake.isSpinning()||L_Intake.isSpinning())
        {}
    }
}//end rIntake

void rStand(double deg, int s, bool b){
  Stand.rotateFor(deg, vex::rotationUnits::deg,s, vex::velocityUnits::pct, false);
}

int updateScreen(){
    while(1){
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("Left: %4.1f", L_Drive.rotation(rotationUnits::deg));   //LEFT MOTOR 
        Controller1.Screen.setCursor(2,1);
        Controller1.Screen.print("Right: %4.1f",R_Drive.rotation(rotationUnits::deg));   //RIGHT MOTOR        
        Controller1.Screen.setCursor(3,1);
        Controller1.Screen.print("L: %4.2f S: %4.2f", L_Lift.rotation(rotationUnits::deg),Stand.rotation(rotationUnits::deg));   //LIFT AND STAND
		
	 //RESET ENCODERS
      if(Controller1.ButtonA.pressing()){   //DRIVE ENCODERS
          L_Drive.resetRotation();
          R_Drive.resetRotation();
      }
      if(Controller1.ButtonY.pressing()){   //ARM AND GRABBER ENCODERS
          L_Lift.resetRotation();
          Stand.resetRotation();
      }

        // don't hog the cpu :)
        vex::this_thread::sleep_for( 250 );    

        }//end while
    return 0;
}//END UPDATE SCREEN


//SYSTEM SET-UP
void pre_auton(void) {  
  //set motor defayults 
	setUpMotor(L_Drive);
  setUpMotor(R_Drive);  
  setUpMotor(L_Lift);
  setUpMotor(R_Lift);
  setUpMotor(L_Intake);
  setUpMotor(R_Intake);
  setUpMotor(Stand);


    //clear controller display
    Controller1.Screen.clearScreen();
} //end pre_auton  

//DEFAULT MOTOR SETTINGS
void setUpMotor(motor(M)){
    M.setMaxTorque(90,percentUnits::pct); //set torque
    M.setBrake(brakeType::brake);          //braking
    M.setTimeout(3,timeUnits::sec);       //set motor timeout
    M.resetRotation();                    //reset encoders
}

// Main will set up the competition functions and callbacks.
int main() {    
    //Run the pre-autonomous function. 
    pre_auton();
    //Set up callbacks for autonomous and driver control periods.
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );
    //Prevent main from exiting with an infinite loop.                        
    while(1) {
      vex::task::sleep(100);//Sleep the task for a short amount of time to prevent wasted resources.
    } 
}
