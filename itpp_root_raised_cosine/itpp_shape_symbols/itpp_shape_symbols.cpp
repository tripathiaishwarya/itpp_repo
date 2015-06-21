/*----------------------------------------------------------------------------------------------------------------
Code for Root raised cosine, shape symbols
Aishwarya Tripathi
June 10,2015
----------------------------------------------------------------------------------------------------------------*/


/*I/O format of the code for scilab engine
* 
* <outputPulseShape> = itpp_shape_symbols(<inputSymbols>double roll_off_factor, int filter_length, int upsampling_factor)
*
* The code interfaces the function Root_Raised_Cosine<double>(roll_off_factor, filter_length, upsampling_factor).shape_symbols(vec inputSymbols) of ITPP toolbox with the scilab engine.
*
* ITPP toolbox function description : 
*-----------------------------------
* The function shape_symbols() shapes the the input symbols performing upsampling. It 
* uses roll off factor, filter length and upsampling factor to generate the pulse and then shapes input symbols.
* 
*
* Input/Output Details : 
*----------------------
* It is expected that the user inputs input symbols vector of {1,-1,} roll off factor, filter length and upsampling factor in scilab
* engine while calling the itpp_shape_symbols() function and expect the function to return pulse shape vector as output in scilab console.
*/


/***********Sample I/O in scilab console******************
-->b=[-1 1 -1 1 -1]
 b  =
 
  - 1.    1.  - 1.    1.  - 1.  
 
-->c=0.5
 c  =
 
    0.5  
 
-->d=6
 d  =
 
    6.  
 
-->e=8
 e  =
 
    8.  
 
-->a=itpp_shape_symbols(b,c,d,e)
 a  =
 
 
         column 1 to 5
 
  - 0.0010718    0.0027027    0.0058167    0.0069687    0.0053052  
 
         column  6 to 10
 
    0.0008155  - 0.0054688  - 0.0115738  - 0.0139335  - 0.0161333  
 
         column 11 to 15
 
  - 0.0112855    0.0016469    0.0212207    0.0431932    0.0609211  
 
         column 16 to 20
 
    0.0665056    0.0514466    0.0166657  - 0.0441668  - 0.1279351  
 
         column 21 to 25
 
  - 0.2257981  - 0.3239221  - 0.4054572  - 0.4534809  - 0.4533024  
 
         column 26 to 30
 
  - 0.4036410  - 0.3003694  - 0.1527938    0.0212207    0.1976339  
 
         column 31 to 35
 
    0.3500050    0.4540133    0.4908156    0.4585728    0.3558216  
 
         column 36 to 40
 
    0.1968026    0.0053052  - 0.1890183  - 0.3554738  - 0.4674439  
 
 
*/


#include <itpp/comm/pulse_shape.h>
#include <itpp/base/vec.h>


using namespace itpp;


extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include <localization.h>


/*=================================================================*/

