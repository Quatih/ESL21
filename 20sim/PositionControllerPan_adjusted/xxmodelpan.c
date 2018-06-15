/**********************************************************
 * This file is generated by 20-sim ANSI-C Code Generator
 *
 *  file:  xxmodel.c
 *  model: Jiwy
 *  expmt: Jiwy
 *  date:  June 7, 2018
 *  time:  2:05:31 PM
 *  user:  Universiteit Twente
 *  from:  20-sim 4.7 Campus License
 *  build: 4.7.0.8692
 **********************************************************/

/* This file contains the actual model variables and equations */

/* Note: Alias variables are the result of full optimization
   of the model in 20-sim. As a result, only the real variables
   are used in the model for speed. The user may also include
   the alias variables by adding them to the end of the array:

   XXDouble xx_variables[NUMBER_VARIABLES + NUMBER_ALIAS_VARIABLES + 1];
   XXString xx_variable_names[] = {
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
#include "xxmodelpan.h"
#include "xxfuncspan.h"

/* the global variables */
XXDouble xx_start_timepan = 0.0;
XXDouble xx_finish_timepan = 20.0;
XXDouble xx_step_sizepan = 0.01;
XXDouble xx_timepan = 0.0;
XXInteger xx_stepspan = 0;
XXBoolean xx_initializepan = XXTRUE;
XXBoolean xx_majorpan = XXTRUE;
XXBoolean xx_stop_simulationpan = XXFALSE;

/* the variable arrays */
XXDouble xx_Ppan[xx_parameters_size];		/* parameters */
XXDouble xx_Ipan[xx_initialvalues_size];		/* initial values */
XXDouble xx_Vpan[xx_variables_size];		/* variables */
XXDouble xx_span[xx_states_size];		/* states */
XXDouble xx_Rpan[xx_states_size];		/* rates (or new states) */

/* the names of the variables as used in the arrays above
   uncomment this part if these names are needed
XXString xx_parameter_names[] = {
	"corrGain\\K",
	"PID1\\kp",
	"PID1\\tauD",
	"PID1\\beta",
	"PID1\\tauI",
	"SignalLimiter2\\minimum",
	"SignalLimiter2\\maximum"
,	NULL
};
XXString xx_initial_value_names[] = {
	"PID1\\uD_previous_initial",
	"PID1\\error_previous_initial",
	"PID1\\uI_previous_initial"
,	NULL
};
XXString xx_variable_names[] = {
	"corrGain\\corr",
	"PID1\\output",
	"",
	"PlusMinus1\\plus1",
	"SignalLimiter2\\output",
	"Splitter1\\input",
	"corr",
	"in",
	"position",
	"out"
,	NULL
};
XXString xx_state_names[] = {
	"PID1\\uD_previous",
	"PID1\\error_previous",
	"PID1\\uI_previous"
,	NULL
};
XXString xx_rate_names[] = {
	"",
	"PID1\\error",
	""
,	NULL
};
*/

#if (7 > 8192) && defined _MSC_VER
#pragma optimize("", off)
#endif
void XXModelInitialize_parameterspan(void)
{
	/* set the parameters */
	xx_Ppan[0] = 0.0;		/* corrGain\K */
	xx_Ppan[1] = 2.6;		/* PID1\kp */
	xx_Ppan[2] = 0.05;		/* PID1\tauD */
	xx_Ppan[3] = 0.17;		/* PID1\beta */
	xx_Ppan[4] = 9.0;		/* PID1\tauI */
	xx_Ppan[5] = -0.9899999999999996;		/* SignalLimiter2\minimum */
	xx_Ppan[6] = 0.9899999999999996;		/* SignalLimiter2\maximum */

}
#if (7 > 8192) && defined _MSC_VER
#pragma optimize("", on)
#endif

void XXModelInitialize_initialvaluespan(void)
{
	/* set the initial values */
	xx_Ipan[0] = 0.0;		/* PID1\uD_previous_initial */
	xx_Ipan[1] = 0.0;		/* PID1\error_previous_initial */
	xx_Ipan[2] = 0.0;		/* PID1\uI_previous_initial */

}

