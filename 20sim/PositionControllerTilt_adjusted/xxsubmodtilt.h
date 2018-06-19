/**********************************************************
 * This file is generated by 20-sim ANSI-C Code Generator
 *
 *  file:  xxsubmod.h
 *  subm:  PositionControllerTilt
 *  model: Jiwy
 *  expmt: Jiwy
 *  date:  June 7, 2018
 *  time:  2:05:49 PM
 *  user:  Universiteit Twente
 *  from:  20-sim 4.7 Campus License
 *  build: 4.7.0.8692
 **********************************************************/

/* This file describes the model functions
   that are supplied for computation.

   The model itself is the xxmodel.c file
*/

#ifndef XX_SUBMOD_H
#define XX_SUBMOD_H

/* Our own include files */
#include "xxmodel.h"

/* Submodel I/O variables */
extern XXInteger xx_number_of_inputs;
extern XXInteger xx_number_of_outputs;

/* The names of the variables as used in the arrays above */
extern XXString xx_input_names[];
extern XXString xx_output_names[];


/* The submodel functions */
void XXInitializeSubmodel (XXDouble *u, XXDouble *y, XXDouble t);
void XXCalculateSubmodel (XXDouble *u, XXDouble *y, XXDouble t);
void XXTerminateSubmodel (XXDouble *u, XXDouble *y, XXDouble t);
void XXCopyInputsToVariables (XXDouble *u);
void XXCopyVariablesToOutputs (XXDouble *y);

#endif
