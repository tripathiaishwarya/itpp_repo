/*----------------------------------------------------------------------------------------------------------------
Code for BPSK Modulation
Aishwarya Tripathi
May 20,2015
----------------------------------------------------------------------------------------------------------------*/

/*I/O format of the code for scilab engine
* 
* <outputBitStream> = itpp_bpsk_mod(<binaryInputBitStream>)
*
* The code interfaces the function BPSK().modulate_bits() of ITPP toolbox with the scilab engine.
*
* ITPP toolbox funcction description : The input to the modulate_bits() function is a binary vector and the output is a vector of (1,-1).
* ----------------------------------
*
* Input/Output Details : 
*----------------------
* It is expected that the user inputs a single dimension binary matrix of (0,1) in scilab engine while calling the itpp_bpsk_mod() 
* and expect the function to return a single dimension matrix as output in scilab console.
*/

/***********Sample I/O in scilab console******************
-->a=[0 1 0 1 0 1]
 a  =
 
    0.    1.    0.    1.    0.    1.  
 
-->c=itpp_bpsk_mod(a)

 c  =
 
    1.  - 1.    1.  - 1.    1.  - 1.  
*/



#include <itpp/base/vec.h>
#include <itpp/comm/modulator.h>

using namespace itpp;

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include <localization.h>

/*=================================================================*/

int itpp_bpsk_mod(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;

	//Variable declaration
	int m=0,n=0,i=0,flag=1;
	int *piAddressVar = NULL;
	double *matrixOfInputSignal = NULL;
	double *matrixOfModulatedOutput = NULL;
		
	
	

	//Checking the output and input arguements
	//c=foo(a)
	//Input arguements=1
	//Output arguements=1
	CheckInputArgument(pvApiCtx, 1, 1) ;
    	CheckOutputArgument(pvApiCtx, 1, 1) ;

	
	//-----------------------------For Input Arguement(Input Signal)---------------------------------- 
	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVar);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}	
	

	//Checking if input signal is of boolean type
	if ( !isDoubleType(pvApiCtx, piAddressVar) ||  isVarComplex(pvApiCtx, piAddressVar) )
    	{
        	Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 1);
        	return 0;
    	}

    	//Getting input signal
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar, &m, &n, &matrixOfInputSignal);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}

	//Checking input signal is a vector
	if( ( m != 1 ) && ( n != 1) ) 
	{
		Scierror(999, _("%s: Single row or column vector expected.\n"), fname, 1);
		return 0;
	}
	
	//Checking if the input signal is digital type
	for( i = 0 ; i < m*n ; i++ )
	{
		if( (matrixOfInputSignal[i] != 0) && (matrixOfInputSignal[i] != 1) )
		{
			Scierror(999, _("%s: Digital Input Signal Expected.\n"), fname, 1);
			return 0;
		}
	}
	
	//-------------------------------Implementation of BPSK modulation------------------------------
	//Declaring input and output vectors for modulation
	vec outputVector;
	bvec inputVector;


	//Invoking the PSK modulation function
	inputVector = itpp::to_bvec(vec(matrixOfInputSignal, m * n));
	outputVector = BPSK().modulate_bits(inputVector);	

	//----------------------------------------Output Arguements---------------------------------------
	//Storing the output in a double array to be sent to scilab
	matrixOfModulatedOutput = (double *) malloc( sizeof(double)*m*n);
	for( i=0 ; i < m*n ; i++ )
		matrixOfModulatedOutput[i] = outputVector(i);		

	
	
	//Creating Output Modulated Signal
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m, n, matrixOfModulatedOutput); 
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}

	free(matrixOfModulatedOutput); 


	//-------------------------------Returning output arguements to Scilab----------------------------

	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	ReturnArguments(pvApiCtx);
	return 0;
}
/*===============================================================================*/
}
