// ----------------------------------------------------------- 
// NAME : Noah de Longpre'                        User ID: nkdelong 
// DUE DATE : 11/3/2017                                       
// PROGRAM ASSIGNMENT 3                                        
// FILE NAME : thread-main.cpp            
// PROGRAM PURPOSE :                                           
//     Program runs the prefix sum algorithm concurrently using 
//     Threads and ThreadMentor, as well as a global array.     
//                    
// -----------------------------------------------------------

#include <iostream>
#include "thread.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "ThreadClass.h" //Actually needed from ThreadMentor
#include <string.h>
#include <stdio.h>

using namespace std;
int **B; //Global 2-D array

//---------------------------------------------------------
// FUNCTION main :
//    This starts the program by initalizing the global 2-D array used for
//    the calculation of the Prefix Sum. For each iteration of summing
//    each thread is assigned to a one of n numbers, and the sum is calculated
//    then placed one row down in the 2-D array
// PARAMETER USEAGE :
//    No parameters used. variables initailized from stdin
// FUNCTION CALLED :
//    No outside functions are called.
//---------------------------------------------------------

int main()
{
	int n = 0;
	char buf[1000];
	char print[1000];
	cin >> n; //Number of elements in the Array
	int x[n];


	int k = log2(n)+1;
	

	B = (int**) malloc( sizeof(int*) * k); // Array of Arrays

	for(int i = 0; i < k ; i++)
	{
		B[i] = (int*) malloc(sizeof(int) * n); //Opening the rest of the 2-D array
	}

	cout << "\n\nConcurrent Prefix Sum Computation" << endl;
	cout << "Number of input data = " << n << endl;
	cout << "Input Array: " << endl;

	strcpy(print, "");
	for(int i = 0 ; i < n ; i++)
	{
		cin >> x[i]; //Reads in the values from the text doc
		strcpy(buf, "");
		sprintf(buf, "%4d", x[i] );
		strcat(print, buf);
	}

	strcat(print, "\n");
	write(1, print, strlen(print)); // Prints the output array

	//First part of main is complete


	//Time to copy x[i] into the first line of B[i][j]
	for(int i = 0; i < n ; i++)
	{
		B[0][i] = x[i]; //Copies X into the first (0th) row of B. 
	}

	cout << endl << endl << endl;

	PrefixSum *array[n]; // Makes n threads to run the sum

	int gap;
	int stage;

	for(int stage = 1; stage < k ; stage++)
	{
		sprintf(buf, "Run %d: \n", stage); //Printing which run we're on
		write(1, buf, strlen(buf));
		
		gap = (int) pow(2, stage-1);
		for(int j = 0 ; j < n ; j++)
		{
			array[j] = new PrefixSum(n, k, gap, stage, j, B ); //Starting a thread to run for a value
			array[j]->Begin(); //Begins the thread
		}
		for(int j = 0 ; j < n ; j++)
		{
			array[j]->Join(); //Collects the threads
		}


		sprintf(buf, "Result after run %d: \n", stage);
		write(1, buf, strlen(buf));

		strcpy(print, "");
		for(int i = 0 ; i < n ; i++)
		{ //Reads in what was just changed
			strcpy(buf, "");
			sprintf(buf, "%4d", B[stage][i] );
			strcat(print, buf);
		}

		strcat(print, "\n");
		write(1, print, strlen(print)); // Prints the output array
		

	}

	sprintf(buf, "Final result after run %d: \n", k-1);
	write(1, buf, strlen(buf));

	strcpy(print, "");
	for(int i = 0 ; i < n ; i++)
	{ //Final Array print
		strcpy(buf, "");
		sprintf(buf, "%4d", B[k-1][i] );
		strcat(print, buf);
	}

	strcat(print, "\n");
	write(1, print, strlen(print)); // Prints the output array
	

	for(int i = 0 ; i < k ; i++)
	{
		free(B[i]); //Free of the intrim arrays
	}
	
	free(B); //Free the main 2-D array

return 0;

}
