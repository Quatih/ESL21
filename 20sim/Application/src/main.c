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
#include <stdbool.h>

#define pan_max 1110.0
#define tilt_max pan_max //309.0
#define DEBUG

// GPMC file descriptor
int fd; 
int32_t encpan, enctilt;

#ifndef DEBUG
#define getPan() getGPMCValue(fd, 2)
#define getTilt() getGPMCValue(fd, 0)
#define setPan(val) setGPMCValue(fd, val, 6)
#define setTilt(val) setGPMCValue(fd, val, 4)


#define setPanIn(encval) upan[0] = ConvertRad(encval, pan_max)
#define setPanPos(pos) upan[1] = ConvertRad(pos, pan_max)

#define setTiltIn(encval) utilt[1] = ConvertRad(encval, tilt_max)
#define setTiltPos(pos) utilt[2] = ConvertRad(pos, tilt_max)

#else

#define getPan() encpan++
#define getTilt() enctilt++
#define setPan(val) 
#define setTilt(val) 


#define setPanIn(encval) upan[0] = ConvertRad(encval, pan_max)
#define setPanPos(pos) upan[1] = ConvertRad(pos, pan_max)

#define setTiltIn(encval) utilt[1] = ConvertRad(encval, tilt_max)
#define setTiltPos(pos) utilt[2] = ConvertRad(pos, tilt_max)

#endif

// returns the current run-time, hopefully

/* The pan encoder spans 2000 values which correspond to approximately Pi radians. */
double ConvertRad(int32_t val, double max)
{
  return ((double)val/max) * M_PI;
}

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
  if(val < 0)
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
  #ifndef DEBUG
    setGPMCValue(fd, 1, 7);
  #endif
}

void move2end(){ 
  #ifndef DEBUG
  reset();

  printf("moving to end\n");
  printf("%7d, %7d\r\n", getTilt(), getPan());
  int32_t pan = getPan(), tilt = getTilt();
  int32_t lp = 3000, lt = 3000; // defined out of range of encoders
  setPan(ConvertPWM(0.5));
  setTilt(ConvertPWM(0.5));
  bool tf = true, pf = true;
  printf("%7d, %7d\r\n", tilt -lt, pan - lp);
  while(tf || pf){ // terminate when both have gone to the end
    usleep(100000);
    lp = pan;
    lt = tilt;

    pan = getPan();
    tilt = getTilt();
    if(( ((tilt - lt) <= 2) && ((tilt-lt) >= -2)) ){
	setTilt(ConvertPWM(0.0));
printf("tilt stop\n");
	pf = false;
    }
    if((((pan - lp) <= 5) && ((pan-lp) >= -5))){
	setPan(ConvertPWM(0.0));
	printf("pan stop\n");
	tf = false;
    }
    printf("%7d, %7d\r\n", tilt -lt, pan - lp);

  }
  setTilt(ConvertPWM(0.0));
  setPan(ConvertPWM(0.0));
  reset();
printf("ending\r\n");
#endif
}

/* The main function */
int main(int argc, char* argv[])
{
  uint32_t Mpan = 0, Mtilt = 0;
  XXDouble upan [2 + 1];
  XXDouble ypan [2 + 1];
  XXDouble utilt [3 + 1];
  XXDouble ytilt [1 + 1];

  #ifndef DEBUG
  // open connection to device.
  printf("Opening gpmc_fpga...\n");
  fd = open("/dev/gpmc_fpga", 0);
  if (0 > fd)
  {
  printf("Error, could not open device: %s.\n", argv[1]);
  return 1;
  }
  #endif
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

  /*int a, b;
  while(1){
    a = getGPMCValue(fd, 0);
    b = getGPMCValue(fd, 2);
    setGPMCValue(fd,0,6);
    //printf("Radians: %f %f\n", ConvertRad(a), ConvertRad(b)); 
    printf("RAW: %7d %7d\r", a,b); 
    usleep(1000);
    //if(b >100 || b < -100) {
   //   setGPMCValue(fd, 1, 7);
    //}
  }*/
  move2end();
  //usleep(2000000);
  reset();
  // set the target to the middle value of the range of motion
  setPanPos(100);
  setTiltPos(100);
  printf("positions[rad]: %f, %f", upan[1], utilt[2]);
  // clock_t lastclk = clock()
  // clock_t clockdiff;
  // clock_t clk;
  
  
  /* Initialize the submodel itself */
  XXInitializeSubmodelpan (upan, ypan, curr_time);
  /* Initialize the submodel itself */
  XXInitializeSubmodeltilt (utilt, ytilt, curr_time);


printf("%d %d\n", getPan(), getTilt());

  struct timeval time;
  
  time.tv_sec = 0;
  time.tv_usec = 0;
  gettimeofday(&time, NULL);
  long timenow = time.tv_usec; 
  long dur;
  double diff;
  bool panpos = true, tiltpos = true;
  while (panpos && tiltpos)
  {
    gettimeofday(&time, NULL);  
    //printf("%lu, %lu, %lu\n", time.tv_sec, time.tv_usec, time.tv_usec - timenow);
    if (time.tv_usec < timenow){
	    timenow = time.tv_usec;
    }
    else if(time.tv_usec - timenow >= 10000){
	
    dur = time.tv_usec - timenow;
	//printf("%f, %lu, %lu, %lu\n", (double)dur/1000000, dur, time.tv_sec, time.tv_usec);
    // clockdiff = clk - lastclk;
    // if (clockdiff >= (clock_t)((double)CLOCKS_PER_SEC*0.01)){
    //Get and convert the decoder readings. 
    setPanIn(getPan()); 		
    setTiltIn(getTilt()); 

    /* Call the submodel to calculate the output */
    XXCalculateSubmodelpan (upan, ypan, (double)dur/1000000);
    XXCalculateSubmodeltilt (utilt, ytilt, (double)dur/1000000);

    //Convert, check and send the Motor steering values

    diff = upan[1] - utilt[0];
    
    if((diff <= 0.05) && (diff >= -0.05)){
      printf("pan position met\n");
      setPan(0);
      panpos = false;
    }
    else if (panpos) {     
      Mpan = ConvertPWM(ypan[1]);
      setPan(Mpan);
    }

    diff = utilt[2] - utilt[1];
    
    if((diff <= 0.05) && (diff >= -0.05)){
      printf("tilt position met\n");
      setTilt(0);
      tiltpos = false;
    }
    else if (tiltpos) {
      Mtilt = ConvertPWM(ytilt[0]);
      setTilt(Mtilt);
    }
    //printf("err: %7f, %7f\r", utilt[1] - utilt[2], ytilt[0]);
    
    printf("Timestep: %f, %f, %f, %f, %f, %d, %d\n", xx_step_sizepan, upan[0], utilt[1], ypan[1], ytilt[0], Mpan,Mtilt);

    timenow = time.tv_usec; 
    }
    // lastclk = clk;
  }
  printf("%d %d\n", getPan(), getTilt());
  
  /* Perform the final calculations */
  XXTerminateSubmodelpan (upan, ypan, 0);
  XXTerminateSubmodeltilt (utilt, ytilt, 0);
  #ifndef DEBUG
  close(fd);
  #endif
  return 0;
}

