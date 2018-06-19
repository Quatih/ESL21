/**********************************************************
 * This file is generated by 20-sim C++ Code Generator
 *
 *  file:  test_IO_Tilt.cpp
 *  subm:  IO_Tilt
 *  model: JIWY-better-tuned
 *  expmt: JIWY-better-tuned
 *  date:  June 19, 2018
 *  time:  2:04:40 PM
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

#include <stdio.h>

/* 20-sim submodel class include file */
#include "IO_Tilt.h"

/* the main function */
int main()
{
	XXDouble u [2 + 1];
	XXDouble y [2 + 1];

	/* initialize the inputs and outputs with correct initial values */
	u[0] = 0.0;		/* angleCT */
	u[1] = 0.0;		/* steerDT */

	y[0] = 0.0;		/* angleDT */
	y[1] = 0.0;		/* steerCT */


	IO_Tilt my20simSubmodel;

	/* initialize the submodel itself and calculate the outputs for t=0.0 */
	my20simSubmodel.Initialize(u, y, 0.0);
	printf("Time: %f\n", my20simSubmodel.GetTime() );

	/* simple loop, the time is incremented by the integration method */
	while (my20simSubmodel.state != IO_Tilt::finished)
	{
		/* call the submodel to calculate the output */
		my20simSubmodel.Calculate (u, y);
		printf("Time: %f\n", my20simSubmodel.GetTime() );
	}

	/* perform the final calculations */
	my20simSubmodel.Terminate (u, y);

	/* and we are done */
	return 0;
}

