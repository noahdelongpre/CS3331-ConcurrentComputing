//-----------------------------------------------------------------------------
//NAME: Noah de Longpre'                         USER ID: nkdelong
//DUE DATE: 12/8/17
//PROGRAM ASSIGNMENT 5
//FILE NAME: thread-main.cpp
//PROGRAM PURPOSE:
//	This starts the Cannibal and Missionary program by reading the input
//	from command line, and creating the BoatThread, Cannibal threads, and
//	the missionary threads. After the program has completed, this Joins
//	all of the threads back together and exits. 
//-----------------------------------------------------------------------------
#include <iostream>
#include "thread.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "ThreadClass.h"
#include <string.h>
#include <stdio.h>


using namespace std;


//-----------------------------------------------------------------------------
//FUCNTION main:
//	This starts the program and reads from command line to start the correct
//	amount of Cannibal and Missionary threads, while also starting the boat
//	thread and boat monitor. 
//PARAMETER USEAGE:
//	argv[1] = Number of cannibals read from command line
//	argv[2] = Number of Missionaries read from command line
//	argv[3] = Number of boat rides until the ride closes
//FUNCTION CALLED:
//	N/A, no custom functions were called.
//-----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{

	int c = atoi(argv[1]); //Cannibals
	int m = atoi(argv[2]); //Missionaries
	int b = atoi(argv[3]); //Boat rides 
	char buf[1000];

	if ( c == 0) 
	{
		c = 8;
	}

	if ( m == 0 )
	{
		m = 8;
	}

	if ( b == 0 )
	{
		b = 5;
	}
	//sprintf(buf, "Cannibals: %d \n Missionaries: %d \n Boat Rides: %d \n", c , m , b);
	//write(1, buf, strlen(buf));
	
	//Print the amounts
	BoatMonitor *boatMon = new BoatMonitor(b);
	BoatThread *boat = new BoatThread(b, boatMon);
	boat->Begin(); //Starts the boat


	Cannibal *cannibals[c];

	for(int i = 0 ; i < c ; i++)
	{
		cannibals[i] = new Cannibal(i, boatMon);
		cannibals[i]->Begin();
	}

	Missionary *missionaries[m];

	for(int i = 0 ; i < m ; i++)
	{
		missionaries[i] = new Missionary(i, boatMon);
		missionaries[i]->Begin();
	}


	//Wait for the threads to finish

	boat->Join();
	for(int i = 0; i < c ; i++)
	{
		cannibals[i]->Join();
	}
	
	for(int i = 0 ; i < m ; i++ )
	{
		missionaries[i]->Join();
	}


}
