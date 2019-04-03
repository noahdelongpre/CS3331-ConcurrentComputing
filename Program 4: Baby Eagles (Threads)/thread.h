// ----------------------------------------------------------- 
// NAME : Noah de Longpre'                        User ID: nkdelong 
// DUE DATE : 11/17/2017                                       
// PROGRAM ASSIGNMENT 4                                     
// FILE NAME : thread.h            
// PROGRAM PURPOSE :                                           
//     Header file for thread.cpp, which implements the babyEagle class.
//     This is the general blueprint for thread.cpp, and includes the
//     prototypes to the methods it will use. 
// -----------------------------------------------------------
#ifndef _thread_h
#define _thread_h
#include "ThreadClass.h"
#include <iostream>

using namespace std;

class babyEagle : public Thread{
	public:
		babyEagle(int who, int pots, int *feeds, int *waiting, int *eaten, int *eating, Mutex *Mu, Semaphore *Food, Semaphore *Mom, Semaphore *Ready, int *calledMom );

	private:
		int who; //Which eagle am i?
		int pots; //How many total pots exist?
		int *feeds; //How many feeds do we have left?
		int *waiting; //Birds in waiting
		int *eaten;
		int *eating;

		int tMax;

		Mutex *Mu;
		Semaphore *Food;
		Semaphore *Mom;
		Semaphore *Ready;

		int *calledMom;

		void ThreadFunc();

		
};

int ready_to_eat(int tMax, int pots, int who, int *eating, int *eaten, int *feeds, int *waiting, Mutex *Mu, Semaphore *Mom, Semaphore *Food, Semaphore *Ready, int *calledMom);
int finish_eating(int pots, int who, int *eating, int *t, int *eaten, int *waiting, Mutex *Mu, Semaphore *Mom, Semaphore *Ready);
void goto_sleep(Semaphore *Mom);
void food_ready(int pots, int tMax, int *t, int *waiting, int* eaten, int* eating, Mutex *Mu, Semaphore *Food, Semaphore *Mom, int *calledMom);
void delay();

#endif