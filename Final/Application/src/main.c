/**********************************************************
 * This file is generated by 20-sim ANSI-C Code Generator
 *
 *  file:  xxmain.c
 *  subm:  PositionControllerPan
 *  model: Jiwy
 *  expmt: Jiwy
 *  date:  June 7, 2018
 *  time:  2:05:31 PM
 *  user:  Universiteit Twente
 *  from:  20-sim 4.7 Campus License
 *  build: 4.7.0.8692
 **********************************************************/

/* This file is a demo application of how the submodel function can 
 * be used. It uses the global time variables that are used by both
 * the submodel and the integration method.
 *
 * PLEASE NOTE: THIS IS AN EXAMPLE WHERE ALL INPUTS ARE ZERO ! 
 * USE YOUR OWN INPUTS INSTEAD!! ALSO THE SUBMODEL MIGHT SIMPLY 
 * NOT WORK CORRECTLY WITH INPUTS THAT ARE ZERO.
 */


#ifdef __cplusplus
extern "C" {
#endif
/* 20-sim include files */
#include "xxsubmodpan.h"
#include "xxsubmodtilt.h"
#include "gpmc_driver_c.h"

#ifdef __cplusplus
}
#endif

#include <stdio.h>
#include <math.h>
#include <fcntl.h>      // open()
#include <unistd.h>     // close()
#include <stdint.h>
#include <stdlib.h>
#include <time.h> // clock()

#define pan_max 1110.0
#define tilt_max 309.0

// GPMC file descriptor
int fd; 

#define getPan() getGPMCValue(fd, 2)
#define getTilt() getGPMCValue(fd, 0)
#define setPan(val) setGPMCValue(fd, val, 6)
#define setTilt(val) setGPMCValue(fd, val, 4)

// returns the current run-time, hopefully
#define curr_time (double) clock()/CLOCKS_PER_SEC

struct modelvars {
  XXDouble upan [2 + 1];
  XXDouble ypan [2 + 1];
  XXDouble utilt [3 + 1];
  XXDouble ytilt [1 + 1];
}
/* The pan encoder spans 2000 values which correspond to approximately Pi radians. */
double ConvertRad(int32_t val, double max)
{
  return ((double)val/max) * M_PI;
}

#define setPanIn(encval) upan[0] = ConvertRad(encval, pan_max)
void setPanPos(struct modelvars * model, double pos){
  model.upan[1] = ConvertRad(pos, pan_max);
} 
#define setTiltIn(encval) utilt[1] = ConvertRad(encval, pan_max)
#define setTiltPos(pos) utilt[2] = ConvertRad(pos, tilt_max)


/* According to 20SIM, the output is a signal between -1.0 and 1.0. 
This output must be converted to a PWM output between 0 and 250 
and a direction according to a truthtable. */
uint32_t ConvertPWM(double val)
{
  //Brake
  if(val == 0)
    return 0;
  
  uint32_t ret_val = 0;
  double ch = abs(val * 250.0);
  //printf("%f, %f\n", val, ch);
  ret_val = (uint32_t) (ch);
  //Clockwise
  if(val > 0)
  {
    //INA = 1;
    //INB = 0;
    ret_val |= 0x01 << 8;
  }
  //Counterclockwise
  else
  {
    //INA = 0;
    //INB = 1;
    ret_val |= 0x02 << 8;
  }
  return ret_val;
}

void reset() {
    setGPMCValue(fd, 1, 7);
}

void move2end(){ 
  reset();
  printf("moving to end\n");
  int32_t pan = getPan(), tilt = getTilt();
  int32_t lp = 3000, lt = 3000; // defined out of range of encoders
  setPan(ConvertPWM(-0.5));
  setTilt(ConvertPWM(-0.5));

  while(tilt != lt && pan != lp){ // terminate when both have gone to the end
    usleep(2000);
    lp = pan;
    lt = tilt;

    pan = getPan();
    tilt = getTilt();

  }
  setTilt(0);
  setPan(0);
  reset(fd);
}

void stop(){
  setPan(0);
  setTilt(0);
}

modelvars initializeModel(){
  XXDouble upan [2 + 1];
  XXDouble ypan [2 + 1];
  XXDouble utilt [3 + 1];
  XXDouble ytilt [1 + 1];

  // open connection to device.
  printf("Opening gpmc_fpga...\n");
  fd = open("/dev/gpmc_fpga", 0);
  if (0 > fd)
  {
  printf("Error, could not open device: %s.\n", argv[1]);
  return 1;
  }

  /* Initialize the inputs and outputs with correct initial values */
  upan[0] = 0.0;		/* in */
  upan[1] = 0.0;		/* position */

  ypan[0] = 0.0;		/* corr */
  ypan[1] = 0.0;		/* out */

/* Initialize the inputs and outputs with correct initial values */
  utilt[0] = 0.0;		/* corr */
  utilt[1] = 0.0;		/* in */
  utilt[2] = 0.0;		/* position */

  ytilt[0] = 0.0;		/* out */

  move2end();
  struct modelvars ret = {upan, ypan, utilt, ytilt};
  return ret; 
}

/* The main function */
int main(int argc, char* argv[])
{
  uint32_t Mpan = 0, Mtilt = 0;
  struct modelvars model = initializeModel()

  reset();
  move2end();
  
  // set the target to the middle value of the range of motion
  setPanPos(struct modelvars * model, pan_max/2);
  setTiltPos(tilt_max/2);
  
  // clock_t lastclk = clock()
  // clock_t clockdiff;
  // clock_t clk;
  
  
  /* Initialize the submodel itself */
  XXInitializeSubmodelpan (upan, ypan, curr_time);
  /* Initialize the submodel itself */
  XXInitializeSubmodeltilt (utilt, ytilt, curr_time);

  while (1)
  {
    // clockdiff = clk - lastclk;
    // if (clockdiff >= (clock_t)((double)CLOCKS_PER_SEC*0.01)){
    //Get and convert the decoder readings. 
    setPanIn(getPan()); 		
    setTiltIn(getTilt()); 

    /* Call the submodel to calculate the output */
    XXCalculateSubmodelpan (upan, ypan, curr_time);
    XXCalculateSubmodeltilt (utilt, ytilt, curr_time);

    //Convert, check and send the Motor steering values
    Mpan = ConvertPWM(ypan[1]);
    Mtilt = ConvertPWM(ytilt[0]);
    setPan(Mpan);		
    setTilt(Mtilt);
    if(abs(utilt[2] - utilt[1]) <= 0.05 && abs(upan[1] - upan[0]) <= 0.05){
      printf("position met");
      setPan(0);
      setTilt(0);
      break;
    }
    //printf("err: %7f, %7f\r", utilt[1] - utilt[2], ytilt[0]);
    //printf("Timestep: %f, %f, %f, %f, %f, %d, %d\n", xx_timepan, upan[1], utilt[2], ypan[1], ytilt[0], Mpan,Mtilt);
    //usleep(1000);
  
    // lastclk = clk;
  }

  /* Perform the final calculations */
  XXTerminateSubmodelpan (upan, ypan, curr_time);
  XXTerminateSubmodeltilt (utilt, ytilt, curr_time);
  close(fd);

  return 0;
}
