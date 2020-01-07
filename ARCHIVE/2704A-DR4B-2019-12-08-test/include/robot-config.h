using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor L_Drive;
extern motor R_Drive;
extern motor L_Lift;
extern motor R_Lift;
extern motor Claw;
extern motor_group Lift;
extern double adjField;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );
