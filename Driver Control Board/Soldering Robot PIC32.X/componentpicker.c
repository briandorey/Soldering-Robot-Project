/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

//#define enablezeroing
//define testmode

#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif



#include <plib.h>            /* Include to use PIC32 peripheral libraries     */
#include <sys/attribs.h>     /* For __ISR definition                          */
#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include "HardwareProfile.h"

// feeder location variables
const long feederArray[16]={
  0,
  106,
  213,
  320,
  426,
  533,
  640,
  746,
  853,
  960,
  1066,
  1173,
  1280,
  1386,
  1493,
  1600
};

// initialise variables
int speedArray[200];
long stepstomove = 0;
long currentposition = 0;
int previousposition = 0;

int currentFeeder = 0;
int previousFeeder = 0;
int pickaftermove = 0;

char feederrunning = 0;

float stepperSpeed = 400; // lower = faster
const float stepperAcceleration = 0.94; // closer to 1 = slower accelleration
const float stepperStartSpeed = 10000;
int decelerationPoint = 0;
float currentSpeed = 0;
int timeTillStep = 0;

char isZeroing = 0;

// Private function prototypes
void init_component_picker();
void Stepper_Move(long movepos, int feeder);
void __ISR(_TIMER_1_VECTOR,IPL5) Stepper_Step(void);
void PickerDown();
void PickerUp();
void ZeroFeeder();
void moveToPosition(int feeder);
char FeederStatus();


void init_component_picker()
{
    // disable the parallel master port
    PMCON = 0x00;
    // configure stepper drivers and air valve direction
    feederStepTRIS = 0;
    feederDirTRIS = 0;
    feederRelayTRIS = 0;
    feederXHomeTRIS = 1;
    feederZHomeTRIS = 1;
    pickerBusTRIS = 0x00;

    // turn off stepper and relay outputs
    pickerBus = 0;
    feederStep = 0;
    feederDir = 0;
    feederRelay = 0;

    // initialise Timer 1, used on the main picker loop
    OpenTimer1(T1_ON | T1_PS_1_64 | T1_SOURCE_INT, 10);
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_5);

    PickerUp();

    // Zero the picker X axis
    isZeroing = 0;
    #ifdef enablezeroing
        ZeroFeeder();
    #endif

}

char FeederStatus(){
    if (feederrunning == 1){
        return 1;
    }
    else{
        return 0;
    }

}

void Stepper_Move(long movepos, int feeder){

    if ((previousFeeder == feeder + 1) || (previousFeeder == feeder - 1)){
      stepperSpeed = 800;
    }
    else if ((previousFeeder == feeder + 2) || (previousFeeder == feeder - 2)){
      stepperSpeed = 600;
    }
    else{
       stepperSpeed = 300;
    }

    if ((movepos - currentposition) < 0){
        feederDir = 0;
        stepstomove = (movepos - currentposition) * -1;
    }
    else{
       feederDir = 1;
       stepstomove = (movepos - currentposition);
    }

    currentSpeed = stepperStartSpeed; // set the currentAcceleration to default
    decelerationPoint = 0;
    currentposition = movepos;
    feederrunning = 1;
    mT1IntEnable(1); // enable T1 to start the move
    mT1ClearIntFlag(); // clear the interrupt flag
}

void __ISR(_TIMER_1_VECTOR,IPL5) Stepper_Step(void){
    if (isZeroing == 0){ // main movement loop
        if (stepstomove > 0){ // still moving so send a step pulse and calculate the next delay
            feederStep = 1;
            DelayUs(5);
            feederStep = 0;
            stepstomove--;
            if (stepstomove >= decelerationPoint){
              if (currentSpeed > 1){
                  currentSpeed = currentSpeed * stepperAcceleration;
                  decelerationPoint = decelerationPoint + 1;
             } else {
                  currentSpeed = 1;
              }
              timeTillStep = stepperSpeed + currentSpeed;
              speedArray[decelerationPoint] = timeTillStep;

            }
            if (stepstomove < decelerationPoint){
              timeTillStep = speedArray[stepstomove + 1];
            }
            PR1 = timeTillStep;
        }
        else{ // picker has finished so stop the T1 timer and clear the interrupt flag
          if (pickaftermove == 1){
               PickerDown();
               DelayMs(20);
          }
          mT1IntEnable(0); // disable T1
          feederDir = 0;
          feederrunning = 0;
        }
    }
    else{ // zeroing loop
        if (feederXHome == 0){
        feederStep = 1;
        DelayUs(5);
        feederStep = 0;
        }
        else{ // zeroing finished
            isZeroing = 0;
            currentFeeder = 0;
            stepstomove = 0;
            currentposition = 0;
            previousposition = 0;
            previousFeeder = 0;
            pickaftermove = 0;
            feederrunning = 0;
            mT1IntEnable(0); // disable T1
        }
    }
    
    mT1ClearIntFlag(); // clear the interrupt flag

}

void PickerDown(){ // enable the picker relay
     feederRelay = 0;
}

void PickerUp(){ // disable the picker relay and wait until the picker reaches the top before continuing
    feederRelay = 1;
    #ifndef testmode
     while (feederZHome == 1) {
        DelayUs(10);
     }
    #endif
}

void ZeroFeeder() {
  // disable picker ram
  feederrunning = 1;
  PickerUp();
  feederDir = 0;
  isZeroing = 1;

  PR1 = 1500; // set zeroing speed

  mT1IntEnable(1); // enable T1 to start the move
  mT1ClearIntFlag(); // clear the interrupt flag

}

void moveToPosition(int feeder){
  pickaftermove = 1;
  // disable picker ram
  PickerUp();

  if (feeder == previousFeeder){
    PickerDown();
  }

  else{
    Stepper_Move(feederArray[feeder], feeder);
    
  }
  previousFeeder = feeder;
}

void moveToPositionWithoutPick(int feeder){
  PickerUp();
  Stepper_Move(feederArray[feeder], feeder);
  previousFeeder = feeder;
}


