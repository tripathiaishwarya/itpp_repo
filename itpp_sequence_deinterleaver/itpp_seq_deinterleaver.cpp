/*----------------------------------------------------------------------------------------------------------------
Code for Sequence Deinteleaver
Aishwarya Tripathi
June 10,2015
----------------------------------------------------------------------------------------------------------------*/


/*I/O format of the code for scilab engine
* 
* <outputDeinterleavedSymbols> = seq_deinterleaver(<inputInterleavedstream>,<interleaver_depth>)
*
* The code interfaces the function Sequence_Interleaver().deinterleave() of ITPP toolbox with the scilab engine.
*
* ITPP toolbox function description : 
*-----------------------------------
* The input to the deinterleave() function is an interleaved stream and an integer interleaver depth of interleaver sequence, 
* and the output is the deinterleaved vector.
* 
*
* Input/Output Details : 
*----------------------
* It is expected that the user inputs a array of interleaved symbols and an integer interleaver_depth in scilab engine while calling the
* seq_deinterleaver() function and expect the function to return the deinterleaved vector as output in scilab console.
*/

/***********Sample I/O in scilab console******************
-->b=[3 4 1 2 7 8 5 6];
 
-->c=4;
 
-->a=seq_deinterleaver(b,c)
 a  =
 
    1.    2.    3.    4.    5.    6.    7.    8.  
 
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

int itpp_seq_deinterleaver(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;


	//Variable declaration
	int m1=0,n1=0,m2=0,n2=0,i=0,row=1,col=1; 
	int *piAddressVar1 = NULL;
	int *piAddressVar2 = NULL;
	double *matrixOfInputSymbols = NULL;
	double *matrixOfSequenceDeinterleavedSymbols = NULL;
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


	//Checking for a single integer input 
	if( ( m2 != 1 ) || ( n2 != 1) ) 
	{
		Scierror(999, _("%s: Single integer input expected.\n"), fname, 1);
		return 0;
	}



	/*//-------------------------For Third Input Arguement (Interleaver Sequence)----------------------------- 
	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVar3);
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
	
	*/
		
	//-------------------------------Implementation of Sequence Interleaving------------------------------
	//Declaring input and output vectors for interleaving 
	vec inputVector, outputVector;
	//ivec interSeq;	


	//Invoking the PSK modulation function
	
	inputVector = itpp::vec(vec(matrixOfInputSymbols, m1 * n1));
	//interSeq = itpp::to_ivec(vec(matrixOfInterleaverSequence, m3 * n3));

	Sequence_Interleaver<double> seqInter(*in_interleaver_depth);	
	//seqInter.set_interleaver_sequence (interSeq);
	outputVector = seqInter.deinterleave(inputVector, 1);


	//----------------------------------------Output Arguements---------------------------------------
	//Storing the output in a double array to be sent to scilab
	matrixOfSequenceDeinterleavedSymbols = (double *) malloc( sizeof(double)*outputVector.length());
	for( i=0; i<outputVector.length(); i++ )
		matrixOfSequenceDeinterleavedSymbols[i] = outputVector(i);


	//Defining the no. of rows and columns in matrix to be sent as output
	if( n1 == 1 )
		row = outputVector.length();
	else
		col = outputVector.length();	


	//Creating Output Modulated Signal
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, row, col, matrixOfSequenceDeinterleavedSymbols);	 
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}
	free(matrixOfSequenceDeinterleavedSymbols); 


	//-------------------------------Returning output arguements to Scilab----------------------------

	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	ReturnArguments(pvApiCtx);
	return 0;
}
/*===============================================================================*/
}
