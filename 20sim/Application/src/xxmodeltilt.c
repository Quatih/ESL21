/**********************************************************
 * This file is generated by 20-sim ANSI-C Code Generator
 *
 *  file:  xxmodel.c
 *  model: Jiwy
 *  expmt: Jiwy
 *  date:  June 7, 2018
 *  time:  2:05:49 PM
 *  user:  Universiteit Twente
 *  from:  20-sim 4.7 Campus License
 *  build: 4.7.0.8692
 **********************************************************/

/* This file contains the actual model variables and equations */

/* Note: Alias variables are the result of full optimization
   of the model in 20-sim. As a result, only the real variables
   are used in the model for speed. The user may also include
   the alias variables by adding them to the end of the array:

   XXDouble xx_variablestilt[NUMBER_VARIABLES + NUMBER_ALIAS_VARIABLES + 1];
   XXString xx_variable_namestilt[]tilt = {
     VARIABLE_NAMES, ALIAS_VARIABLE_NAMES, NULL
   };

   and calculate them directly after the output equations:

   void XXCalculateOutput (void)
   {
     OUTPUT_EQUATIONS
     ALIAS_EQUATIONS
   }
*/

/* system include files */
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* 20-sim include files */
#include "xxmodeltilt.h"
#include "xxfuncs.h"

/* the global variables */
XXDouble xx_start_timetilt = 0.0;
XXDouble xx_finish_timetilt = 20.0;
XXDouble xx_step_sizetilt = 0.01;
XXDouble xx_timetilt = 0.0;
XXInteger xx_stepstilt = 0;
XXBoolean xx_initializetilt = XXTRUE;
XXBoolean xx_majortilt = XXTRUE;
XXBoolean xx_stop_simulationtilt = XXFALSE;

/* the variable arrays */
XXDouble xx_Ptilt[xx_parameters_size];		/* parameters */
XXDouble xx_Itilt[xx_initialvalues_size];		/* initial values */
XXDouble xx_Vtilt[xx_variables_size];		/* variables */
XXDouble xx_stilt[xx_states_size];		/* states */
XXDouble xx_Rtilt[xx_states_size];		/* rates (or new states) */

/* the names of the variables as used in the arrays above
   uncomment this part if these names are needed
XXString xx_parameter_namestilt[]tilt = {
	"corrGain\\K",
	"PID1\\kp",
	"PID1\\tauD",
	"PID1\\beta",
	"PID1\\tauI",
	"SignalLimiter2\\minimum",
	"SignalLimiter2\\maximum"
,	NULL
};
XXString xx_initial_value_namestilt[]tilt = {
	"PID1\\uD_previous_initial",
	"PID1\\error_previous_initial",
	"PID1\\uI_previous_initial"
,	NULL
};
XXString xx_variable_namestilt[]tilt = {
	"corrGain\\input",
	"corrGain\\output",
	"PID1\\output",
	"",
	"PlusMinus1\\output",
	"PlusMinus2\\plus1",
	"PlusMinus2\\minus1",
	"SignalLimiter2\\output",
	"corr",
	"in",
	"position",
	"out"
,	NULL
};
XXString xx_state_namestilt[]tilt = {
	"PID1\\uD_previous",
	"PID1\\error_previous",
	"PID1\\uI_previous"
,	NULL
};
XXString xx_rate_namestilt[]tilt = {
	"",
	"PID1\\error",
	""
,	NULL
};
*/

#if (7 > 8192) && defined _MSC_VER
#pragma optimize("", off)
#endif
void XXModelInitialize_parameterstilt(void)
{
	/* set the parameters */
	xx_Ptilt[0] = 0.0;		/* corrGain\K */
	xx_Ptilt[1] = 1.6;		/* PID1\kp */
	xx_Ptilt[2] = 0.05;		/* PID1\tauD */
	xx_Ptilt[3] = 0.001;		/* PID1\beta */
	xx_Ptilt[4] = 10.5;		/* PID1\tauI */
	xx_Ptilt[5] = -0.9899999999999996;		/* SignalLimiter2\minimum */
	xx_Ptilt[6] = 0.9899999999999996;		/* SignalLimiter2\maximum */

}
#if (7 > 8192) && defined _MSC_VER
#pragma optimize("", on)
#endif

void XXModelInitialize_initialvaluestilt(void)
{
	/* set the initial values */
	xx_Itilt[0] = 0.0;		/* PID1\uD_previous_initial */
	xx_Itilt[1] = 0.0;		/* PID1\error_previous_initial */
	xx_Itilt[2] = 0.0;		/* PID1\uI_previous_initial */

}

void XXModelInitialize_statestilt(void)
{
	/* set the states */
	xx_stilt[0] = xx_Itilt[0];		/* PID1\uD_previous */
	xx_stilt[1] = xx_Itilt[1];		/* PID1\error_previous */
	xx_stilt[2] = xx_Itilt[2];		/* PID1\uI_previous */

}

