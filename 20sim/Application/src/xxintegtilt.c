/**********************************************************
 * This file is generated by 20-sim ANSI-C Code Generator
 *
 *  file:  xxinteg.c
 *  subm:  PositionControllerPan
 *  model: Jiwy
 *  expmt: Jiwy
 *  date:  June 7, 2018
 *  time:  2:05:31 PM
 *  user:  Universiteit Twente
 *  from:  20-sim 4.7 Campus License
 *  build: 4.7.0.8692
 **********************************************************/

/* This file describes the integration methods
   that are supplied for computation.

   Currently the following methods are supported:
   * Euler
   * RungeKutta2
   * RungeKutta4
   but it is easy for the user to add their own
   integration methods with these two as an example.
*/

/* the system include files */
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* our own include files */
#include "xxintegtilt.h"
#include "xxmodeltilt.h"

/* global variables prototypes */
extern XXDouble xx_timetilt;
extern XXDouble xx_step_sizetilt;

#define xx_STATE_SIZE 3

/*********************************************************************
 * Discrete integration method
 *********************************************************************/

/* the initialization of the Discrete integration method */
void XXDiscreteInitializetilt (void)
{
	/* nothing to be done */
	xx_majortilt = XXTRUE;
}

/* the termination of the Discrete integration method */
void XXDiscreteTerminatetilt (void)
{
	/* nothing to be done */
}

/* the Discrete integration method itself */
void XXDiscreteSteptilt (void)
{
	XXInteger index;

	/* for each of the supplied states */
	for (index = 0; index < xx_STATE_SIZE; index++)
	{
		/* just a move of the new state */
		xx_stilt [index] = xx_Rtilt [index];
	}
	/* increment the simulation time */
	xx_timetilt += xx_step_sizetilt;

	xx_majortilt = XXTRUE;

	/* evaluate the dynamic part to calculate the new rates */
	XXCalculateDynamictilt ();
}

