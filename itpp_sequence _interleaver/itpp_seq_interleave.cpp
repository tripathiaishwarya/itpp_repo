/*----------------------------------------------------------------------------------------------------------------
Code for Sequence Inteleaver
Aishwarya Tripathi
June 8,2015
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
#include <itpp/base/vec.h>
#include <itpp/itcomm.h>
#include <itpp/itbase.h>
#include <itpp/comm/interleave.h>

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

int itpp_seq_interleave(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;


	//Variable declaration
	int m1=0,n1=0,m2=0,n2=0,m3=0,n3=0,i=0,row1=1,col1=1,row2=1,col2=1; 
	int *piAddressVar1 = NULL;
	int *piAddressVar2 = NULL;
	int *piAddressVar3 = NULL;
	double *matrixOfInputSymbols = NULL;
	double *matrixOfSequenceInterleavedSymbols = NULL;
	double *matrixOfInterleaverSequence = NULL;
	double *in_interleaver_depth = NULL;
		
	
	

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
	
	/*//Checking if the input signal is digital type
	for( i = 0 ; i < m1*n1 ; i++ )
	{
		if( (matrixOfInputSymbols[i] != -1) && (matrixOfInputSymbols[i] != 1) )
		{
			Scierror(999, _("%s: Digital Input Signal Expected.\n"), fname, 1);
			return 0;
		}
	}*/


	/*//------------------------------For second arguement(In Interleaver depth)-------------------------------

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
	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar2, &m2, &n2, &in_interleaver_depth);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}


	//Checking for a single integer input 
	if( ( m2 != 1 ) || ( n2 != 1) ) 
	{
		Scierror(999, _("%s: Single integer input expected.\n"), fname, 1);
		return 0;
	}

	*/



	//-------------------------For Third Input Arguement (Interleaver Sequence)----------------------------- 
	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVar3);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}	
	

	//Checking if input signal is of boolean type
	if ( !isDoubleType(pvApiCtx, piAddressVar3) ||  isVarComplex(pvApiCtx, piAddressVar3) )
    	{
        	Scierror(999, _("%s: Wrong type for input argument #%d: A boolean matrix expected.\n"), fname, 1);
        	return 0;
    	}

    	//Getting input signal
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar1, &m3, &n3, &matrixOfInterleaverSequence);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}

	//Checking input signal is a vector
	if( ( m3 != 1 ) && ( n3 != 1) ) 
	{
		Scierror(999, _("%s: Single row or column vector expected.\n"), fname, 1);
		return 0;
	}


		
	//-------------------------------Implementation of Sequence Interleaving------------------------------
	//Declaring input and output vectors for interleaving 
	vec inputVector, outputVector;
	ivec interSeq;


	//Invoking the PSK modulation function
	inputVector = itpp::vec(vec(matrixOfInputSymbols, m1 * n1));
	interSeq = itpp::to_ivec(vec(matrixOfInterleaverSequence, m3 * n3));
	Sequence_Interleaver<double> seq_inter( interSeq );
	seq_inter.set_interleaver_sequence (interSeq);
	//seq_inter.randomize_interleaver_sequence();
	//interSeq = seq_inter.get_interleaver_sequence();
	outputVector = seq_inter.interleave(inputVector);


	//----------------------------------------Output Arguements---------------------------------------
	//For output arguement 1(Interleaved Sequence)
	//Storing the output in a double array to be sent to scilab
	matrixOfSequenceInterleavedSymbols = (double *) malloc( sizeof(double)*outputVector.length());
	for( i=0; i<outputVector.length(); i++ )
		matrixOfSequenceInterleavedSymbols[i] = outputVector(i);


	//Defining the no. of rows and columns in matrix to be sent as output
	if( n1 == 1 )
		row1 = outputVector.length();
	else
		col1 = outputVector.length();	


	//Creating Output Modulated Signal
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, row1, col1, matrixOfSequenceInterleavedSymbols);	 
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}
	free(matrixOfSequenceInterleavedSymbols); 


	/*//For output arguement 2(Interleaver Sequence used)
	//Storing the output in a double array to be sent to scilab
	matrixOfInterleaverSequence = (double *) malloc( sizeof(double)*interSeq.length());
	for( i=0; i<interSeq.length(); i++ )
		matrixOfInterleaverSequence[i] = interSeq(i);


	//Defining the no. of rows and columns in matrix to be sent as output
	if( n1 == 1 )
		row2 = interSeq.length();
	else
		col2 = interSeq.length();	


	//Creating Output Modulated Signal
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, row2, col2, matrixOfInterleaverSequence);	 
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}
	free(matrixOfInterleaverSequence); 	
	*/

	//-------------------------------Returning output arguements to Scilab----------------------------

	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	//AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
	
	ReturnArguments(pvApiCtx);
	return 0;
}
/*===============================================================================*/
}