void XXModelInitialize_variablestilt(void)
{
	/* initialize the variable memory to zero */
	memset(xx_Vtilt, 0, xx_variables_size * sizeof(XXDouble));
}

/* this method is called before calculation is possible */
void XXModelInitializetilt (void)
{
	XXModelInitialize_parameterstilt();
	XXModelInitialize_variablestilt();
	XXModelInitialize_initialvaluestilt();
	XXModelInitialize_statestilt();
}

/* This function calculates the initial equations of the model.
 * These equations are calculated before anything else
 */
void XXCalculateInitialtilt (void)
{

}

/* This function calculates the static equations of the model.
 * These equations are only dependent from parameters and constants
 */
void XXCalculateStatictilt (void)
{

}

/* This function calculates the input equations of the model.
 * These equations are dynamic equations that must not change
 * in calls from the integration method (like random and delay).
 */
void XXCalculateInputtilt (void)
{

}

/* This function calculates the dynamic equations of the model.
 * These equations are called from the integration method
 * to calculate the new model rates (that are then integrated).
 */
void XXCalculateDynamictilt (void)
{
	/* PID1\factor = 1 / (sampletime + PID1\tauD * PID1\beta); */
	xx_Vtilt[3] = 1.0 / (xx_step_sizetilt + xx_Ptilt[2] * xx_Ptilt[3]);

	/* corrGain\input = corr; */
	xx_Vtilt[0] = xx_Vtilt[8];

	/* PlusMinus2\plus1 = in; */
	xx_Vtilt[5] = xx_Vtilt[9];

	/* PlusMinus2\minus1 = position; */
	xx_Vtilt[6] = xx_Vtilt[10];

	/* corrGain\output = corrGain\K * corrGain\input; */
	xx_Vtilt[1] = xx_Ptilt[0] * xx_Vtilt[0];

	/* PID1\error = PlusMinus2\plus1 - PlusMinus2\minus1; */
	xx_Rtilt[1] = xx_Vtilt[5] - xx_Vtilt[6];

	/* PID1\uD = PID1\factor * (((PID1\tauD * PID1\uD_previous) * PID1\beta + (PID1\tauD * PID1\kp) * (PID1\error - PID1\error_previous)) + (sampletime * PID1\kp) * PID1\error); */
	xx_Rtilt[0] = xx_Vtilt[3] * (((xx_Ptilt[2] * xx_stilt[0]) * xx_Ptilt[3] + (xx_Ptilt[2] * xx_Ptilt[1]) * (xx_Rtilt[1] - xx_stilt[1])) + (xx_step_sizetilt * xx_Ptilt[1]) * xx_Rtilt[1]);

	/* PID1\uI = PID1\uI_previous + (sampletime * PID1\uD) / PID1\tauI; */
	xx_Rtilt[2] = xx_stilt[2] + (xx_step_sizetilt * xx_Rtilt[0]) / xx_Ptilt[4];

	/* PID1\output = PID1\uI + PID1\uD; */
	xx_Vtilt[2] = xx_Rtilt[2] + xx_Rtilt[0];

	/* PlusMinus1\output = corrGain\output + PID1\output; */
	xx_Vtilt[4] = xx_Vtilt[1] + xx_Vtilt[2];

	/* SignalLimiter2\output = (if PlusMinus1\output < SignalLimiter2\minimum then SignalLimiter2\minimum else (if PlusMinus1\output > SignalLimiter2\maximum then SignalLimiter2\maximum else PlusMinus1\output end) end); */
	xx_Vtilt[7] = ((xx_Vtilt[4] < xx_Ptilt[5]) ? 
		/* SignalLimiter2\minimum */
		xx_Ptilt[5]
	:
		/* (if PlusMinus1\output > SignalLimiter2\maximum then SignalLimiter2\maximum else PlusMinus1\output end) */
		((xx_Vtilt[4] > xx_Ptilt[6]) ? 
			/* SignalLimiter2\maximum */
			xx_Ptilt[6]
		:
			/* PlusMinus1\output */
			xx_Vtilt[4]
		)
	);


	/* increment the step counter */
	xx_stepstilt++;
}

/* This function calculates the output equations of the model.
 * These equations are not needed for calculation of the rates
 * and are kept separate to make the dynamic set of equations smaller.
 * These dynamic equations are called often more than one time for each
 * integration step that is taken. This makes model computation much faster.
 */
void XXCalculateOutputtilt (void)
{
	/* out = SignalLimiter2\output; */
	xx_Vtilt[11] = xx_Vtilt[7];

}

/* This function calculates the final equations of the model.
 * These equations are calculated after all the calculations
 * are performed
 */
void XXCalculateFinaltilt (void)
{

}

/* this method is called after all calculations are performed */
void XXModelTerminatetilt(void)
{
}

