// ----------------------------------------------------------- 
// NAME : Noah de Longpre'                        User ID: nkdelong 
// DUE DATE : 11/17/2017                                       
// PROGRAM ASSIGNMENT 4                                     
// FILE NAME : thread-support.cpp            
// PROGRAM PURPOSE :                                           
//     Contains all of the supporting methods for the Mother Eagle
//     run by thread-main, as well as the methods using during 
//     the baby eagles feed cycle.
//                    
// -----------------------------------------------------------
#include "ThreadClass.h"
#include "thread.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <iostream>


using namespace std;


//---------------------------------------------------------
// FUNCTION ready_to_eat :
//    ready_to_eat is made for the babyEagle to use.
//    When ready is called, the baby looks to see if it has
//    a feeding pot to eat from, if it doesn't, it waits until
//    the last eagle leaves feeding to tell Mom to refill, and then
//    Mom lets in baby eagles from the waiting list equal to the amount of
//    feeding pots.
// PARAMETER USEAGE :
//    tMax = Max value of feeding amounts, pots = number of pots, who = which baby am I
//    *eating = Current amount of baby eagles eating, *eaten = how many pots have been eaten
//    *feeds = amount of feedings left, *waiting = amount of birds waiting to eat, 
//    *Mu = Global mutex for babies and mom, *Mom = Semaphore birds use to signal mom to refill
//    *Food = The waiting Semaphore the babies use while waiting for food. 
// FUNCTION CALLED :
//     No custom functions were called in this method.
//---------------------------------------------------------
int ready_to_eat(int tMax, int pots, int who, int *eating, int *eaten, int *feeds, int *waiting, Mutex *Mu, Semaphore *Mom, Semaphore *Food, Semaphore *Ready, int* calledMom ){
	

	Mu->Lock();  //!!!!!!!!!!!!!!!!!!!!!!!!
	
	char buf[1000];
	char space[100];

	strcpy(space, "");

	for (int i = 0 ; i < who ; i++ )
	{
		strcat(space, " ");
	}
	
	if( (*feeds) > 0)
	{
		sprintf(buf, "%sBaby Eagle %d is ready to eat.\n",space, who);
		write(1, buf, strlen(buf));
 		//I am Ready to eat, and mom is still here. 
	}
	


 	while((*eaten) == pots) //The babies can't leave unless this condition is satisfied, letting only m babies into the feeding area. 
 	{
 		if((*feeds) < 0) // If no feeding cycles are left, because mom left for Vacation
 		{
 			if((*waiting) > 0)
 				Food->Signal(); // Pass the baton and let the others out
 			else
 				Mu->Unlock(); //Otherwise if no one else is waiting on 
 			return -1; //Leave the method as a whole 
 		}
 		(*waiting)++; //If you got here, that means all the food was taken, and you are waiting for Mom to refill.
 		//printf("***calledMom =  %d\n", (*calledMom));
 		//printf("Does calledMom == 0? %d\n", (*calledMom) == 0 );

 		if( (*calledMom) == 0 )
 		{
 			(*calledMom) = 1; //Mom has been called. 

 			//printf("Set calledMom to 1\n");

 			Mu->Unlock();
 			if( (*feeds) != tMax)
 			{
 				//printf("feeds != tMax\n");
 				Ready->Wait();
 			}

 			if( (*feeds) != 0 )
 			{
 				sprintf(buf , "%sBaby eagle %d sees all feeding pots are empty and wakes up the mother.\n",space, who, (*eaten) ); //Print for the ones leaving the 
				write(1, buf, strlen(buf));

				sprintf(buf, "Mother Eagle is awoke by baby eagle %d and starts preparing food. \n", who );
				write(1, buf, strlen(buf)); //Has to print this here because mom doesn't know who wakes her up without passing that back up. 
 			}
 			//printf("***%d flagged mom\n", who);
 			Mom->Signal(); //Tell mom to wake up

 		}
 		else
 		{
 			Mu->Unlock(); //Open to the world if mom has already been called, so others can wait. 
 		}
 		Food->Wait(); //When they baby gets out of the loop, 
 		(*waiting)--;
 	}

 	(*eating)++;
 	(*eaten)++;

	sprintf(buf, "%sBaby eagle %d is eating using feeding pot %d\n", space, who, (*eaten) ); //Print for the ones leaving the 
	write(1, buf, strlen(buf));

	//printf("EATEN IS ___%d___\n", (*eaten));

	if( (*waiting) > 0 )
		Food->Signal(); //If someone else is still waiting, pass the baton to them. 
	else
		Mu->Unlock(); //If nothing else is left waiting, open the Mutex to the world so anything can get in. 

	return 0; //Leaves after its done hanging out



}


