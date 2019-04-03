//-----------------------------------------------------------------------------
//NAME: Noah de Longpre'                             USER ID: nkdelong
//DUE DATE: 12/8/17
//PROGRAM ASSIGNMENT 5
//FILE NAME: thread.cpp
//PROGRAM PURPOSE:
//	This is the implementation of the thread.h class. This includes the 
//	implementation of the boat, Cannibal, and Missionary threads.
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "thread.h"
//#include "boat-monitor.h"

using namespace std;


//-----------------------------------------------------------------------------
//FUNCTION Cannibal:
//	Constructor for the Cannibal Thread.
//PARAMETER USEAGE:
//	who = which number is this thread
//	boaty = a pointer to the boat monitor
//FUNCTION CALLED:
//	N/A, no function called
//-----------------------------------------------------------------------------
Cannibal::Cannibal(int who, BoatMonitor *boaty)
{
	number = who;
	boat = boaty;
}

//-----------------------------------------------------------------------------
//FUNCTION Missionary:
//	Constructor for the Missionary Thread.
//PARAMETER USEAGE:
//	who = which number is this thread
//	boaty = a pointer to the boat monitor
//FUNCTION CALLED:
//	N/A, no function called
//-----------------------------------------------------------------------------
Missionary::Missionary(int who, BoatMonitor *boaty)
{
	number = who;
	boat = boaty;
}

//-----------------------------------------------------------------------------
//FUNCTION BoatThread:
//	Constructor for the Boat Thread.
//PARAMETER USEAGE:
//	who = which number is this thread
//	maxLoads = the orginal number passed in for the max number of loads the boat
//	is supposed to do before it retires.
//FUNCTION CALLED:
//	N/A, no function called
//-----------------------------------------------------------------------------
BoatThread::BoatThread(int maxLoads, BoatMonitor *boaty)
{
	loads = maxLoads;
	boat = boaty;
}


//-----------------------------------------------------------------------------
//FUNCTION Cannibal::ThreadFunc:
//	This runs the Cannibal thread. It runs a while loop that waits for a random
//	amount of time before the thread is allowed to arrive at the boat again. 
//PARAMETER USEAGE:
//	N/A
//FUNCTION CALLED:
//	rand()
//-----------------------------------------------------------------------------
void Cannibal::ThreadFunc()
{
	Thread::ThreadFunc();
	srand(time(NULL));
	int ran;
	
	char buf[100];
	char space[50];
	
	strcpy(space, "");
	for(int i = 0 ; i < number-1 ; i++)
		strcat(space, " ");
	
	
	
	sprintf(buf, "%s Cannibal %d starts\n", space, number);
	write(1, buf, strlen(buf));
	
	while(1)
	{
		//ran = rand() % 100;
		//for(int i = 0 ; i < ran ; i++)
			//Thread::Delay();

		boat->CannibalArrives(number); //Cannibal arrives to take the boat, gives number
	}

}


//-----------------------------------------------------------------------------
//FUNCTION Missionary::ThreadFunc:
//	This runs the Missionary thread. It runs a while loop that waits for a random
//	amount of time before the thread is allowed to arrive at the boat again. 
//PARAMETER USEAGE:
//	N/A
//FUNCTION CALLED:
//	rand()
//-----------------------------------------------------------------------------
void Missionary::ThreadFunc()
{
	Thread::ThreadFunc();
	srand(time(NULL));
	int ran;
	
	char buf[100];
	char space[50];
	
	strcpy(space, "");
	for(int i = 0 ; i < number-1 ; i++)
		strcat(space, " ");
	
	
	
	sprintf(buf, "%s Missionary %d starts\n", space, number);
	write(1, buf, strlen(buf));

	while(1)
	{
		//ran = rand() % 100;
		//for(int i = 0 ; i < ran ; i++)
			//Thread::Delay();

		boat->MissionaryArrives(number); //Missionary arrives to take boat, gives number
	}
}



//-----------------------------------------------------------------------------
//FUNCTION BoatThread::ThreadFunc:
//	This runs the BoatThread thread. This controls what the boat is doing phyiscally,
//	unlike the monitor that controls all of the internals of the boat.
//	BoatReady tells the waitng people that the boat is ready to be boarded again.
//	BoatDone kicks everyone off so it can reload the boat. 
//PARAMETER USEAGE:
//	N/A
//FUNCTION CALLED:
//	rand()
//-----------------------------------------------------------------------------
void BoatThread::ThreadFunc()
{
	Thread::ThreadFunc();
	srand(time(NULL));
	int ran;
	
	char buf[1000];
	

	for(int i = 0 ; i < loads ; i++)
	{
		//Thread::Delay();
		sprintf(buf, "***** The boat is ready\n");
		write(1, buf, strlen(buf));
		boat->BoatReady(i);
		//Thread::Delay();
		boat->BoatDone();
		//Print that boat load is done
	}
	
	//for(int i = 0 ; i < 200 ; i++)
		//Thread::Delay();
	
	//Print all of the crossings have completed
	sprintf(buf, "MONITOR: %d crosses have been made.\n", loads);
	write(1, buf, strlen(buf));
	sprintf(buf, "MONITOR: This river cross is closed indefinitely for renovation.\n");
	write(1, buf, strlen(buf));
	
	
	exit(0);
}


