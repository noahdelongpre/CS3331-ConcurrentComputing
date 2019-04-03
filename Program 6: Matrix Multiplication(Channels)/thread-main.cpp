//----------------------------------------------------------- 
//NAME : Noah de Longpre'                        User ID: nkdelong 
//DUE DATE : 12/15/2017                                       
//PROGRAM ASSIGNMENT 6                                    
//FILE NAME : thread-main.cpp        
//PROGRAM PURPOSE :                                           
//    This reads in the values from the file given in at command line.
//    Places them into arrays, and generates the threads and channels 
//    used for communication between the threads. 
// -----------------------------------------------------------
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

#include "ThreadClass.h"
#include "thread.h"

using namespace std;


//-----------------------------------------------------------------------------
//FUNCTION main:
//	brains of the program. Creates threads and channels for the processers to use. 
//PARAMETER USEAGE:
//	No parameters used, all read in from the file passed in. 
//FUNCTION CALLED:
//	N/A, no custom functions called. 
//-----------------------------------------------------------------------------
int main(int argc, char** argv )
{

	int l = 0; //Rows of A
	int m = 0; //Columns of A
	scanf("%d %d", &l, &m);

	int temp = 0;
	
	int** A;
	A = (int**) malloc(sizeof(int*) * l);
	for(int i = 0 ; i < l ; i++)
	{
		A[i] = (int*) malloc(sizeof(int) * m); //Allocate space for the A matrix
	}

	for(int i = 0 ; i < l ; i++ )
	{
		for(int j = 0 ; j < m ; j++)
		{
			scanf("%d", &temp);
			A[i][j] = temp; //Place values into A 
		}
	}

	int u = 0; //Rows of B 
	int v = 0; //Columns of B
	scanf("%d %d", &u, &v);

	temp = 0;

	int** B;
	B = (int**) malloc(sizeof(int*) * v);
	for(int i = 0 ; i < v ; i++)
	{
		B[i] = (int*) malloc( sizeof(int) * u); //Allocate space for the B matrix
	}

	for(int i = 0 ; i < u ; i++)
	{
		for(int j = 0 ; j < v ; j++)
		{
			scanf("%d", &temp);
			B[j][i] = temp; //Reading in B transpose instead. 
			//Makes Columns easier to read
		}
	}
	

	if( m != u ) //Test to see if it can be multiplied together. 
	{
		printf("These matrices cannot be multiplied. \n Exiting...\n");
		printf("m = %d \n u = %d \n", m, u);
		free(B);
		free(A);
		exit(1);
	}

	//Time to make processes. 
	// l = rows
	// v = columns
	

	int** C;
	C = (int**) malloc( sizeof(int*) * l);
	for(int i = 0 ; i < l ; i++ ) //Allocating space for C after they can be multiplied
	{
		C[i] = (int*) malloc( sizeof(int) * v );
	}

	




	SynOneToOneChannel* rowChan[l][v]; //Main channels between the inner processers. 
	SynOneToOneChannel* colChan[l][v];
	int id = 0;
	int rowNext = 0;
	int colNext = 0;



	for(int i = 0 ; i < l ; i++)
	{
		for(int j = 0 ; j < v ; j++)
		{
			id = 100 * i + j + 50; //ID scheme for unique id's for all processes. 
			rowNext = id+1; //One to the right
			colNext = id+100; //One below

			if(j != v-1 ) 
			{
				rowChan[i][j] = new SynOneToOneChannel("", id, rowNext );
			}
			else
			{
				rowChan[i][j] = NULL; //On the edge? Right is NULL
			}

			if(i != l-1)
			{
				colChan[i][j] = new SynOneToOneChannel("", id, colNext );
			}
			else
			{
				colChan[i][j] = NULL; //On the bottom? bottom is NULL
			}
			
		}
	}

	

	SynOneToOneChannel* rowPass[l]; //Special case channels for the passers
	SynOneToOneChannel* colPass[v]; 

	for(int i = 0 ; i < l ; i++)
	{
		rowPass[i] = new SynOneToOneChannel("", i*100 , 50 + (100 * i) );
	}

	for(int j = 0 ; j < v ; j++)
	{
		colPass[j] = new SynOneToOneChannel("", j+1 , 50 + j );
	}

	

	Passer* CRowPasser[l];	//Actually creating the passers
	Passer* CColPasser[v];

	Processor* inner[l][v]; //Creating the inner processers
	int IDPro;

	for(int i = 0 ; i < l ; i++) //Row 
	{
		for(int j = 0 ; j < v ; j++) //Column
		{
			IDPro = 100 * i + j + 50;

			SynOneToOneChannel *above, *below, *left, *right; //Holder channels for easy creation. 

			if (i == 0){//first row
				above = colPass[j];
			}		
			else
			{
				above = colChan[i-1][j]; //Not in the first row
			}
			if(j == 0) //First column
			{
				left = rowPass[i];
			}
			else
			{
				left = rowChan[i][j-1]; //Not in the first column
			}

			right = rowChan[i][j]; //Right is always the same
			below = colChan[i][j]; //Down is always the same,   Either of these could be NULL. 

			inner[i][j] = new Processor(i, j, C, IDPro, above, left, right, below); //Creation
			inner[i][j]->Begin(); //Start the process. 
	

		}


	}


	for(int i = 0 ; i < l ; i++)
	{
		CRowPasser[i] = new Passer(i*100, rowPass[i], A[i], m ); //Creating the Passer Threads for the rows.
		CRowPasser[i]->Begin();
	}

	for(int j = 0 ; j < v ; j++)
	{
		CColPasser[j] = new Passer(j+1, colPass[j], B[j], m );	//Creating the Passer Threads for the columns.
		CColPasser[j]->Begin();
	}

	//Creation is finished. 
	
	for(int i = 0 ; i < l ; i++)
	{
		for(int j = 0 ; j < v ; j++)
		{
			inner[i][j]->Join(); //Trying to join for all of the inner processes
		}
	}


	//Print A
	printf("\n");
	printf("*** From main ***\n");
	printf("Matrix A : %d rows and %d columns\n",l ,m );
	for(int i = 0 ; i < l ; i++)
	{
		for(int j = 0 ; j < m ; j++)
		{
			printf("%6d ", A[i][j]);
		}
		printf("\n");
	}
	printf("\n");


	//Print B
	printf("Matrix B Transpose: %d rows and %d columns\n",v ,u );
	for(int i = 0 ; i < v ; i++)
	{
		for(int j = 0 ; j < m ; j++)
		{
			printf("%6d ", B[i][j]);
		}
		printf("\n");
	}
	printf("\n");


	//Print C
	printf("Matrix C : %d rows and %d columns\n",l ,v );
	for(int i = 0 ; i < l ; i++)
	{
		for(int j = 0 ; j < v ; j++)
		{
			printf("%6d ", C[i][j]);
		}
		printf("\n");
	}


}
