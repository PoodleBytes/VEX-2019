using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor L_Drive;
extern motor R_Drive;
extern motor UL_Lift;
extern motor LL_Lift;
extern motor UR_Lift;
extern motor LR_Lift;
extern motor Claw;
extern sonar Dist;
extern motor_group Lift;
extern smartdrive Drive;
extern inertial Gyro;

// VARIABLES
extern double adjField; // adjust automomouse for different fields
extern double adjSpeed; // adjust drive sensitivity
extern double adjLift;  // adjust arm's sensitivity
extern double dist_mm;        //sonar distance in mm
extern int deadBand; // range below which joystick is ignored
extern bool display; // set to true (1) will displays encoders on controller, false will not
extern bool buzz_on;
extern int dist2Cube; // mm to grab cube

// MOVEMENT / CONTROL
extern void drive2Cube(void);
extern void rDrive(double l_deg, double r_deg, double l_speed, double r_speed, bool wait); // DRIVE BY RELATIVE DISTANCE
extern void sDrive(double l_speed, double r_speed);                       // drive by spinning (speed L, speed r)
extern void rLift(double deg, double speed, bool wait); // LIFT BY RELATIVE DISTANCE (deg, speed, wait)
extern void aLift(double deg, double speed, bool wait); // LIFT BY ABSOLUTE DISTANCE (deg, speed, wait)
extern void rClaw(double deg, double speed, bool wait); // LIFT BY ABSOLUTE DISTANCE (deg, speed, wait)
extern void openClaw(int deg, int speed);              // open claw (deg)
extern void closeClaw(double speed);           // close claw (speed)
extern void drive2Target(double target, double s, bool b);        // drive to target (distance in mm)
extern void grabCube( int num_Cubes);    //GRAB CUBE - (# OF CUBES)
extern void turnTo(int degrees, double maxVoltage);
extern void varMotorRot(double target, bool b);//distance
extern void varMotorDist(double target, bool b);// degrees
 

// PROTOTYPE FUNCTION
extern void pre_auton(void);            // SETTINGS FOR MOTORS, SCREEN ETC
extern void setUpMotor(motor(M), char); // default motor settings
extern int updateScreen();              // DISPLAY ENCODER TASK
extern int tLift();                     // arm-relatecd tasks / buttons
extern void homeClaw(void);             // set claw height and open position (home)
extern int read_sonar(void);            // read sonar task








/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );
