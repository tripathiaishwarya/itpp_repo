/*----------------------------------------------------------------------------------------------------------------
Code for Root raised cosine, get pulse shape
Aishwarya Tripathi
June 10,2015
----------------------------------------------------------------------------------------------------------------*/


/*I/O format of the code for scilab engine
* 
* <outputPulseShape> = itpp_get_pulse_shape(double roll_off_factor, int filter_length, int upsampling_factor)
*
* The code interfaces the function Root_Raised_Cosine<double>(roll_off_factor, filter_length, upsampling_factor).get_pulse_shape() of ITPP toolbox with the scilab engine.
*
* ITPP toolbox function description : 
*-----------------------------------
* The function get_pulse_shape() gives the shape of the pulse which will be further used in upsampling and shaping of the input signal. It 
* uses roll off factor, filter length and upsampling factor to generate the pulse
* 
*
* Input/Output Details : 
*----------------------
* It is expected that the user inputs roll off factor, filter length and upsampling factor in scilab engine while calling the
* itpp_get_pulse_shape() function and expect the function to return pulse shape vector as output in scilab console.
*/

/***********Sample I/O in scilab console******************
-->b=0.5
 b  =
 
    0.5  
 
-->c=6
 c  =
 
    6.  
 
-->d=8
 d  =
 
    8.  
 
-->a=itpp_get_pulse_shape(b,c,d)
 a  =
 
 
         column 1 to 5
 
    0.0012376  - 0.0044831  - 0.0079323  - 0.0061259    0.0013402  
 
         column  6 to 10
 
    0.0112106    0.0173266    0.0132935  - 0.0038556  - 0.0306294  
 
         column 11 to 15
 
  - 0.0563406  - 0.0655574  - 0.0433165    0.0186471    0.1171152  
 
         column 16 to 20
 
    0.2362257    0.3508636    0.4337738    0.4640231    0.4337738  
 
         column 21 to 25
 
    0.3508636    0.2362257    0.1171152    0.0186471  - 0.0433165  
 
         column 26 to 30
 
  - 0.0655574  - 0.0563406  - 0.0306294  - 0.0038556    0.0132935  
 
         column 31 to 35
 
    0.0173266    0.0112106    0.0013402  - 0.0061259  - 0.0079323  
 
         column 36 to 37
 
  - 0.0044831    0.0012376  
 
*/


#include <itpp/comm/pulse_shape.h>
#include <itpp/base/vec.h>


using namespace itpp;


extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include <localization.h>


/*=================================================================*/

int itpp_get_pulse_shape(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;


	//Variable declaration
	int m1=0,n1=0,m2=0,n2=0,m3=0,n3=0,row=1,col=1,i=0; 
	int *piAddressVar1 = NULL;
	int *piAddressVar2 = NULL;
	int *piAddressVar3 = NULL;
	double *roll_off_factor = NULL;
	double *filter_length= NULL;
	double *upsampling_factor = NULL;
	double *matrixOfPulseShape = NULL;
		
	
	//-----------------------Checking the no. of output and input arguements----------------------------
	/*c=foo(a,b)
	  Input arguements=3
	  Output arguements=1*/
	  CheckInputArgument(pvApiCtx, 3, 3) ;
    	  CheckOutputArgument(pvApiCtx, 1, 1) ;

	
	//-------------------------For First Input Arguement (Roll Off factor)----------------------------- 
	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVar1);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}	
	

	//Checking if input signal is of boolean type
	if ( !isDoubleType(pvApiCtx, piAddressVar1) ||  isVarComplex(pvApiCtx, piAddressVar1) )
    	{
        	Scierror(999, _("%s: Wrong type for input argument #%d: A boolean matrix expected.\n"), fname, 1);
        	return 0;
    	}


    	//Getting input signal
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar1, &m1, &n1, &roll_off_factor);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}


	//Checking if roll_off_factor is not zero
	if(*roll_off_factor == 0)
	{
		Scierror(999, _("%s: Non-zero roll off factor expected.\n"), fname, 1);
		return 0;
	}


	//Checking input signal is a vector
	if( ( m1 != 1 ) || ( n1 != 1) ) 
	{
		Scierror(999, _("%s: Single input expected.\n"), fname, 1);
		return 0;
	}


	//------------------------------For second arguement(filter length)-------------------------------

	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVar2);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}	


	//Checking if input signal is of boolean type
	if ( !isDoubleType(pvApiCtx, piAddressVar2) ||  isVarComplex(pvApiCtx, piAddressVar2) )
    	{
        	Scierror(999, _("%s: Wrong type for input argument #%d: An integer matrix expected.\n"), fname, 1);
        	return 0;
    	}


	//Retrieve dimensions and data
	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar2, &m2, &n2, &filter_length);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}


	//Checking if filter length is not an integer
	if((*filter_length - floor(*filter_length)) != 0)
	{
		Scierror(999, _("%s: Integer input expected.\n"), fname, 1);
		return 0;
	}


	//Checking for a single integer input 
	if( ( m2 != 1 ) || ( n2 != 1) ) 
	{
		Scierror(999, _("%s: Single integer input expected.\n"), fname, 1);
		return 0;
	}


	//------------------------------For third arguement(upsampling factor)-------------------------------

	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVar3);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}	


	//Checking if input signal is of boolean type
	if ( !isDoubleType(pvApiCtx, piAddressVar3) ||  isVarComplex(pvApiCtx, piAddressVar3) )
    	{
        	Scierror(999, _("%s: Wrong type for input argument #%d: An integer matrix expected.\n"), fname, 1);
        	return 0;
    	}


	//Retrieve dimensions and data
	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar3, &m3, &n3, &upsampling_factor);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}


	//Checking if upsampling factor is not an integer
	if((*upsampling_factor - floor(*upsampling_factor)) != 0)
	{
		Scierror(999, _("%s: Integer input expected.\n"), fname, 1);
		return 0;
	}


	//Checking for a single integer input 
	if( ( m3!= 1 ) || ( n3 != 1) ) 
	{
		Scierror(999, _("%s: Single integer input expected.\n"), fname, 1);
		return 0;
	}

		
	//-------------------------------Implementation of get_pulse_shape------------------------------
	//Vectors for storing output
	vec outputVector;


	//Invoking get_pulse_shape
	outputVector = Root_Raised_Cosine<double>(*roll_off_factor, (int) *filter_length, (int) *upsampling_factor ).get_pulse_shape(); 


	//----------------------------------------Output Arguements---------------------------------------
	//Defining the no. of col in matrix to be sent as output
	col = outputVector.length();	


	//Storing the output in a double array to be sent to scilab
	matrixOfPulseShape = (double *) malloc( sizeof(double)*outputVector.length());
	for(i=0; i < outputVector.length(); i++)
	{
		matrixOfPulseShape[i] = outputVector(i);
	}


	//Creating Output Pulse Shape
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, row, col, matrixOfPulseShape);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}
	free(matrixOfPulseShape);


	//-------------------------------Returning output arguements to Scilab----------------------------

	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	ReturnArguments(pvApiCtx);
	return 0;
}
/*===============================================================================*/
}
