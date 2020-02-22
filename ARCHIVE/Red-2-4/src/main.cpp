/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\VEX                                              */
/*    Created:      Tue Feb 04 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
}

HI JANE - INCASE YOU CAN'T GET MY FILE FROM GITHUB POSTED 
THE AUTO() AND DRIVE() FUNCTIONS BELOW

ALSO ATTACHED THE WHOLE CONTENTS OF MAIN.CPP

void autonomous(void) {
   //position claw - DO NOT REMOVE
  homeClaw();  

  /* BLUE -    4 POINT AUTONOMOUS - GRAB CUBE & STACK -  */
  // ** - TURNS OPPOSITE FOR RED/BLUE

  //DRIVE TO FIRST CUBE
  drive2Target(dist2Cube);
  //GRAB CUBE
  closeClaw(70);
  //LIFT CUBE TO PLACE ON 4-STACK                    
  rLift(220, 40, 1);            
  //DRIVE TO 4-STACK
  drive2Target(dist2Cube);   
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
  rDrive(500, -500, 40, 40, 1); 
  //DRIVE TORWARDS WALL
  drive2Target(300);    //300mm ~ 12"
  //** TURN LEFT 90 DEG TOWARD CORNER
  rDrive(-360, 360, 40, 40, 1); 
  //DRIVE TORWARDS WALL
  drive2Target(200);    //200mm ~ 8"
  //LOWER (HOME) CLAW 
  aLift(0,25,1);
  //OPEN CLAW
  openClaw(40);
  //RUN AWAY!!!
  rDrive(-1000, -1000, 80, 80, 1);

}//END AUTOMOUS




void drive2Target(double stopHere) { // drive by spin

  int speed;    //set motor speed
  bool b = 1;
  
  // reset motor encoders & set braking
  L_Drive.resetRotation();
  R_Drive.resetRotation();

 while (b) {

    while (dist_mm > (stopHere)) { // distance from object > target distance + buffer
      
      if (dist_mm > 4 * stopHere) {  //more than 4x target
        speed = 60;                                 //go 60% speed
      } else if(dist_mm >= 3 * stopHere && dist_mm < 4 * stopHere){  // >3x but <4x target - slow a little
        speed = (40);    //less than 60% from target start slowing
      } else if(dist_mm >= 2 * stopHere && dist_mm < 3 * stopHere){
        speed = (30);    //less than 3x target start slowing
      } else{speed = 20;}

      L_Drive.spin(vex::directionType::fwd, speed, vex::velocityUnits::pct);
      R_Drive.spin(vex::directionType::rev, speed, vex::velocityUnits::pct);
      
      wait(10,msec);
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

