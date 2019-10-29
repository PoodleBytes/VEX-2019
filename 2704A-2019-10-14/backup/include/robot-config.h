#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"
//
using namespace vex;
vex::brain Brain;

vex::motor R_Rear_Drive (vex::PORT16, vex::gearSetting::ratio18_1,true);
vex::motor L_Rear_Drive (vex::PORT17, vex::gearSetting::ratio18_1,true);
vex::motor R_Lift (vex::PORT2, vex::gearSetting::ratio36_1,false);
vex::motor L_Lift (vex::PORT9, vex::gearSetting::ratio36_1,true);
vex::motor Claw (vex::PORT4, vex::gearSetting::ratio18_1,true);

vex::controller Controller1;