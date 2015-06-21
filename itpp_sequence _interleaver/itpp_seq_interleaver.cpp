/*----------------------------------------------------------------------------------------------------------------
Code for Sequence Inteleaver
Aishwarya Tripathi
June 10,2015
----------------------------------------------------------------------------------------------------------------*/


/*I/O format of the code for scilab engine
* 
* <outputInterleavedSymbols> = itpp_seq_interleaver(<inputBitstream>,<interleaver_depth>)
*
* The code interfaces the function Sequence_Interleaver().interleave() of ITPP toolbox with the scilab engine.
*
* ITPP toolbox function description : 
*-----------------------------------
* The input to the interleave() function is an vector and an integer interleaver depth of interleaver sequence, 
* and the output is the interleaved vector.
* 
*
* Input/Output Details : 
*----------------------
* It is expected that the user inputs a array of input symbols and an integer interleaver_depth in scilab engine while calling the
* itpp_seq_interleaver() function and expect the function to return the interleaved vector as output in scilab console.
*/

/***********Sample I/O in scilab console******************
-->b=[1 2 3 4 5 6 7 8]
 b  =
 
    1.    2.    3.    4.    5.    6.    7.    8.  
 
-->c=4
 c  =
 
    4.  
 
-->a=itpp_seq_interleaver(b,c)
 a  =
 
    3.    4.    1.    2.    7.    8.    5.    6.  
 
*/



#include <itpp/base/vec.h>
#include <itpp/comm/interleave.h>


using namespace itpp;


extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include <localization.h>


/*=================================================================*/

int itpp_seq_interleaver(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;


	//Variable declaration
	int m1=0,n1=0,m2=0,n2=0,i=0,row1=1,col1=1; 
	int *piAddressVar1 = NULL;
	int *piAddressVar2 = NULL;
	double *matrixOfInputSymbols = NULL;
	double *matrixOfSequenceInterleavedSymbols = NULL;
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


	//------------------------------For second arguement(In Interleaver depth)-------------------------------

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


	//Checking if m is not an integer
	if((*in_interleaver_depth - floor(*in_interleaver_depth)) != 0)
	{
		Scierror(999, _("%s: Integer input expected.\n"), fname, 1);
		return 0;
	}


	//Checking if m is greater than zero
	if(*in_interleaver_depth <= 0)
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

		
	//-------------------------------Implementation of Sequence Interleaving------------------------------
	//Declaring input and output vectors for interleaving 
	vec inputVector, outputVector;
	inputVector = itpp::vec(vec(matrixOfInputSymbols, m1 * n1));


	//Invoking the sequence interleaving function
	Sequence_Interleaver<double> seq_inter( *in_interleaver_depth );
	seq_inter.randomize_interleaver_sequence();
	outputVector = seq_inter.interleave(inputVector);


	//----------------------------------------Output Arguements---------------------------------------
	//For output arguement (Interleaved Sequence)
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


	//-------------------------------Returning output arguements to Scilab----------------------------

	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;	
	ReturnArguments(pvApiCtx);
	return 0;
}
/*===============================================================================*/
}
