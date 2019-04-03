
//----------------------------------------------------------- 
//NAME : Noah de Longpre'                        User ID: nkdelong 
//DUE DATE : 12/15/2017                                       
//PROGRAM ASSIGNMENT 6                                    
//FILE NAME : thread.h         
//PROGRAM PURPOSE :                                           
//	The header file for the processor and passer class, general blueprint for thread.cpp
//	Processor is the inner processor that calculates the value to go into C
//	Passer is the thread that passes information along from reading A and B
// -----------------------------------------------------------
#include "ThreadClass.h"
#include <iostream>

using namespace std;

class Processor : public Thread{
		public:
			Processor(int row, int col, int** arr, int UserDef, SynOneToOneChannel* ab, SynOneToOneChannel* le, SynOneToOneChannel* ri, SynOneToOneChannel* dw);

		private:
			int UserDefIndex;
			int rowValue;
			int colValue;
			int** array;

			SynOneToOneChannel* above; //Recieve
			SynOneToOneChannel* left;  //Recieve
			SynOneToOneChannel* right; //Pass
			SynOneToOneChannel* down;  //Pass

			int total;
			
			void ThreadFunc();

};

class Passer : public Thread{
		public:
			Passer(int UserDef, SynOneToOneChannel* pass, int* arr, int len);


		private:
			int UserDefIndex;
			SynOneToOneChannel* passing;
			int* array; //A or B at the index it'll use. 
			int length;
			void ThreadFunc();


};

