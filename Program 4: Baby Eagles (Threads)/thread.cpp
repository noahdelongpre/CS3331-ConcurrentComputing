// ----------------------------------------------------------- 
// NAME : Noah de Longpre'                        User ID: nkdelong 
// DUE DATE : 11/17/2017                                       
// PROGRAM ASSIGNMENT 4                                     
// FILE NAME : thread.cpp           
// PROGRAM PURPOSE :                                           
//     Implementation of thread.h, contains the constructor for the babyEagle class
//     as well as the code that each babyEagle thread runs 
// -----------------------------------------------------------
#include "ThreadClass.h"
#include "thread.h"
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


using namespace std;
//---------------------------------------------------------
// FUNCTION babyEagle :
//    Just the constructor for babyEagle, sets all of the variables passed
//    to local variables that the threads know. 
// PARAMETER USEAGE :
//    tMax = Max value of feeding amounts, pots = number of pots, who = which baby am I
//    *eating = Current amount of baby eagles eating, *eaten = how many pots have been eaten
//    *t = amount of feedings left, *waiting = amount of birds waiting to eat, 
//    *Mu = Global mutex for babies and mom, *Mom = Semaphore birds use to signal mom to refill
//    *Food = The waiting Semaphore the babies use while waiting for food.
//    globalSeed = seed used for generating a random number for delay. 
// FUNCTION CALLED :
//    No custom functions called here. 
//---------------------------------------------------------
babyEagle::babyEagle(int who, int pots, int *feeds, int *waiting, int *eaten, int *eating, Mutex *Mu, Semaphore *Food, Semaphore *Mom, Semaphore *Ready, int *calledMom ){

	this->who = who + 1; //Which baby am I?
	this->pots = pots; //Number of pots used. 
	this->feeds = feeds; //Pointer
	this->waiting = waiting; //Pointer
	this->eaten = eaten; //Pointer
	this->eating = eating; //Pointer

	this->tMax = (*feeds); //Holds the orginal value for t. 

	this->Mu = Mu; //Mutex Lock for whole program
	this->Food = Food; //Waiting for food lock / blocker Semaphore
	this->Mom = Mom; //To send mom the message that she needs to wake up. 
	this->Ready = Ready; //Asking to see if food is ready, only one Eagle 

	this->calledMom = calledMom;
}

//---------------------------------------------------------
// FUNCTION ThreadFunc :
//    This is what every babyEagle thread runs when Begin is called.  
// PARAMETER USEAGE :
//    No parameters passed into this method. 
// FUNCTION CALLED :
//    delay() = delays the thread for a random amount per call.
//    ready_to_eat() = Baby bird is hungry, and wants to eat
//    finish_eating() = baby bird is done eating, if last tell mom to refill
//---------------------------------------------------------
void babyEagle::ThreadFunc(){
	Thread::ThreadFunc();
	srand(time(NULL));

	while(1)
	{
	

		//int tMax, int pots, int who, int *eating, int *eaten, int *feeds, int *waiting, Mutex *Mu, Semaphore *Mom, Semaphore *Food, Semaphore *Ready, bool* calledMom 		
		if(-1 == ready_to_eat(tMax, pots, who, eating, eaten, feeds, waiting, Mu, Mom, Food, Ready, calledMom) ){
			break;
		}

		int random = rand() % 20;
		for(int i = 0; i < random ; i++)
		{
			Thread::Delay();
		}

		if ( -1 == finish_eating(pots, who, eating, feeds, eaten, waiting, Mu, Mom, Ready) ) {
			break;
		}

		for(int j = 0 ; j < random ; j++)
		{
			Thread::Delay();
		}
	}
}


