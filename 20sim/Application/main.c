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

/* 20-sim include files */
#include "xxsubmodpan.h"
#include "xxsubmodtilt.h"
#include "stdio.h"
#include "gpmc_driver_c.h"

#include <fcntl.h>      // open()
#include <unistd.h>     // close()



/* The pan encoder spans 1110 values which correspond to approximately Pi radians. */
double ConvertRad(uint32_t val)
{
	return (val/1110) * M_PI;
}

/* According to 20SIM, the output is a signal between -20 and 20. 
This output must be converted to a PWM output between 0 and 250 
and a direction according to a truthtable. */
uint32_t ConvertPWM(double val)
{
	//Brake
	if(val == 0)
		return 0;
	
	uint32_t ret_val = 0;
	ret_val = (abs(val/20) * 250;
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
}

uint32_t CheckOutput(uint32_t val)
{
	
	
}

/* The main function */
int main()
{
	uint32_t Mpan = 0, Mtilt = 0;
	XXDouble upan [2 + 1];
	XXDouble ypan [2 + 1];
	XXDouble utilt [3 + 1];
	XXDouble ytilt [1 + 1];

	int fd; // File descriptor.
	if (2 != argc)
	{
	printf("Usage: %s <device_name>\n", argv[0]);
	return 1;
	}

	printf("GPMC driver c-example\n");

	// open connection to device.
	printf("Opening gpmc_fpga...\n");
	fd = open(argv[1], 0);
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


	/* Initialize the submodel itself */
	XXInitializeSubmodelpan (upan, ypan, xx_timepan);


/* Initialize the inputs and outputs with correct initial values */
	utilt[0] = 0.0;		/* corr */
	utilt[1] = 0.0;		/* in */
	utilt[2] = 0.0;		/* position */

	ytilt[0] = 0.0;		/* out */


	/* Initialize the submodel itself */
	XXInitializeSubmodeltilt (utilt, ytilt, xx_timetilt);

	/* Simple loop, the time is incremented by the integration method */
	while ( (xx_timepan < xx_finish_timepan) && (xx_stop_simulationpan == XXFALSE) )
	{

		//Get and convert the decoder readings. 
		uPan[0] = ConvertRad(getGPMCValue(fd, 0)); 		
		uTilt[0] = ConvertRad(getGPMCValue(fd, 2)); 

		/* Call the submodel to calculate the output */
		XXCalculateSubmodelpan (upan, ypan, xx_timepan);
		XXCalculateSubmodeltilt (utilt, ytilt, xx_timetilt);


		//Convert, check and send the Motor steering values
		Mpan = CheckOutput(ConvertPWM(yPan[1]));
		Mtilt = CheckOutput(ConvertPWM(yTilt[1]));
		setGPMCValue(fd, M1, 4);		
		setGPMCValue(fd, M2, 6);

	}

	/* Perform the final calculations */
	XXTerminateSubmodelpan (upan, ypan, xx_timepan);
	XXTerminateSubmodeltilt (utilt, ytilt, xx_timetilt);


	/* and we are done */
	return 0;
}
