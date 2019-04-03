//--------------------------------------------------------
// NAME: Noah de Longpre'               User ID: nkdelong
// DUE DATE: 12/8/2017
// PROGRAM ASSIGNMENT 5
// FILE NAME: boat-monitor.h
// PROGRAM PURPOSE:
// 	The blueprint for the boat-monitor.cpp class.
//	This header file provides the different variables and methods
//	that should be included in the implentation of the class. 
//---------------------------------------------------------
#include "ThreadClass.h"



class BoatMonitor : public Monitor
{

	public:	
		BoatMonitor(int loads); //BoatMonitor *boat

		void BoatReady(int currentLoad);
		void BoatDone();
		void CannibalArrives(int who);
		void MissionaryArrives(int who);




	private:
		Condition *Riding;
		
		Condition *BoatWaiting;
		Condition *CannibalHere;
		Condition *MissionaryHere;
		BoatMonitor *boat; //Pointer to the BoatMonitor
		
		int waiting; // Total number of cannibals and Missionaries waiting to board. 
		int onBoat; //How many are on the boat / error checking. 
		int isBoatReady; //Is the boat ready to be loaded
		int canWait; //Cannibals waiting
		int misWait; //Missionaries waiting
		char types[3]; //Type of people on the boat for printing
		int boarding[3]; //Which number of the cannibal or Missionary boarding



		void ThreadFunc();
		int PickThree(int l); //Picks 3 people to go on the boat, passes which load currently on

};
