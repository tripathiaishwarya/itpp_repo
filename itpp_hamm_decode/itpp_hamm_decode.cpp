/*----------------------------------------------------------------------------------------------------------------
Code for Hamming Decoding 
Author : Aishwarya Tripathi
June 1,2015
----------------------------------------------------------------------------------------------------------------*/


/*I/O format of the code for scilab engine
* 
* <outputBitStream> = itpp_hamm_decode(<InputBitStream>,<integer m>)
*
* The code interfaces the function HammingCode(int m).decode(bvec <inputBitStream>) of ITPP toolbox with the scilab engine.
*
* ITPP toolbox function description : 
*-----------------------------------
* The input to the decode() function is a binary vector and an integer M, 
* and the output is encoded binary vector.
* 
*
* Input/Output Details : 
*----------------------
* It is expected that the user inputs a single dimension binary matrix of (0,1) and an integer m in scilab engine while calling the
* itpp_hamm_decode() function and expect the function to return a single dimension binary matrix of (0,1) as decoded output in scilab
* console.
*/

/***********Sample I/O in scilab console******************
-->a=[0 0 1 0 0 1 1 1 0 1 1 0 1 0]
 a  =
 
 
         column  1 to 10
 
    0.    0.    1.    0.    0.    1.    1.    1.    0.    1.  
 
         column 11 to 14
 
    1.    0.    1.    0.  
 
-->b=3
 b  =
 
    3.  
 
-->c=itpp_hamm_decode(a,b)
 c  =
 
    0.    0.    1.    1.    1.    0.    1.    0.  
 
*/


//#include <numeric>
#include <itpp/base/vec.h>
//#include <itpp/itcomm.h>
//#include <itpp/itbase.h>
#include <itpp/comm/hammcode.h>


using namespace itpp;
//using namespace std;

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
//#include "BOOL.h"
#include <localization.h>
//#include "MALLOC.h"
//#include <itpp/base/vec.h>

/*=================================================================*/

int itpp_hamm_decode(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;

	//Variable declaration
	int m1=0,n1=0,m2=0,n2=0,i=0,flag=1,row=1,col=1;
	int *piAddressVar1 = NULL;
	int *piAddressVar2 = NULL;
	double *matrixOfInputBits = NULL;
	double *matrixOfHammDecodedBits = NULL;
	double *m;
		

	//-----------------------Checking the output and input arguements---------------------------------
	//c=foo(a,b)
	//Input arguements=2
	//Output arguements=1
	CheckInputArgument(pvApiCtx, 2, 2) ;
    	CheckOutputArgument(pvApiCtx, 1, 1) ;

	
	//-------------------------For First Arguement (Input Bitstream)----------------------------------  
	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVar1);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}	
	

	//Checking if input bitstream is not of complex type
	if ( !isDoubleType(pvApiCtx, piAddressVar1) ||  isVarComplex(pvApiCtx, piAddressVar1) )
    	{
        	Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 1);
        	return 0;
    	}


    	//Getting input bitstream
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar1, &m1, &n1, &matrixOfInputBits);
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
		if( (matrixOfInputBits[i] != 0) && (matrixOfInputBits[i] != 1) )
		{
			Scierror(999, _("%s: Digital Input Signal Expected.\n"), fname, 1);
			return 0;
		}
	}
	
		
	//------------------------------For Second Arguement(M)-----------------------------------
	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVar2);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}	
	

	//Checking if input m is not complex type
	if ( !isDoubleType(pvApiCtx, piAddressVar2) ||  isVarComplex(pvApiCtx, piAddressVar2) )
    	{
        	Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 1);
        	return 0;
    	}


	//Getting value of m
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar2, &m2, &n2, &m);
    	if (sciErr.iErr)
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


	//Checking m is a single number
	if( ( m2 != 1 ) || ( n2 != 1) ) 
	{
		Scierror(999, _("%s: Single number expected.\n"), fname, 1);
		return 0;
	}

	
	//---------------------------Implementation of Hamming Code Decoding------------------------------
	//Vectors for storing input and decoded bits
	bvec inputVector, outputVector;


	//Converting the input arguement into a binary vector
	inputVector = to_bvec(vec(matrixOfInputBits, m1*n1)); 


	//Invoking the Hamming code decoding function
	outputVector = Hamming_Code((int) *m).decode(inputVector);
	

	//----------------------------------------Output Arguements---------------------------------------
	//Storing the output in a double array to be sent to scilab
	matrixOfHammDecodedBits = (double *) malloc( sizeof(double)*outputVector.length());
	for( i=0; i<outputVector.length(); i++ )
		matrixOfHammDecodedBits[i] = outputVector(i) ? 1 :0;		

	
	//Setting dimensions for the output matrix 
	if(n1==1)
		row = outputVector.length();
	else
		col = outputVector.length();


	//Creating Output Modulated Signal
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, row, col, matrixOfHammDecodedBits);	 
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}

	free(matrixOfHammDecodedBits); 



	//-------------------------------Returning output arguements to Scilab----------------------------
	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	ReturnArguments(pvApiCtx);
	return 0;
}
/*===============================================================================*/
}
