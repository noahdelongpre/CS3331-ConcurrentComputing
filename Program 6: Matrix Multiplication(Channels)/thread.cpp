//----------------------------------------------------------- 
//NAME : Noah de Longpre'                        User ID: nkdelong 
//DUE DATE : 12/15/2017                                       
//PROGRAM ASSIGNMENT 6                                    
//FILE NAME : thread.cpp
//PROGRAM PURPOSE :                                           
//	The implementation of the thread.h class. 
//	Processor is the inner processor that calculates the value to go into C and passes it along
//	Passer is the thread that passes information along from reading A and B
// -----------------------------------------------------------
#include <string.h>
#include <stdio.h>
#include "thread.h"

using namespace std;



//-----------------------------------------------------------------------------
//FUNCTION Processor:
//	The constructor for the Processor thread.
//PARAMETER USEAGE:
//	row: The row index for where it puts its total into matrix C
//	col: The column index for where it puts its total into matrix C
//	arr: The C matrix passed in, instead of it being global.
//	UserDef: The UserDefinedThreadID calculated and passed in
//	ab: The channel above the processor
//	le: The channel left the processor
//	ri: The channel right the processor
//	dw: The channel down the processor
//FUNCTION CALLED:
//	N/A
//-----------------------------------------------------------------------------
Processor::Processor(int row, int col, int** arr, int UserDef, SynOneToOneChannel* ab, SynOneToOneChannel* le, SynOneToOneChannel* ri, SynOneToOneChannel* dw)
{
	rowValue = row;
	colValue = col;
	array = arr;
	UserDefinedThreadID = UserDef;

	//SynChannels
	above = ab;
	left = le;
	right = ri;
	down = dw;

	total = 0;
}


//-----------------------------------------------------------------------------
//FUNCTION Passer:
//	The constructor for the Passer thread
//PARAMETER USEAGE:
//	UserDef: The UserDefinedThreadID calculated and passed in
//	pass: The channel in which passer will keep sending too
//  arr: The chunk of the array ripped from A or B to read and pass. 
//	len: the length of the array passed in, to it knows how long to read for
//FUNCTION CALLED:
//	N/A
//-----------------------------------------------------------------------------
Passer::Passer(int UserDef, SynOneToOneChannel* pass, int* arr, int len)
{
	UserDefinedThreadID = UserDef;
	passing = pass;
	array = arr;
	length = len;

}

//-----------------------------------------------------------------------------
//FUNCTION Processor::ThreadFunc():
//	The working part of the Processor thread, what actually does the work.
//	Receives its information from the channels, multiplies the numbers, and adds
//	that number 
//PARAMETER USEAGE:
//	N/A no parameters.
//FUNCTION CALLED:
//	N/A no custom 
//-----------------------------------------------------------------------------
void Processor::ThreadFunc()
{
	Thread::ThreadFunc();


	int aboveint = 0; //temp variables for the recieved ints
	int leftint = 0;
	total = 0; //make sure this starts at zero.
	char buf[1000];

	while(1) //Until the information is out, and the thread returns. 
	{

		above->Receive(&aboveint, sizeof(int)); //Pull from above and left
		left->Receive(&leftint, sizeof(int));

		sprintf(buf, "      Thread P[%d,%d] received %d from above and %d from left\n", rowValue, colValue, aboveint, leftint);
		write(1, buf, strlen(buf)); //Print what you pulled

		if(aboveint == -1 && leftint == -1) //If they are both -1, time to leave. 
		{
			//End of the road
			array[rowValue][colValue] = total; //Set C

			sprintf(buf, "      Thread P[%d,%d] received EOD, saved result %d and terminated\n", rowValue, colValue, total);
			write(1, buf, strlen(buf));


			if(right != NULL)
				right->Send(&leftint, sizeof(int)); //Pass along -1
			if(down != NULL)
				down->Send(&aboveint, sizeof(int)); //If you can

			return; //And leave


		}

		total = total + (aboveint * leftint);

		if(right != NULL)
			right->Send(&leftint, sizeof(int)); //Pass along the information used
		if(down != NULL)
			down->Send(&aboveint, sizeof(int)); //In the correct direction
		
	}
	
}


//-----------------------------------------------------------------------------
//FUNCTION Passer::ThreadFunc():
//	The working part of the passer thread.
//	Takes the values from 0 up to the length of the array and sends them to a channel.
//	When it runs out of room it sends a -1 and exits. 
//PARAMETER USEAGE:
//	N/A
//FUNCTION CALLED:
//	N/A
//-----------------------------------------------------------------------------
void Passer::ThreadFunc()
{
	Thread::ThreadFunc();

	char buf[1000];

	for(int i = 0 ; i < length ; i++)
	{		
		passing->Send(&array[i], sizeof(int));
		sprintf(buf, "Passing Thread rc[%d] sent %d \n", UserDefinedThreadID, array[i]);
		write(1, buf, strlen(buf));
	}
	length = -1;
	passing->Send(&length, sizeof(int));
	
	return;
	
}

