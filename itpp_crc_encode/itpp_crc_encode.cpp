/*----------------------------------------------------------------------------------------------------------------
Code for CRC Code Encoding 
Author: Aishwarya Tripathi
June 4,2015
----------------------------------------------------------------------------------------------------------------*/


/*I/O format of the code for scilab engine
* 
* <outputBitStream> = itpp_crc_encode(<InputBitStream>,<string>)
*
* The code interfaces the function CRC_Code(<string>).encode(bvec <inputBitStream>) of ITPP toolbox with the scilab engine.
*
* ITPP toolbox function description : 
*-----------------------------------
* The input to the encode() function is a binary vector and a string, 
* and the output is encoded binary vector.
* 
*
* Input/Output Details : 
*----------------------
* It is expected that the user inputs a single dimension binary matrix of (0,1) and a string in scilab engine while calling the
* itpp_crc_encode() function and expect the function to return a single dimension binary matrix of (0,1) as encoded output in scilab
* console.
*/

/***********Sample I/O in scilab console******************
-->a=[1 0 0 1 1 0]
 a  =
 
    1.    0.    0.    1.    1.    0.  
 
-->b='CRC-4'
 b  =
 
 CRC-4   
 
-->c=itpp_crc_encode(a,b)
 c  =
 
    1.    0.    0.    1.    1.    0.    1.    1.    0.    0.  
 
*/

#include <itpp/base/vec.h>
#include <itpp/itbase.h>
#include <itpp/comm/crc.h>


using namespace itpp;

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include <localization.h>


/*=================================================================*/

int itpp_crc_encode(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;


	//Variable declaration
	int m1=0,n1=0,m2=0,n2=0,i=0,row=1,col=1;
	int *piAddressVar1 = NULL;
	int *piAddressVar2 = NULL;
	double *matrixOfInputBits = NULL;
	double *matrixOfCrcEncodedBits = NULL;
	char **str = NULL;
	int *strlen = NULL;
		

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
	
		
	//------------------------------For Second Arguement(String)-----------------------------------
	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVar2);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}	
	

	//first call to retrieve dimensions
	sciErr = getMatrixOfString(pvApiCtx, piAddressVar2, &m2, &n2, NULL, NULL);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	strlen = (int *)malloc(sizeof(int) * m2 * n2);

	//second call to retrieve length of each string
	sciErr = getMatrixOfString(pvApiCtx, piAddressVar2, &m2, &n2, strlen, NULL);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	str = (char**)malloc(sizeof(char*) * m2 * n2);
	for(i = 0 ; i < m2*n2 ; i++)
	{
		str[i] = (char *)malloc(sizeof(char) * (strlen[i] + 1));//+ 1 for null termination
	}


	//third call to retrieve data
	sciErr = getMatrixOfString(pvApiCtx, piAddressVar2, &m2, &n2, strlen, str);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	
	//---------------------------Implementation of CRC Encoding------------------------------
	//Vectors for storing input and encoded bits
	bvec inputVector, outputVector;

	
	//Converting the input arguement into a binary vector
	inputVector = to_bvec(vec(matrixOfInputBits, m1*n1)); 


	//Invoking the CRC encoding function
	outputVector = CRC_Code(*str).encode(inputVector);
	

	//----------------------------------------Output Arguements---------------------------------------
	//Storing the output in a double array to be sent to scilab
	matrixOfCrcEncodedBits = (double *) malloc( sizeof(double)*outputVector.length());
	for( i=0; i<outputVector.length(); i++ )
		matrixOfCrcEncodedBits[i] = outputVector(i);		

	
	//Setting dimensions for the output matrix 
	if(n1==1)
		row = outputVector.length();
	else
		col = outputVector.length();


	//Creating Output Modulated Signal
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, row, col, matrixOfCrcEncodedBits);	 
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}
	free(matrixOfCrcEncodedBits); 


	//-------------------------------Returning output arguements to Scilab----------------------------
	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	ReturnArguments(pvApiCtx);
	return 0;
}
/*===============================================================================*/
}
