using namespace vex;

extern brain Brain;
extern controller Controller1;

// VEXcode devices
extern smartdrive DriveTrain;
extern inertial Gyro;

extern motor R_Drive;
extern motor L_Drive;
extern motor R_Lift;
extern motor L_Lift;
extern motor L_Intake;
extern motor R_Intake;
extern motor R_Stand;
extern motor L_Stand;

// FUNCTIONS
extern void pre_auton(void); // SETTINGS FOR MOTORS, SCREEN ETC
extern void setUpMotor(motor(M));
extern int updateScreen(); // DISPLAY ENCODER TASK
extern void unfold(void);
extern void raiseStand(void);

// VARIABLES
extern double adjSpeed; // adjust drive sensitivity
extern double adjField; // adjust automouse drive() degrees match conditions

// MOVEMENT / CONTROL
extern int tLift();                                 // arm-relatecd tasks / buttons
extern void aDrive(double, double, int, int, bool); // DRIVE BY RELATIVE DISTANCE
extern void aLift(double, int, bool);               // ARM BY RELATIVE DISTANCE
extern void aIntake(double, int, bool);             // INTAKE RELATIVE DISTANCE
extern void aStand(double, int, bool);              // STAND BY RELATIVE DISTANCE
extern motor_group Intake;
extern motor_group Lift;
extern motor_group Stand;


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );