/*----------------------------------------------------------------------------------------------------------------
Code for BERC get errors
Aishwarya Tripathi
June 10,2015
----------------------------------------------------------------------------------------------------------------*/

/*I/O format of the code for scilab engine
* 
* <outputErrorCount> = itpp_berc_get_errors(<binaryInputBitStream1>,<inputBitStream2>)
*
* The code interfaces the function BERC().get_errors() of ITPP toolbox with the scilab engine.
*
* ITPP toolbox funcction description : The get_errors() function gives the error count of two binary bit streams.
* ----------------------------------
*
* Input/Output Details : 
*----------------------
* It is expected that the user inputs two binary arrays of (0,1) in scilab engine while calling the itpp_berc_get_errors() 
* and expect the function to return the error count of the two bitstreams as output in scilab console.
*/

/***********Sample I/O in scilab console******************
-->a=[1 0 0 1 1 0]
 a  =
 
    1.    0.    0.    1.    1.    0.  
 
-->b=[1 1 0 0 1 1]
 b  =
 
    1.    1.    0.    0.    1.    1.  
 
-->c=itpp_berc_get_errors(a,b)
 c  =
 
    3.  
 */

#include <itpp/base/vec.h>
#include <itpp/itbase.h>
#include <itpp/comm/error_counters.h>

using namespace itpp;

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include <localization.h>

/*=================================================================*/

int itpp_berc_get_errors(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;


	//Variable declaration
	int m1=0,n1=0,m2=0,n2=0,i=0;
	int *piAddressVar1 = NULL;
	int *piAddressVar2 = NULL;
	double *matrixOfInputBits1 = NULL;
	double *matrixOfInputBits2 = NULL;
		

	//Checking the output and input arguements
	//c=foo(a)
	//Input arguements=2
	//Output arguements=1
	CheckInputArgument(pvApiCtx, 2, 2) ;
    	CheckOutputArgument(pvApiCtx, 1, 1) ;

	
	//-----------------------------For First Input Arguement(Input Bits 1)---------------------------------- 
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
        	Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 1);
        	return 0;
    	}

    	//Getting input signal
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar1, &m1, &n1, &matrixOfInputBits1);
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
		if( (matrixOfInputBits1[i] != 0) && (matrixOfInputBits1[i] != 1) )
		{
			Scierror(999, _("%s: Digital Input Signal Expected.\n"), fname, 1);
			return 0;
		}
	}
	

	//-----------------------------For Second Input Arguement(Input Bits 2)---------------------------------- 
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
        	Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 1);
        	return 0;
    	}

    	//Getting input signal
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar2, &m2, &n2, &matrixOfInputBits2);
    	if (sciErr.iErr)
	{
        	printError(&sciErr, 0);
        	return 0;
    	}

	//Checking input signal is a vector
	if( ( m2 != 1 ) && ( n2 != 1) ) 
	{
		Scierror(999, _("%s: Single row or column vector expected.\n"), fname, 1);
		return 0;
	}
	
	//Checking if the input signal is digital type
	for( i = 0 ; i < m2*n2 ; i++ )
	{
		if( (matrixOfInputBits2[i] != 0) && (matrixOfInputBits2[i] != 1) )
		{
			Scierror(999, _("%s: Digital Input Signal Expected.\n"), fname, 1);
			return 0;
		}
	}
	

	//-------------------------------Implementation of BERC count errors------------------------------
	//Declaring input and output vectors for modulation
	bvec inputVector1, inputVector2;
	double output=0;

	int indelay=0, inignorefirst=0, inignorelast=0;


	//Converting the input to binary vectors	
	inputVector1 = itpp::to_bvec(vec(matrixOfInputBits1, m1 * n1));
	inputVector2 = itpp::to_bvec(vec(matrixOfInputBits2, m2 * n2));


	//Invoking the BERC count_errors function
	BERC berc(indelay, inignorefirst, inignorelast);
	berc.count(inputVector1, inputVector2);
	output = berc.get_errors();	


	//----------------------------------------Output Arguements---------------------------------------
	
	//Creating the output
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 1,&output); 
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
	}

	//-------------------------------Returning output arguements to Scilab----------------------------
	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	ReturnArguments(pvApiCtx);
	return 0;
}
/*===============================================================================*/
}
