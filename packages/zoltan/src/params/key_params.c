/*****************************************************************************
 * Zoltan Dynamic Load-Balancing Library for Parallel Applications           *
 * Copyright (c) 2000, Sandia National Laboratories.                         *
 * For more info, see the README file in the top-level Zoltan directory.     *  
 *****************************************************************************/
/*****************************************************************************
 * CVS File Information :
 *    $RCSfile$
 *    $Author$
 *    $Date$
 *    $Revision$
 ****************************************************************************/

#include <stdio.h>
#include "lb_const.h"
#include "params_const.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
static PARAM_VARS Key_params[] = {
	{ "IMBALANCE_TOL", NULL, "DOUBLE" },
	{ "AUTO_MIGRATE", NULL, "INT" },
	{ "OBJ_WEIGHT_DIM", NULL, "INT" },
	{ "COMM_WEIGHT_DIM", NULL, "INT" },
	{ "DEBUG_LEVEL", NULL, "INT" },
	{ "DEBUG_PROCESSOR", NULL, "INT" },
	{ "DETERMINISTIC", NULL, "INT" },
	{ "TIMER", NULL, "STRING" },
	{ NULL, NULL, NULL } };
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 
 * Handle parameter changes for variables stored in LB structure.
 */

int LB_Set_Key_Param(
LB *lb,                         /* load balance structure */
char *name,			/* name of variable */
char *val)			/* value of variable */
{
    int status;			/* return code */
    PARAM_UTYPE result;		/* value returned from Check_Param */
    int index;			/* index returned from Check_Param */
    int tmp;

    status = LB_Check_Param(name, val, Key_params, &result, &index);

    if (status == 0) {

      switch (index) {

      case 0:  		/* Imbalance_Tol */
        if (result.def) 
            result.dval = LB_IMBALANCE_TOL_DEF;
	if (result.dval < 1.0) {
	    fprintf(stderr, "WARNING: Invalid Imbalance_Tol value (%g) "
		"being set to %g\n", result.dval, LB_IMBALANCE_TOL_DEF);
	    result.dval = LB_IMBALANCE_TOL_DEF;
	}
	lb->Imbalance_Tol = result.dval;
	status = 3;		/* Don't add to Params field of LB */
        break;

      case 1:		/* Help_Migrate */
        if (result.def)
            result.ival = LB_AUTO_MIGRATE_DEF;
	lb->Migrate.Auto_Migrate = result.ival;
	status = 3;		/* Don't add to Params field of LB */
        break;

      case 2:		/* Object weight dim.  */
        if (result.def)
            result.ival = LB_OBJ_WEIGHT_DEF;
	if (result.ival < 0) {
	    fprintf(stderr, "WARNING: Invalid Obj_Weight_Dim value (%d) "
		"being set to 0\n", result.ival);
	    result.ival = 0;
	}
	lb->Obj_Weight_Dim = result.ival;
	status = 3;		/* Don't add to Params field of LB */
        break;

      case 3:		/* Communication weight dim.  */
        if (result.def)
            result.ival = LB_COMM_WEIGHT_DEF;
	if (result.ival < 0) {
	    fprintf(stderr, "WARNING: Invalid Comm_Weight_Dim value (%d) "
		"being set to 0\n", result.ival);
	    result.ival = 0;
	}
	lb->Comm_Weight_Dim = result.ival;
	status = 3;		/* Don't add to Params field of LB */
        break;

      case 4: 		/* Debug level  */
        if (result.def)
            result.ival = LB_DEBUG_LEVEL_DEF;
	if (result.ival < 0) {
	    fprintf(stderr, "WARNING: Invalid Debug_Level value (%d) "
		"being set to 0\n", result.ival);
	    result.ival = 0;
	}
	lb->Debug_Level = result.ival;
	status = 3;		/* Don't add to Params field of LB */
        break;

      case 5: 		/* Debug processor  */
        if (result.def)
            result.ival = LB_DEBUG_PROC_DEF;
	if (result.ival < 0 || result.ival > lb->Num_Proc) {
	    fprintf(stderr, "WARNING: Invalid Debug_Processor value (%d) "
		"being set to 0\n", result.ival);
	    result.ival = 0;
	}
	lb->Debug_Proc = result.ival;
	status = 3;		/* Don't add to Params field of LB */
        break;
       
      case 6: 		/* Deterministic flag */
        if (result.def)
            result.ival = LB_DETERMINISTIC_DEF;
	if (result.ival < 0) {
	    fprintf(stderr, "WARNING: Invalid Deterministic value (%d) "
		"being set to TRUE\n", result.ival);
	    result.ival = TRUE;
	}
	lb->Deterministic = result.ival;
	status = 3;		/* Don't add to Params field of LB */
        break;

      case 7: 		/* Timer */
	status = LB_Set_Timer_Param(name, val, &tmp);
        lb->Timer = tmp;

	if (status==0) status = 3;	/* Don't add to Params field of LB */
        break;

      }  /* end switch (index) */
    }

    return(status);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*
 *  Print key parameters.
 */
void LB_Print_Key_Params(LB *lb)
{
  printf("ZOLTAN Parameter %s = %f\n", Key_params[0].name, 
         lb->Imbalance_Tol);
  printf("ZOLTAN Parameter %s = %s\n", Key_params[1].name, 
         (lb->Migrate.Auto_Migrate ? "TRUE" : "FALSE"));
  printf("ZOLTAN Parameter %s = %d\n", Key_params[2].name, 
         lb->Obj_Weight_Dim);
  printf("ZOLTAN Parameter %s = %d\n", Key_params[3].name, 
         lb->Comm_Weight_Dim);
  printf("ZOLTAN Parameter %s = %d\n", Key_params[4].name, 
         lb->Debug_Level);
  printf("ZOLTAN Parameter %s = %d\n", Key_params[5].name, 
         lb->Debug_Proc);
  printf("ZOLTAN Parameter %s = %s\n", Key_params[6].name, 
         (lb->Deterministic ? "TRUE" : "FALSE"));
  printf("ZOLTAN Parameter %s = %d ", Key_params[7].name, lb->Timer);
  if (lb->Timer == LB_TIME_WALL)
     printf("(wall)");
  else if (lb->Timer == LB_TIME_CPU)
     printf("(cpu)");
  printf("\n");
}
