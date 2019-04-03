//--------------------------------------------------------
// NAME: Noah de Longpre'               User ID: nkdelong
// DUE DATE: 12/8/2017
// PROGRAM ASSIGNMENT 5
// FILE NAME: boat-monitor.cpp
// PROGRAM PURPOSE:
// 	Boat-monitor.cpp is the brains of the whole boat operation happens.
// 	This controls the Cannibals and Missionaries that get on the boat, 
// 	as well as when the boat is ready and when it is done.
// 	This .cpp implements the blueprints laid out in the boat-monitor.h
// 	header file also submitted with this program.
//---------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"
#include <time.h>


//----------------------------------------------------
//FUNCTION: BoatMonitor
//	This is the constructor for the boat monitor,
//	it saves all of the information laid out in the header file
//	to actual locations in memory for it to be used during runtime.
//PARAMETER USEAGE:
//	loads: Originally the max number of loads the boat has to do.
//FUNCTION CALLED:
//	None / N/A
//-----------------------------------------------------
BoatMonitor::BoatMonitor(int loads) : Monitor("Boat", HOARE) //BoatMonitor *boaty
{


	//boat = boaty; //Pointer to the boat Monitor. 


	waiting = 0; //How many total people are waiting
	onBoat =0; //How many are on the boat already
	isBoatReady = 1; //Yes its Ready
	canWait = 0; //Cannibals waiting
	misWait = 0; //Missionaries waiting

	Riding = new Condition("Riding");
	BoatWaiting = new Condition("BoatWait");
	CannibalHere = new Condition("CanHere");
	MissionaryHere = new Condition("MisHere");
	char types[3];
	int boarding[3];

}


//---------------------------------------------------------
//FUCNTION BoatReady
//	This function is the first monitor function that the boat thread runs.
//	The function checks in a while loop if there exists a correct
//	boat load of people to cross the river.
//	If one exists, the boat releases the waiting people, and runs the boat.
//PARAMETER USAGE:
//	currentLoad: Is passed from the boat thread, on what the current load
//	is, and is meant for printing from the monitor.
//FUNCTION CALLED:
//	PickThree(currentLoad)
//		It passes the currentLoad to another function that checks
//		a random sequence to see if a correct boat load exists.
//-------------------------------------------------------
void BoatMonitor::BoatReady(int currentLoad)
{
	MonitorBegin();
	char buf[1000];	
	int picked = 0;
	int counter = 0;
	
	


 	while(1){
		picked = PickThree(currentLoad);
		//printf("************************What is picked: %d \n", picked);
		if(picked == 1)
		{
			counter = 0;
			break;
		}
		
		if(counter > 10)
		{
			counter = 0;
			MonitorEnd();
			MonitorBegin();
			
		}
		//printf("Counter = %d \n", counter);
		counter++;
	} 
	
	//PickThree(currentLoad);

	if(onBoat < 3) 
	{
		isBoatReady = 1; //Still ready to load
		BoatWaiting->Wait();

	}

	isBoatReady = 0; //Boat is full, and is not ready to load

	sprintf(buf, "***** Boat Load(%d): Passenger list (%c%d, %c%d, %c%d)\n", currentLoad +1, types[0], boarding[0], types[1], boarding[1], types[2], boarding[2]);
	write(1, buf, strlen(buf));




	MonitorEnd();
}


//---------------------------------------------------------------------
//FUNCTION BoatDone:
//	This runs when the boat has finished its ride across the river
//	and kicks everyone that was on the boat, off the boat.
//PARAMETER USEAGE: 
//	N/A, no parameters
//FUNCTION CALLED:
//	N/A, no custom functions called.
//-------------------------------------------------------------------
void BoatMonitor::BoatDone()
{
	MonitorBegin();

	for(int i = 0 ; i< 3 ; i++)
		Riding->Signal(); //Kick everyone currently on the boat, off. 


	MonitorEnd();

}




//-----------------------------------------------------------------
//FUNCTON CannibalArrives:
//	This runs when the cannibal thread is ready to board the boat.
//	He comes in, waits, and is released when the boat has
//	a correct number of people to form a safe boat load over the river.
//PARAMETER USEAGE:
//	who: The cannibal thread passes its number down so that it may be printed.
//	For example, if cannibal 4 arrives, it needs to pass '4' down so it may be 
//	printed inside the monitor. 
//FUCNTION CALLED:
//	N/A, no custom functions called. 
//---------------------------------------------------------------
void BoatMonitor::CannibalArrives(int who)
{
	//who is the number of which cannibal arrived.

	MonitorBegin();

	char space[100];
	char buf[1000];

	strcpy(space, "");
	for(int i = 0; i < who ; i++)
		strcat(space, " "); //Load space with the spaces for this cannibal

	sprintf(buf, "%s Cannibal %d arrives\n", space, who);
	write(1, buf, strlen(buf));
	
	waiting++; //I'm now waiting
	canWait++; //I'm also a cannibal waiting
	//printf("mis wait = %d \n can wait = %d \n waiting = %d \n", misWait, canWait, waiting);		
		
	CannibalHere->Wait(); //Wait for someone to let me go

	waiting--; //No longer waiting
	canWait--; //No longer a cannibal waiting
	

	onBoat++; //I'm now getting on the boat
	//printf("onBoat? = %d \n", onBoat);
	//Testing to see which member of the boat I am.

	if(onBoat == 1)
	{
		types[0] = 'c'; //I'm a cannibal getting in the first spot in the boat
		boarding[0] = who; //What cannibal am I?
	}
	if(onBoat == 2)
	{
		types[1] = 'c'; //Cannibal taking the second spot
		boarding[1] = who;
	}
	if(onBoat == 3) //Last person to get on the boat
	{
		types[2] = 'c'; //Cannibal taking the third spot. 
		boarding[2] = who;
		BoatWaiting->Signal(); //Telling the boat to go
	}

	Riding->Wait(); //All processes wait here while the boat rides across
	onBoat--; //Each Can or Mis will lower this as they get off the boat

	MonitorEnd(); //Leave and continue


}



