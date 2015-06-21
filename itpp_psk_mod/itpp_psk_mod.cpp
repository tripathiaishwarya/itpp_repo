/*----------------------------------------------------------------------------------------------------------------
Code for BPSK Modulation
Aishwarya Tripathi
May 27,2015
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


#include <itpp/base/vec.h>
#include <itpp/comm/modulator.h>

using namespace itpp;

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include <localization.h>

/*=================================================================*/

int itpp_psk_mod(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;

	//Variable declaration
	int m1=0,n1=0,m2=0,n2=0,i=0,c=0,row=1,col=1; 
	int *piAddressVar1 = NULL;
	int *piAddressVar2 = NULL;
	double *matrixOfInputSignal = NULL;
	double *matrixOfModulatedOutputReal = NULL;
	double *matrixOfModulatedOutputImg = NULL;
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
	

	//Checking if input signal is of boolean type
	if ( !isDoubleType(pvApiCtx, piAddressVar1) ||  isVarComplex(pvApiCtx, piAddressVar1) )
    	{
        	Scierror(999, _("%s: Wrong type for input argument #%d: A boolean matrix expected.\n"), fname, 1);
        	return 0;
    	}

    	//Getting input signal
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar1, &m1, &n1, &matrixOfInputSignal);
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
	
	//Checking if the input signal is digital type
	for( i = 0 ; i < m1*n1 ; i++ )
	{
		if( (matrixOfInputSignal[i] != 0) && (matrixOfInputSignal[i] != 1) )
		{
			Scierror(999, _("%s: Digital Input Signal Expected.\n"), fname, 1);
			return 0;
		}
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
	if (!isDoubleType(pvApiCtx, piAddressVar2) || isVarComplex(pvApiCtx, piAddressVar2) )
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
	

	//Checking for correct no. of bits in input bit stream
	if( (m1*n1) % c != 0 )
	{
		Scierror(999, _("%s: Incorrect no. of input bits .\n"), fname, 1);
		return 0;
	}


		
	//-------------------------------Implementation of PSK modulation------------------------------
	//Declaring input and output vectors for modulation
	cvec outputVector;
	bvec inputVector;


	//Invoking the PSK modulation function
	inputVector = itpp::to_bvec(vec(matrixOfInputSignal, m1 * n1));
	outputVector = PSK((int) *m).modulate_bits(inputVector);


	//----------------------------------------Output Arguements---------------------------------------
	//Defining the no. of rows and columns in matrix to be sent as output
	if( n1 == 1 )
		row = outputVector.length();
	else
		col = outputVector.length();	


	//Reserve space in scilab memory and fill it
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
