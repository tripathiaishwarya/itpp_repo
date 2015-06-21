/*----------------------------------------------------------------------------------------------------------------
Code for PSK Demodulation
Aishwarya Tripathi
May 27,2015
----------------------------------------------------------------------------------------------------------------*/


/*I/O format of the code for scilab engine
* 
* <outputBitStream> = itpp_psk_demod(<InputComplexVector>,<integer m>)
*
* The code interfaces the function PSK(int m).demodulate_bits(cvec <inputComplexVector>) of ITPP toolbox with the scilab engine.
*
* ITPP toolbox function description : 
*-----------------------------------
* The input to the demodulate_bits() function is a complex vector and an integer M (should be a power of 2) defining M-ary behaviour of * * PSK, and the output is a binary vector.
* 
*
* Input/Output Details : 
*----------------------
* It is expected that the user inputs a single dimension complex matrix and an integer m in scilab engine while calling the
* itpp_bpsk_demod() function and expect the function to return a single dimension binary matrix as output in scilab console.
*/

/***********Sample I/O in scilab console******************
-->a=[0.7071068-0.7071068*%i  -1 -0.7071068-0.7071068*%i]
 a  =
 
    0.7071068 - 0.7071068i  - 1.  - 0.7071068 - 0.7071068i  
 
-->b=8
 b  =
 
    8.  
 
-->c=itpp_psk_demod(a,b)
 c  =
 
    1.    0.    0.    1.    1.    0.    1.    1.    1.  
 
*/


#include <itpp/base/vec.h>
#include <itpp/comm/modulator.h>

using namespace itpp;
using namespace std;

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include <localization.h>

/*=================================================================*/

int itpp_psk_demod(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;

	//Variable declaration
	int m1=0,n1=0,m2=0,n2=0,i=0,c=0,row=1,col=1; 
	int *piAddressVar1 = NULL;
	int *piAddressVar2 = NULL;
	double *matrixOfInputSignalReal = NULL;
	double *matrixOfInputSignalImg = NULL;
	double *matrixOfDemodulatedOutput = NULL;
	double *m = NULL;
		
	
	//-----------------------Checking the no. of output and input arguements----------------------------
	/*c=foo(a,b)
	  Input arguements=2
	  Output arguements=1*/
	  CheckInputArgument(pvApiCtx, 2, 2) ;
    	  CheckOutputArgument(pvApiCtx, 1, 1) ;

	
	//-------------------------For First Input Arguement (Input Signal)----------------------------- 
	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVar1);
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
    	sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddressVar1, &m1, &n1, &matrixOfInputSignalReal, &matrixOfInputSignalImg);
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
	


	//------------------------------For second arguement(m of M-ary PSK)-------------------------------

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
	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar2, &m2, &n2, &m);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}


	//Checking if m is not an integer
	if((*m - floor(*m)) != 0)
	{
		Scierror(999, _("%s: Integer input expected.\n"), fname, 1);
		return 0;
	}


	//Checking if m is greater than zero
	if(*m <= 0)
	{
		Scierror(999, _("%s: Positive non-zero integer expected.\n"), fname, 1);
		return 0;
	}


	//Checking for a single integer input 
	if( ( m2 != 1 ) || ( n2 != 1) ) 
	{
		Scierror(999, _("%s: Single integer input expected.\n"), fname, 1);
		return 0;
	}

	
	//Checking for integer to be a exact power of 2
	i=1;
	while(1)
	{
		i *= 2;
		c += 1;
		if( *m == i)
			break;
		if( *m < i)
		{
			Scierror(999, _("%s: Second input should be a power of 2.\n"), fname, 1);
			return 0;
		}
	}
	
		
	//-------------------------------Implementation of PSK modulation------------------------------
	//Declaring input and output vectors for modulation
	vec inputReal, inputImg;
	bvec outputVector;
	cvec inputVector;


	//Converting input Real and imaginary matrices into vectors
	inputReal = vec(matrixOfInputSignalReal, m1 * n1);
	inputImg = vec(matrixOfInputSignalImg, m1 * n1);


	//Invoking the PSK modulation function
	inputVector = itpp::to_cvec(inputReal, inputImg);
	outputVector = PSK((int) *m).demodulate_bits(inputVector);


	//----------------------------------------Output Arguements---------------------------------------
	//Defining the no. of rows and columns in matrix to be sent as output
	if( n1 == 1 )
		row = outputVector.length();
	else
		col = outputVector.length();	


	//Storing the output in a double array to be sent to scilab
	matrixOfDemodulatedOutput = (double *) malloc( sizeof(double)*outputVector.length());
	for(i=0; i < outputVector.length(); i++)
	{
		matrixOfDemodulatedOutput[i] = ((outputVector(i) == 0) ? 0 : 1);
	}


	//Creating Output Modulated Signal
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, row, col, matrixOfDemodulatedOutput);
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
