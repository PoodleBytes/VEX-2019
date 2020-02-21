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


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );