// ----------------------------------------------------------- 
// NAME : Noah de Longpre'                        User ID: nkdelong 
// DUE DATE : 11/17/2017                                       
// PROGRAM ASSIGNMENT 4                                     
// FILE NAME : thread-main.cpp            
// PROGRAM PURPOSE :                                           
//     Runs the mother thread of the babyEagles program. Mom waits
//     until a baby needs food, and refills the pots so the others can eat. 
//     when mom leaves the babies that can eat will, and all others leave.
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
// FUNCTION main :
//    This is the mother bird thread, as well as general setup
//    for the thread pointers and passed variables,
//    It also deals with readying from command line to tell the program
//    how many pots, babies, and feeding cycles this iteration should have. 
// PARAMETER USEAGE :
//    argv = [1] = Number of feeding pots read from command line
//           [2] = Number of baby Eagles in the nest
//           [3] = Number of feeding cycles mom will do before she leaves. 
// FUNCTION CALLED :
//     delay() = delays the thread for a random amount per call.
//     goto_sleep() = Tells mom to go to sleep so she can wait for the next feeding cycle.
//     food_ready() = Mom resets variables, and tells the kids to stop waiting. 
//---------------------------------------------------------
int main(int argc, char const *argv[])
{ 

	

	Mutex *Mu; // Mutex for the system
	Semaphore *Food; //Waiting list for the babies that are waiting for food
	Semaphore *Mom; //Signals mom to wake up
	Semaphore *Ready; //Mom signals food is ready to eat

	

	int m = atoi(argv[1]);
	int n = atoi(argv[2]);
	int z = atoi(argv[3]);
	int tMax = atoi(argv[3]);

	

	char buf[1000];
	

	if(m == 0)
		m = 10;
	if(n == 0)
		n = 10; //Check for zero's in the input
	if(z == 0)
	{
		z = 10;
		tMax = 10;
	
	}
		

	int *t = &z;

	Mu = new Mutex();
	Food = new Semaphore("Food", 0);
	Mom = new Semaphore("Mom", 0);
	Ready = new Semaphore("Ready", 0 );

	

	int *waiting = (int*) malloc(sizeof(int));
	(*waiting) =0;

	int *eaten = (int*) malloc(sizeof(int));
	(*eaten) = m;

	int *eating = (int*) malloc(sizeof(int));
	(*eating) = 0; //Amount of birds 

	int *calledMom = (int*) malloc(sizeof(int));
	(*calledMom) = 0; //Flag for if mom has been called to ready the food



	sprintf(buf, "MAIN: There are %d baby eagles, %d feeding pots, and %d feedings. \n", n, m, (*t) );
	write(1, buf, strlen(buf));


	//Make babies

	babyEagle *array[n];
	sprintf(buf, "MAIN: Game starts!!!!!\n");
	write(1, buf, strlen(buf));

	
	for(int i = 0 ; i < n ; i++)
	{

		array[i] = new babyEagle(i, m, t, waiting, eaten, eating, Mu, Food, Mom, Ready, calledMom ); //Create the babies
		array[i]->Begin(); //Start the thread
	}



	srand(time(NULL));	

	while((*t) > 0)
	{
		

		goto_sleep(Mom);


		food_ready(m, tMax , t, waiting, eaten, eating, Mu, Food, Mom, calledMom);
		(*calledMom) = 0;

	}
	goto_sleep(Mom);
	(*t)--;
	sprintf(buf, "Mother Eagle retires after serving %d feedings. Game Ends!!!\n", tMax);
	write(1, buf, strlen(buf) );

	Food->Signal(); //Let the last one out, or it does nothing by letting something out. 

	Mu->Unlock();
	for(int i = 0 ; i < n ; i++){
		//printf("Bird %d joined back with mom\n", i);
		array[i]->Join(); //Wait for the threads 
	}

	//Mu->Unlock();
	
	return 0;
}