void XXModelInitialize_statespan(void)
{
	/* set the states */
	xx_span[0] = xx_Ipan[0];		/* PID1\uD_previous */
	xx_span[1] = xx_Ipan[1];		/* PID1\error_previous */
	xx_span[2] = xx_Ipan[2];		/* PID1\uI_previous */

}

void XXModelInitialize_variablespan(void)
{
	/* initialize the variable memory to zero */
	memset(xx_Vpan, 0, xx_variables_size * sizeof(XXDouble));
}

/* this method is called before calculation is possible */
void XXModelInitializepan (void)
{
	XXModelInitialize_parameterspan();
	XXModelInitialize_variablespan();
	XXModelInitialize_initialvaluespan();
	XXModelInitialize_statespan();
}

/* This function calculates the initial equations of the model.
 * These equations are calculated before anything else
 */
void XXCalculateInitialpan (void)
{

}

/* This function calculates the static equations of the model.
 * These equations are only dependent from parameters and constants
 */
void XXCalculateStaticpan (void)
{

}

/* This function calculates the input equations of the model.
 * These equations are dynamic equations that must not change
 * in calls from the integration method (like random and delay).
 */
void XXCalculateInputpan (void)
{

}

/* This function calculates the dynamic equations of the model.
 * These equations are called from the integration method
 * to calculate the new model rates (that are then integrated).
 */
void XXCalculateDynamicpan (void)
{
	/* PID1\factor = 1 / (sampletime + PID1\tauD * PID1\beta); */
	xx_V[2] = 1.0 / (xx_step_size + xx_P[2] * xx_P[3]);

	/* PlusMinus1\plus1 = in; */
	xx_V[3] = xx_V[7];

	/* Splitter1\input = position; */
	xx_V[5] = xx_V[8];

	/* corrGain\corr = corrGain\K * Splitter1\input; */
	xx_V[0] = xx_P[0] * xx_V[5];

	/* PID1\error = PlusMinus1\plus1 - Splitter1\input; */
	xx_R[1] = xx_V[3] - xx_V[5];

	/* PID1\uD = PID1\factor * (((PID1\tauD * PID1\uD_previous) * PID1\beta + (PID1\tauD * PID1\kp) * (PID1\error - PID1\error_previous)) + (sampletime * PID1\kp) * PID1\error); */
	xx_R[0] = xx_V[2] * (((xx_P[2] * xx_s[0]) * xx_P[3] + (xx_P[2] * xx_P[1]) * (xx_R[1] - xx_s[1])) + (xx_step_size * xx_P[1]) * xx_R[1]);

	/* PID1\uI = PID1\uI_previous + (sampletime * PID1\uD) / PID1\tauI; */
	xx_R[2] = xx_s[2] + (xx_step_size * xx_R[0]) / xx_P[4];

	/* PID1\output = PID1\uI + PID1\uD; */
	xx_V[1] = xx_R[2] + xx_R[0];

	/* SignalLimiter2\output = (if PID1\output < SignalLimiter2\minimum then SignalLimiter2\minimum else (if PID1\output > SignalLimiter2\maximum then SignalLimiter2\maximum else PID1\output end) end); */
	xx_V[4] = ((xx_V[1] < xx_P[5]) ? 
		/* SignalLimiter2\minimum */
		xx_P[5]
	:
		/* (if PID1\output > SignalLimiter2\maximum then SignalLimiter2\maximum else PID1\output end) */
		((xx_V[1] > xx_P[6]) ? 
			/* SignalLimiter2\maximum */
			xx_P[6]
		:
			/* PID1\output */
			xx_V[1]
		)
	);


	/* increment the step counter */
	xx_steps++;
}

/* This function calculates the output equations of the model.
 * These equations are not needed for calculation of the rates
 * and are kept separate to make the dynamic set of equations smaller.
 * These dynamic equations are called often more than one time for each
 * integration step that is taken. This makes model computation much faster.
 */
void XXCalculateOutput (void)
{
	/* corr = corrGain\corr; */
	xx_V[6] = xx_V[0];

	/* out = SignalLimiter2\output; */
	xx_V[9] = xx_V[4];

}

/* This function calculates the final equations of the model.
 * These equations are calculated after all the calculations
 * are performed
 */
void XXCalculateFinal (void)
{

}

/* this method is called after all calculations are performed */
void XXModelTerminate(void)
{
}