int itpp_shape_symbols(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;

	//Variable declaration
	int m1=0,n1=0,m2=0,n2=0,m3=0,n3=0,m4=0,n4=0,row=1,col=1,i=0; 
	int *piAddressVar1 = NULL;
	int *piAddressVar2 = NULL;
	int *piAddressVar3 = NULL;
	int *piAddressVar4 = NULL;
	double *roll_off_factor = NULL;
	double *filter_length= NULL;
	double *upsampling_factor = NULL;
	double *matrixOfInputSymbols = NULL;
	double *matrixOfPulseShape = NULL;
		
	
	//-----------------------Checking the no. of output and input arguements----------------------------
	/*c=foo(a,b)
	  Input arguements=4
	  Output arguements=1*/
	  CheckInputArgument(pvApiCtx, 4, 4) ;
    	  CheckOutputArgument(pvApiCtx, 1, 1) ;

	
	//-----------------------------For First Input Arguement(Input Symbols)---------------------------------- 
	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVar1);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}	
	

	//Checking if input signal is not of complex type
	if ( !isDoubleType(pvApiCtx, piAddressVar1) ||  isVarComplex(pvApiCtx, piAddressVar1) )
    	{
        	Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 1);
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
	

	//Checking if the input signal is BPSK modulated type
	for( i = 0 ; i < m1*n1 ; i++ )
	{
		if( (matrixOfInputSymbols[i] != -1) && (matrixOfInputSymbols[i] != 1) )
		{
			Scierror(999, _("%s: {1, -1} Input Signal Expected.\n"), fname, 1);
			return 0;
		}
	}
	


	//-------------------------For Second Input Arguement (Roll Off factor)----------------------------- 
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
        	Scierror(999, _("%s: Wrong type for input argument #%d: A boolean matrix expected.\n"), fname, 1);
        	return 0;
    	}

    	//Getting input signal
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar2, &m2, &n2, &roll_off_factor);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}

	//Checking input signal is a vector
	if( ( m2 != 1 ) || ( n2 != 1) ) 
	{
		Scierror(999, _("%s: Single input expected.\n"), fname, 1);
		return 0;
	}


	//------------------------------For third arguement(filter length)-------------------------------

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
        	Scierror(999, _("%s: Wrong type for input argument #%d: An integer matrix expected.\n"), fname, 1);
        	return 0;
    	}


	//Retrieve dimensions and data
	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar3, &m3, &n3, &filter_length);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}


	//Checking if filter length is not an integer
	if((*filter_length - floor(*filter_length)) != 0)
	{
		Scierror(999, _("%s: Integer input expected.\n"), fname, 1);
		return 0;
	}


	//Checking for a single integer input 
	if( ( m3 != 1 ) || ( n3 != 1) ) 
	{
		Scierror(999, _("%s: Single integer input expected.\n"), fname, 1);
		return 0;
	}


	//------------------------------For fourth arguement(upsampling factor)-------------------------------

	//Getting address
	sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVar4);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}	


	//Checking if input signal is of boolean type
	if ( !isDoubleType(pvApiCtx, piAddressVar4) ||  isVarComplex(pvApiCtx, piAddressVar4) )
    	{
        	Scierror(999, _("%s: Wrong type for input argument #%d: An integer matrix expected.\n"), fname, 1);
        	return 0;
    	}


	//Retrieve dimensions and data
	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar4, &m4, &n4, &upsampling_factor);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}


	//Checking if upsampling factor is not an integer
	if((*upsampling_factor - floor(*upsampling_factor)) != 0)
	{
		Scierror(999, _("%s: Integer input expected.\n"), fname, 1);
		return 0;
	}


	//Checking for a single integer input 
	if( ( m4 != 1 ) || ( n4 != 1) ) 
	{
		Scierror(999, _("%s: Single integer input expected.\n"), fname, 1);
		return 0;
	}

		
	//-------------------------------Implementation of shape_symbols------------------------------
	//Vectors for storing output
	vec inputVector,outputVector;


	//Converting the input matrix into vector
	inputVector = vec(matrixOfInputSymbols, m1*n1);


	//Invoking get_pulse_shape
	outputVector = Root_Raised_Cosine<double>(*roll_off_factor, (int) *filter_length, (int) *upsampling_factor ).shape_symbols(inputVector); 


	//----------------------------------------Output Arguements---------------------------------------
	//Defining the no. of rows and columns in matrix to be sent as output
	if( n1 == 1 )
		row = outputVector.length();
	else
		col = outputVector.length();	

	//Storing the output in a double array to be sent to scilab
	matrixOfPulseShape = (double *) malloc( sizeof(double)*outputVector.length());
	for(i=0; i < outputVector.length(); i++)
	{
		matrixOfPulseShape[i] = outputVector(i);
	}


	//Creating Output Modulated Signal
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, row, col, matrixOfPulseShape);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}

	free(matrixOfPulseShape);


	//-------------------------------Returning output arguements to Scilab----------------------------

	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	ReturnArguments(pvApiCtx);
	return 0;
}
/*===============================================================================*/
}
