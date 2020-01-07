using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor L_Drive;
extern motor R_Drive;
extern motor M_Drive;
extern motor L_Lift;
extern motor R_Lift;
extern motor Claw;
extern sonar Dist;
extern motor_group Lift;


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );
