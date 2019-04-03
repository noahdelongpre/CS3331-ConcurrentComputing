// ----------------------------------------------------------- 
// NAME : Noah de Longpre'                        User ID: nkdelong 
// DUE DATE : 11/3/2017                                       
// PROGRAM ASSIGNMENT 3                                        
// FILE NAME : thread.h            
// PROGRAM PURPOSE :                                           
//    The header file for thread.cpp. This is the general blueprint
//    for the thread.cpp file, and what should be implemented. 
// -----------------------------------------------------------

#ifndef _thread_h
#define _thread_h
#include "ThreadClass.h"
using namespace std;

class PrefixSum : public Thread
{
	public:
		PrefixSum(int num, int kk, int g, int stag, int jj, int** array );

	private:
		int n;
		int k;
		int gap;
		int stage;
		int j;
		int** B;
		void ThreadFunc();

};
#endif
