
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Claw                 motor         6               
// L_Lift               motor         5               
// L_Rear               motor         1               
// R_Rear               motor         2               
// L_Front              motor         9               
// R_Front              motor         10              
// R_Lift               motor         4               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
int updateScreen(void);   //task for updating Brain LCD

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  // All activities that occur before the competition starts
  L_Claw.setBrake(hold);
  R_Claw.setBrake(hold);
  L_Lift.setBrake(hold);
  R_Lift.setBrake(hold);

}

/*---------------------------------------------------------------------------*/
/*                              Autonomous Task                              */
/*---------------------------------------------------------------------------*/

void autonomous(void) {

L_Drive.resetPosition();
R_Drive.resetPosition();
//drive backward 23"
L_Drive.rotateTo(-720, vex::rotationUnits::deg, 80, vex::velocityUnits::pct, false);
R_Drive.rotateTo(-720, vex::rotationUnits::deg, 80, vex::velocityUnits::pct, true);

L_Drive.stop(hold);
R_Drive.stop(hold);

L_Drive.rotateTo(0, vex::rotationUnits::deg, 80, vex::velocityUnits::pct, false);
R_Drive.rotateTo(0, vex::rotationUnits::deg, 80, vex::velocityUnits::pct, true);

L_Drive.stop(hold);
R_Drive.stop(hold);
}


void usercontrol(void) {
  // User control code here, inside the loop
  vex::task t1(updateScreen); // start task which updates controller screen
  t1.setPriority(6);          //set task to low priority
  

  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.
      L_Drive.spin(directionType::fwd, (Controller1.Axis3.value() + Controller1.Axis4.value()), velocityUnits::pct);
      R_Drive.spin(directionType::fwd, (Controller1.Axis3.value() - Controller1.Axis4.value()), velocityUnits::pct);

      Lift.spin(directionType::fwd, (Controller1.Axis2.value()), velocityUnits::pct);

      if(Controller1.ButtonR1.pressing()){    
          Claw.spin(vex::directionType::fwd, 25, vex::velocityUnits::pct);
      }else if(Controller1.ButtonR2.pressing()){
          Claw.spin(vex::directionType::rev, 25, vex::velocityUnits::pct); 
      }else{
          Claw.stop(brakeType::hold);
      }
    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
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

//UPDATE SCREEN
int updateScreen(void){
  while(1){

        Brain.Screen.clearLine(0,color::black);
        Brain.Screen.clearLine(1,color::black);
        Brain.Screen.setCursor(1,0);
        Brain.Screen.print("Claw Temp: L %f   R  %f ",L_Claw.temperature(fahrenheit), R_Claw.temperature(fahrenheit));
        Brain.Screen.setCursor(2,0);
        Brain.Screen.print("Lift Temp: L %f   R  %f ",L_Lift.temperature(fahrenheit), R_Lift.temperature(fahrenheit));
        Brain.Screen.render(); //push data to the LCD all at once to prevent image flickering

        /*
Two 'OBJECTS'to display motor values to brain:
1 - Brain.Screen : https://api.vexcode.cloud/v5/html/classvex_1_1brain_1_1lcd.html
2 - Motor : https://api.vexcode.cloud/v5/html/classvex_1_1motor.html

also may want to print to the controller: https://api.vexcode.cloud/v5/html/classvex_1_1controller_1_1lcd.html

List of motors
L_Claw;
R_Claw;
L_Lift;
R_Lift;
LF_Drive;
RF_Drive;
LR_Drive;
RR_Drive;
        */

  }//end while
  return 0;
}//end updateScreen
