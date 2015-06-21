/*----------------------------------------------------------------------------------------------------------------
Code for QPSK Demodulation
Aishwarya Tripathi
May 27,2015
----------------------------------------------------------------------------------------------------------------*/


/*I/O format of the code for scilab engine
* 
* <outputComplexVector> = itpp_qpsk_demod(<binaryInputBitStream>)
*
* The code interfaces the function QPSK().demodulate_bits() of ITPP toolbox with the scilab engine.
*
* ITPP toolbox funcction description : The input to the demodulate_bits() function is a complex vector and the output is a binary vector.
* ----------------------------------
*
* Input/Output Details : 
*----------------------
* It is expected that the user inputs a single dimension complex matrix in scilab engine while calling the itpp_bpsk_mod() 
* and expect the function to return a single dimension binary matrix of (0,1) as output in scilab console.
*/

/***********Sample I/O in scilab console******************
-->a=[1 %i -%i -1]
 a  =
 
    1.    i    - i    - 1.  
 
-->c=itpp_qpsk_demod(a)
 c  =
 
    0.    0.    0.    1.    1.    0.    1.    1.  
 
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

int itpp_qpsk_demod(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;

	//Variable declaration
	int m=0,n=0,i=0,row=1,col=1;
	int *piAddressVar = NULL;
	double *matrixOfOutputSignal = NULL;
	double *matrixOfModulatedInputReal = NULL;
	double *matrixOfModulatedInputImg = NULL;		
	
	

	//--------------------------Checking the no. of input and output arguements-------------------------
	//c=foo(a)
	//Input arguements=1
	//Output arguements=1
	CheckInputArgument(pvApiCtx, 1, 1) ;
    	CheckOutputArgument(pvApiCtx, 1, 1) ;

	
	//--------------------------For first input arguement (Input Signal) -----------------------
	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVar);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}	


	//Checking if input signal is of complex type
	if ( !isVarComplex(pvApiCtx, piAddressVar1) )
    	{
        	Scierror(999, _("%s: Wrong type for input argument #%d: A complex matrix expected.\n"), fname, 1);
        	return 0;
    	}


    	//Getting input signal
    	sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddressVar, &m, &n, &matrixOfModulatedInputReal, &matrixOfModulatedInputImg);
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
	
	
	//-------------------------------Implementation of QPSK modulation------------------------------
	//Declaring input and output vectors for modulation
	bvec outputVector;
	cvec inputVector;


	//Converting input matrix to complex vector
	inputVector = itpp::to_cvec(itpp::vec(matrixOfModulatedInputReal, m*n), itpp::vec(matrixOfModulatedInputImg, m*n));


	//Calling the demodulating function
	outputVector = QPSK().demodulate_bits(inputVector);



	//Storing the output in a double array to be sent to scilab
	matrixOfOutputSignal = (double *) malloc( sizeof(double)*outputVector.length());
	for(i=0; i < outputVector.length(); i++)
	{
		matrixOfOutputSignal[i] = ((outputVector(i) == 0) ? 0 : 1);
	}


	//----------------------------------------Output Arguements---------------------------------------
	//Declaring no of rows and coloums for the output matrix
	if( n==1 )
		row = outputVector.length();
	else
		col = outputVector.length();
	
	
	//Creating Output Modulated Signal
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, row, col, matrixOfOutputSignal);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}

	free(matrixOfOutputSignal);
	 

	//-------------------------------Returning output arguements to Scilab----------------------------

	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	ReturnArguments(pvApiCtx);
	return 0;
}
/*===============================================================================*/
}
