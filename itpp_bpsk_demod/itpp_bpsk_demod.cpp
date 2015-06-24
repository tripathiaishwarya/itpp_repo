/*----------------------------------------------------------------------------------------------------------------
Code for BPSK Demodulation
Aishwarya Tripathi
May 20,2015
----------------------------------------------------------------------------------------------------------------*/
/*I/O format of the code for scilab engine
* 
* <binaryOutputBitStream> = bpsk_demod(<inputBitStream>)
*
* The code interfaces the function BPSK().demodulate_bits() of ITPP toolbox with the scilab engine.
*
* ITPP toolbox funcction description : The input to the demodulate_bits() function is a binary vector and the output is a vector of (1,-1).
* ----------------------------------
*
* Input/Output Details : 
*----------------------
* It is expected that the user inputs a single dimension matrix of (-1,1) in scilab engine while calling the bpsk_demod() 
* and expect the function to return a single dimension matrix of (0,1) as output in scilab console.
*/

/***********Sample I/O in scilab console******************
-->a=[1 -1 1 1 -1 -1 1]
 a  =
 
    1.  - 1.    1.    1.  - 1.  - 1.    1.  
 
-->c=bpsk_demod(a)
 c  =
 
    0.    1.    0.    0.    1.    1.    0.  
 
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

int itpp_bpsk_demod(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;


	//Variable declaration
	int m=0,n=0,i=0,flag=1;
	int *piAddressVar = NULL;
	double *matrixOfInputSignal = NULL;
	double *matrixOfDemodulatedOutput = NULL;
		
	
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
	

	//Checking if input signal is not of complex type
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
	

	//Checking if the input signal is BPSK modulated type
	for( i = 0 ; i < m*n ; i++ )
	{
		if( (matrixOfInputSignal[i] != -1) && (matrixOfInputSignal[i] != 1) )
		{
			Scierror(999, _("%s: {1,-1} Input Signal Expected.\n"), fname, 1);
			return 0;
		}
	}
	

	//-------------------------------Implementation of BPSK modulation------------------------------	
	//Declaring input and output vectors for modulation
	bvec outputVector;
	vec inputVector;
	

	//Invoking the PSK modulation function
	inputVector = itpp::to_vec(vec(matrixOfInputSignal, m * n));
	outputVector = BPSK().demodulate_bits(inputVector);	


	//----------------------------------------Output Arguements---------------------------------------
	//Storing the output in a double array to be sent to scilab
	matrixOfDemodulatedOutput = (double *) malloc( sizeof(double) * m*n );
	for(i = 0;i < m*n; i++)
	{
		matrixOfDemodulatedOutput[i] = ((outputVector(i) == 0) ? 0 : 1);
	}

	
	//Creating Output Modulated Signal
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m, n, matrixOfDemodulatedOutput); 
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}

	free(matrixOfDemodulatedOutput); 

	//-------------------------------Returning output arguements to Scilab----------------------------
	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	ReturnArguments(pvApiCtx);
	return 0;
}
/*===============================================================================*/
}
