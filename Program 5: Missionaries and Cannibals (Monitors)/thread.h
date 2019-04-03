//--------------------------------------------------------
// NAME: Noah de Longpre'               User ID: nkdelong
// DUE DATE: 12/8/2017
// PROGRAM ASSIGNMENT 5
// FILE NAME: thread.h
// PROGRAM PURPOSE:
// 	The blueprint for the thread.cpp class.
//	This header file provides the different variables and methods
//	that should be included in the implentation of the classes
//	for the each of the different threads. 
//---------------------------------------------------------
#include <iostream>
#include "ThreadClass.h"
#include "boat-monitor.h"

using namespace std;

class Cannibal : public Thread {
		public:
			Cannibal(int num, BoatMonitor* boat);
			

		private:
			int number; //Which Cannibal are you?
			BoatMonitor* boat;	//Pointer to the Monitor
			void ThreadFunc();
};

class Missionary : public Thread {
		public:
			Missionary(int num, BoatMonitor* boat);
			

		private:
			int number;	//Which Missionary number are you?
			BoatMonitor* boat; // Pointer to the monitor
			void ThreadFunc();
};

class BoatThread : public Thread {
		public:
			BoatThread(int numLoads, BoatMonitor* boat);

		private:
			int loads; //How many total loads does the boat know about? 
			BoatMonitor *boat;
			void ThreadFunc(); 

};