//---------------------------------------------------------
// FUNCTION finish_eating :
//    finish_eating is the method the baby eagles run when they have stopped eating.
//    When the last baby leaves the eating zone, they call Mom to come back and
//    refill the food pots for the next wave of baby birds that want to eat.
// PARAMETER USEAGE :
//    pots = number of pots, who = which baby am I,
//    *eating = Current amount of baby eagles eating, *eaten = how many pots have been eaten
//    *t = amount of feedings left,*Mu = Global mutex for babies and mom, 
//    *Mom = Semaphore birds use to signal mom to refill
// FUNCTION CALLED :
//     No custom functions were called in this method.
//---------------------------------------------------------
int finish_eating(int pots, int who, int *eating, int *t, int *eaten, int *waiting, Mutex *Mu, Semaphore *Mom, Semaphore *Ready ){

	Mu->Lock();
	char buf[1000];
	char space[100];

	strcpy(space, "");
	for (int i = 0 ; i < who ; i++ )
	{
		strcat(space, " ");
	}
	
	if( (*eating) > 0 )	//If other babies are still eating
	{
		(*eating)--; //You are no longer eating

		sprintf(buf, "%sBaby Eagle %d finishes eating.\n", space, who);
		write(1, buf, strlen(buf));

	}

	//printf("eaten = %d , pots = %d\n", (*eaten) , pots);
	//printf("does eaten == pots? %d \n does eating == 0? %d \n", (*eaten) == pots, (*eating) == 0);

	if( (*eaten) == pots && (*eating) == 0) //All feeding pots are empty, and no one is eating.
	{
		//printf("***%d Last one out, signals ready\n", who);
		Ready->Signal();
		if( (*waiting) == 0 || (*t) <= 0 )
			Mu->Unlock();
	}

	else
	{
		Mu->Unlock();
	}
	return 1;

}

//---------------------------------------------------------
// FUNCTION goto_sleep :
//    goto_sleep just prints to output that mom is taking a nap,
//    and she waits until a baby signals that they need a refill.
// PARAMETER USEAGE :
//    *Mom = Semaphore birds use to signal mom to refill
// FUNCTION CALLED :
//     No custom functions were called in this method.
//---------------------------------------------------------
void goto_sleep(Semaphore *Mom){

	char buf[1000];

	sprintf(buf, "Mother eagle takes a nap\n");
	write(1, buf, strlen(buf));

	Mom->Wait(); //Tell mom to wait for someone to wake her
}


//---------------------------------------------------------
// FUNCTION food_ready :
//    food ready is what mom uses to prepare the food in the pots,
//    as well as signal all of the children at the end, that they can come out of
//    waiting and eat. During her last feeding cycle she sets *t to -1 or -2
//    -2 = Birds are still inside the Food Semaphore, and need to be cleared out.
//    -1 = No birds are in waiting and won't join, but the current ones are set to eating
//    and leave after they're done eating. 
// PARAMETER USEAGE :
//    tMax = Max value of feeding amounts, pots = number of pots, who = which baby am I
//    *eating = Current amount of baby eagles eating, *eaten = how many pots have been eaten
//    *t = amount of feedings left, *waiting = amount of birds waiting to eat, 
//    *Mu = Global mutex for babies and mom, *Mom = Semaphore birds use to signal mom to refill
//    *Food = The waiting Semaphore the babies use while waiting for food. 
// FUNCTION CALLED :
//     No custom functions were called in this method.
//---------------------------------------------------------
void food_ready(int pots, int tMax, int *t, int *waiting, int* eaten, int* eating, Mutex *Mu, Semaphore *Food, Semaphore *Mom, int *calledMom){ //Mom is making and serving the food. 

	char buf[1000];

	sprintf(buf, "Mother Eagle says \"Feeding (%d)\"\n", abs( (*t) -tMax ) ); 
	write(1, buf, strlen(buf));

	(*eating) = 0; //No one is eating
	(*eaten) = 0; //No one has eaten the newly filled pots
	Food->Signal(); //Signal someone to come and get it.
	(*t)--; //She's one closer to retirement / vacation. 

	(*calledMom) = 0; // Mom hasn't been called, because she's about to go back to sleep. 
}

//---------------------------------------------------------
// FUNCTION delay :
//     delay is used to delay the Mother and babies for a random amount,
//     making the eating cycles more random than if they ran without it. 
// PARAMETER USEAGE :
//     No parameters. 
// FUNCTION CALLED :
//     No custom functions were called in this method.
//---------------------------------------------------------
void delay(){

	srand(time(NULL));


	int i = 0;
	int random = rand() % 30;
	for(int i = 0; i < random; i++) 
	{
		usleep(1000); //Sleeps for 20ms per iteration
	}

	//usleep(100000); //Sleeps for 100ms 
}