// ----------------------------------------------------------- 
// NAME : Noah de Longpre'                        User ID: nkdelong 
// DUE DATE : 11/3/2017                                       
// PROGRAM ASSIGNMENT 3                                        
// FILE NAME : thread.cpp            
// PROGRAM PURPOSE :                                           
//    This is the implementation of the thread.h class.
//    This contains the constructor, as well as the ThreadFunc
//    that runs when Begin() is called from a thread.        
// -----------------------------------------------------------

#include <iostream>
#include "thread.h"
#include <math.h>
#include <unistd.h>
#include "ThreadClass.h"
#include <stdio.h>
#include <string.h>

using namespace std;


//----------------------------------------------------------
// FUNCTION Prefix Constructor :
//   It's the constructor that takes in all the variables,
//   and saves them locally to be used in the ThreadFunc().
// PARAMETER USEAGE : 
//   num : total number of elements in the array
//   k : calculated 2^k number of values
//   gap : the gap between the numbers that need to be added together again, some value 2^i
//   stage : Which run of summation are we in? What row we're adding in now
//   j : index that belongs to that thread to sum with
//   B : the main 2-D global array
// FUNCTION CALLED :
//   No outside functions are called within this.
//----------------------------------------------------------
PrefixSum::PrefixSum(int num, int kk, int g, int stag, int jj, int** array )
{
	n = num;
	k = kk;
	gap = g;
	stage = stag;
	j = jj;
	B = array;

	ThreadName << "PrefixSum" << j << endl;

}

//-------------------------------------------------------
// FUNCTION ThreadFunc : 
//   This is the function that runs when a created thread uses Begin().
//   Specifically, this runs the sum portion of the program, taking in the inputs
//   and calculating if it should copy the value, or if it should add two previous
//   values together and placing them lower in the array.
// PARAMETER USEAGE :
//   No parameters used, all variables used are made in constructor
// FUNCTION CALLED : 
//   No outside functions are called. 
//   Uses Thread:ThreadFunc to start ThreadMentor
//-------------------------------------------------------
void PrefixSum::ThreadFunc()
{
	//What happens when Begin is run. 

	Thread::ThreadFunc();

	char buf[1000];
	sprintf(buf, "     Thread %d created \n", j);
	write(1, buf, strlen(buf));

	//Initailize the Threads with something to run

	if( (int) j - gap < 0 )
	{
		int val = B[stage-1][j];
		B[stage][j] = val;
		sprintf(buf, "     Thread %d copies x[%d]\n", j, j);
		write(1, buf, strlen(buf));
	}
	else
	{
		int val1 = B[stage-1][j];
		int val2 = B[stage-1][(int) (j - gap) ];
		int comp =  val1 + val2;
		B[stage][j] = comp;
		sprintf(buf, "     Thread %d computes x[%d] + x[%d]\n", j, j, j-gap );
		write(1, buf, strlen(buf));
	}

	sprintf(buf, "     Thread %d exits\n", j);
	write(1, buf, strlen(buf));

	Exit();
	
}