//-----------------------------------------------------------------------------
//FUNCTION MissionaryArrives:
//	Works exactly like Cannibal arrives, only with the other type of
//	passenger, the Missionary. Waits until the boat releases it when 
//	a safe boat trip exists.
//PARAMETER USEAGE:
//	who: Which number thread is this Missionary, passed down from that 
//	missionary thread.
//FUCNTION CALLED:
//	N/A, no custom function called. 
//-----------------------------------------------------------------------------
void BoatMonitor::MissionaryArrives(int who)
{
	MonitorBegin();

	char space[100];
	char buf[1000];

	strcpy(space, "");
	for(int i = 0 ; i < who ; i++)
		strcat(space, " ");

	sprintf(buf, "%s Missionary %d arrives\n", space, who);
	write(1, buf, strlen(buf));

	//Gotta wait
	waiting++;
	misWait++;
	//printf("mis wait = %d \n can wait = %d \n waiting = %d \n", misWait, canWait, waiting);
		
	MissionaryHere->Wait();
	//Done waiting, someone released me
	waiting--;
	misWait--;

	onBoat++; //Got on the boat
	//printf("onBoat? = %d \n", onBoat);

	if(onBoat == 1)	//First one on
	{
		types[0] = 'm';
		boarding[0] = who;
	}
	if(onBoat == 2)//Second one on
	{
		types[1] = 'm';
		boarding[1] = who;
	}
	if(onBoat == 3)//Third one on
	{
		types[2] = 'm';
		boarding[2] = who;
		BoatWaiting->Signal();
	}

	Riding->Wait();
	onBoat--;

	MonitorEnd();

}
//-----------------------------------------------------------------------------
//FUNCTION PickThree:
//	A custom function made to pick three people to get on the boat.
//	It starts with a random number between 0 and 2. Based on which one,
//	it checks one of the 3 safe boat rides. By making it random, it
//	doesn't always check the same boat ride every time, which would make
//	one boat more likely than the others.
//PARAMETER USEAGE:
//	l : currentLoad passed to this so it can print things from the MONITOR.
//FUNCTION CALLED:
//	rand()
//-----------------------------------------------------------------------------
int BoatMonitor::PickThree(int l)
{	//Randomly pick 3 people who are waiting and try to board



	int rando = rand() % 3;
	char buf[100];
	int temp;
	
	if(onBoat > 2)
		return 1;
	
	for(int i = 0 ; i < 3 ; i++)
	{ //Checks through all three boat situations
		if(rando == 0) //3 cannibals
		{
			if(canWait >= 3)
			{
				CannibalHere->Signal();
				CannibalHere->Signal();
				CannibalHere->Signal();
				
				sprintf(buf, "MONITOR(%d): 3 Cannibals are selected.\n", l+1);
				write(1, buf, strlen(buf));
				
				return 1; //Don't continue the loop, you found the boat. 
			}
		}
		else if(rando == 1) //3 missionaries
		{
			if(misWait >= 3) //If more than two missionaries are waiting
			{
				MissionaryHere->Signal();
				MissionaryHere->Signal();
				MissionaryHere->Signal();
			
				sprintf(buf, "MONITOR(%d): 3 Missionaries are selected.\n",  l+1);
				write(1, buf, strlen(buf));	
				
				return 1; //Don't continue the loop, you found the boat. 
			}
		}
		else if(rando == 2) //2 Missionaries and a Cannibal
		{
			if(misWait >= 2 && canWait >= 1) //If the correct amount is waiting
			{
				MissionaryHere->Signal();
				MissionaryHere->Signal();
				CannibalHere->Signal();
				
				sprintf(buf, "MONITOR(%d): 2 Missionaries and a Cannibal are selected.\n", l+1);
				write(1, buf, strlen(buf));
				
				return 1; //Don't continue the loop, you found the boat. 
			}
		}
		//If the one rando happened to be also wasn't true, pick a new rando and go. 
		rando = (rando + 1) % 3; // This way it increments through 3 options in a different order
		//Based on the first number. 
		
	}
	
	return 0;
}

