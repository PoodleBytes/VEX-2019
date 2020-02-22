using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor L_Claw;
extern motor R_Claw;

extern motor L_Lift;
extern motor R_Lift;

extern motor LF_Drive;
extern motor RF_Drive;
extern motor LR_Drive;
extern motor RR_Drive;

extern motor_group L_Drive;
extern motor_group R_Drive;
extern motor_group Claw;
extern motor_group Lift;


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );