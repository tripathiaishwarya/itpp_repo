/*----------------------------------------------------------------------------------------------------------------
Code for BPSK Modulation
Aishwarya Tripathi
June 5,2015
----------------------------------------------------------------------------------------------------------------*/


/*I/O format of the code for scilab engine
* 
* <outputComplexVector> = itpp_psk_mod(<binaryInputBitStream>,<integer m>)
*
* The code interfaces the function PSK(int m).modulate_bits(bvec <inputVector>) of ITPP toolbox with the scilab engine.
*
* ITPP toolbox function description : 
*-----------------------------------
* The input to the modulate_bits() function is a binary vector and an integer M (should be a power of 2) defining M-ary behaviour of PSK, 
* and the output is a complex vector.
* 
*
* Input/Output Details : 
*----------------------
* It is expected that the user inputs a single dimension binary matrix of (0,1) and an integer m in scilab engine while calling the
* itpp_bpsk_mod() function and expect the function to return a single dimension complex matrix as output in scilab console.
*/

/***********Sample I/O in scilab console******************
-->a=[1 0 0 1 1 0 1 1 1]
 a  =
 
    1.    0.    0.    1.    1.    0.    1.    1.    1.  
 
-->b=8
 b  =
 
    8.  
 
-->c=itpp_psk_mod(a,b)
 c  =
 
    0.7071068 - 0.7071068i  - 1.  - 0.7071068 - 0.7071068i  
 
*/


#include <numeric>
#include <itpp/comm/pulse_shape.h>
#include <itpp/base/vec.h>
#include <itpp/itcomm.h>
#include <itpp/itbase.h>


using namespace itpp;
using namespace std;

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include "BOOL.h"
#include <localization.h>
#include "MALLOC.h"
#include <itpp/base/vec.h>

/*=================================================================*/

int itpp_shape_symbols(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;

	//Variable declaration
	int m1=0,n1=0,m2=0,n2=0,m3=0,n3=0,m4=0,n4=0,row=1,col=1,i=0; 
	int *piAddressVar1 = NULL;
	int *piAddressVar2 = NULL;
	int *piAddressVar3 = NULL;
	int *piAddressVar4 = NULL;
	double *roll_off_factor = NULL;
	double *filter_length= NULL;
	double *upsampling_factor = NULL;
	double *matrixOfInputSymbols = NULL;
	double *matrixOfPulseShape = NULL;
		
	
	//-----------------------Checking the no. of output and input arguements----------------------------
	/*c=foo(a,b)
	  Input arguements=4
	  Output arguements=1*/
	  CheckInputArgument(pvApiCtx, 4, 4) ;
    	  CheckOutputArgument(pvApiCtx, 1, 1) ;

	
	//-----------------------------For First Input Arguement(Input Symbols)---------------------------------- 
	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVar1);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}	
	

	//Checking if input signal is not of complex type
	if ( !isDoubleType(pvApiCtx, piAddressVar1) ||  isVarComplex(pvApiCtx, piAddressVar1) )
    	{
        	Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 1);
        	return 0;
    	}


    	//Getting input signal
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar1, &m1, &n1, &matrixOfInputSymbols);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}


	//Checking input signal is a vector
	if( ( m1 != 1 ) && ( n1 != 1) ) 
	{
		Scierror(999, _("%s: Single row or column vector expected.\n"), fname, 1);
		return 0;
	}
	

	//Checking if the input signal is BPSK modulated type
	for( i = 0 ; i < m1*n1 ; i++ )
	{
		if( (matrixOfInputSymbols[i] != -1) && (matrixOfInputSymbols[i] != 1) )
		{
			Scierror(999, _("%s: {1, -1} Input Signal Expected.\n"), fname, 1);
			return 0;
		}
	}
	


	//-------------------------For Second Input Arguement (Roll Off factor)----------------------------- 
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
        	Scierror(999, _("%s: Wrong type for input argument #%d: A boolean matrix expected.\n"), fname, 1);
        	return 0;
    	}

    	//Getting input signal
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar2, &m2, &n2, &roll_off_factor);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}

	//Checking input signal is a vector
	if( ( m2 != 1 ) || ( n2 != 1) ) 
	{
		Scierror(999, _("%s: Single input expected.\n"), fname, 1);
		return 0;
	}


	//------------------------------For third arguement(filter length)-------------------------------

	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVar3);
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
	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar3, &m3, &n3, &filter_length);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}


	//Checking for a single integer input 
	if( ( m3 != 1 ) || ( n3 != 1) ) 
	{
		Scierror(999, _("%s: Single integer input expected.\n"), fname, 1);
		return 0;
	}


	//------------------------------For fourth arguement(upsampling factor)-------------------------------

	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVar4);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}	


	//Checking if input signal is of boolean type
	if ( !isDoubleType(pvApiCtx, piAddressVar4) ||  isVarComplex(pvApiCtx, piAddressVar4) )
    	{
        	Scierror(999, _("%s: Wrong type for input argument #%d: An integer matrix expected.\n"), fname, 1);
        	return 0;
    	}


	//Retrieve dimensions and data
	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar4, &m4, &n4, &upsampling_factor);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}


	//Checking for a single integer input 
	if( ( m4 != 1 ) || ( n4 != 1) ) 
	{
		Scierror(999, _("%s: Single integer input expected.\n"), fname, 1);
		return 0;
	}

		
	//-------------------------------Implementation of get_pulse_shape------------------------------
	//Vectors for storing output
	vec inputVector,outputVector;


	//Converting the input matrix into vector
	inputVector = vec(matrixOfInputSymbols, m1*n1);


	//Invoking get_pulse_shape
	outputVector = Root_Raised_Cosine<double>(*roll_off_factor, (int) *filter_length, (int) *upsampling_factor ).shape_symbols(inputVector); 


	//----------------------------------------Output Arguements---------------------------------------
	//Defining the no. of rows in matrix to be sent as output
	col = outputVector.length();

	//Storing the output in a double array to be sent to scilab
	matrixOfPulseShape = (double *) malloc( sizeof(double)*outputVector.length());
	for(i=0; i < outputVector.length(); i++)
	{
		matrixOfPulseShape[i] = outputVector(i);
	}


	//Creating Output Modulated Signal
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
