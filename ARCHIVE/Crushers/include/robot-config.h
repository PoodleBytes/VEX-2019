using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor Claw;
extern motor L_Lift;
extern motor L_Rear;
extern motor R_Rear;
extern motor L_Front;
extern motor R_Front;
extern motor R_Lift;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );