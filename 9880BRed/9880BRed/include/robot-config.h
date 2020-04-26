using namespace vex;

extern brain Brain;

// VEXcode devices

extern controller Controller1;
extern motor liftR;
extern motor liftL;
extern motor spinL;
extern motor spinR;
extern motor jointTurn;
extern motor driveLF;
extern motor driveRF;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );