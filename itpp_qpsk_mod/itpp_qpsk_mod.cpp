/*----------------------------------------------------------------------------------------------------------------
Code for QPSK Modulation
Aishwarya Tripathi
May 27,2015
----------------------------------------------------------------------------------------------------------------*/

/*I/O format of the code for scilab engine
* 
* <outputComplexVector> = itpp_qpsk_mod(<binaryInputBitStream>)
*
* The code interfaces the function QPSK().modulate_bits() of ITPP toolbox with the scilab engine.
*
* ITPP toolbox funcction description : The input to the modulate_bits() function is a binary vector and the output is a complex vector.
* ----------------------------------
*
* Input/Output Details : 
*----------------------
* It is expected that the user inputs a single dimension binary matrix of (0,1) in scilab engine while calling the itpp_qpsk_mod() 
* and expect the function to return a single dimension complex matrix as output in scilab console.
*/

/***********Sample I/O in scilab console******************
-->a=[0 0 0 1 1 0 1 1]
 a  =
 
    0.    0.    0.    1.    1.    0.    1.    1.  
 
-->c=itpp_qpsk_mod(a)
 c  =
 
    1.    i    - i    - 1.  
 
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

int itpp_qpsk_mod(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;

	//Variable declaration
	int m=0,n=0,i=0,row=1,col=1;
	int *piAddressVar = NULL;
	double *matrixOfInputSignal = NULL;
	double *matrixOfModulatedOutputReal = NULL;
	double *matrixOfModulatedOutputImg = NULL;		
	
	

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
	

	//Checking if input signal is of boolean type
	if ( !isDoubleType(pvApiCtx, piAddressVar) ||  isVarComplex(pvApiCtx, piAddressVar) )
    	{
        	Scierror(999, _("%s: Wrong type for input argument #%d: A boolean vector expected.\n"), fname, 1);
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

	
	//Error check for even no. of input bits
	if( (m*n) % 2 == 1 )
	{
		Scierror(999, _("%s: Even no. of input bits expected.\n"), fname, 1);
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

	
	//-------------------------------Implementation of QPSK modulation------------------------------
	//Declaring input and output vectors for modulation
	bvec inputVector;
	cvec outputVector;


	//Invoking the QPSK modulation function
	inputVector = itpp::to_bvec(vec(matrixOfInputSignal, m * n));
	outputVector = QPSK().modulate_bits(inputVector);	


	//Defining the no. of rows and columns in matrix to be sent as output
	if( n == 1 )
		row = outputVector.length();
	else
		col = outputVector.length();	


	//----------------------------------------Output Arguements---------------------------------------
	//Reserve space in scilab memory for output matrix and fill it
	sciErr = allocComplexMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, row, col, &matrixOfModulatedOutputReal, &matrixOfModulatedOutputImg);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}


	//Storing the output in a double array to be sent to scilab

	for(i=0 ; i < outputVector.length() ; i++)
	{
		matrixOfModulatedOutputReal[i] = real(outputVector(i));		
		matrixOfModulatedOutputImg[i]  = imag(outputVector(i));
	}
	

	//-------------------------------Returning output arguements to Scilab----------------------------

	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	ReturnArguments(pvApiCtx);
	return 0;
}
/*===============================================================================*/
}
