//-------------------------------------------------------//
// NAME: Noah de Longpre'             User ID: nkdelong  //
// DUE DATE: 10/17/2017                                  //
// PROGRAM ASSIGNMENT 2                                  //
// FILE NAME: main.c                                     //
// PROGRAM PURPOSE:                                      // 
//    This program implements shared memory by parsing   //
//    numbers from stdin and merge sorting them by       //
//    splitting each merge into its own process,         //
//    therefore currently the merge process concurrently //
//                                                       //
//-------------------------------------------------------//
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>



//----------------------------------------------------------//
// FUNCTION main :                                          // 
//    Creates and ends shared memory before and after merge //
//    Creates the single child process to start the merge   //
//    process by sending the starting and ending indices    //
//    to the merge child using execvp()                     //
// PARAMETER USEAGE :                                       //
//    argc : the count of command line args given.          //
//    argv : the array of all of te command line args       //
// FUNCTION CALLED :                                        //
//    fork() : Creates a child process                      //
//    execvp() : Current process changes to another file    //
//    based on the arguments given to the function          //
//----------------------------------------------------------//
int main(int argc, char *argv[] )
{
  int ShmID;
  key_t Key;
  int sizeish = 0;
  int* array; /*Shared memory Pointer*/
  int i = 0;
  char buf[100];
  char buf1[100];
  char buff[100];
  char printing[1000];
  int num = 0;
  pid_t first; /*First Process*/

  int firstIndex =0;
  int lastIndex =0;
  char* FirstLast[6];

  int index =0;
  char numy[100];

  pid_t parent;
  pid_t leftChild;
  pid_t rightChild;

  sprintf(buff, "Merge Sort with Multiple Processes: \n");
  write(1, buff, strlen(buff));
  
  scanf("%d", &sizeish); /*Size of the file given*/ 
  //Making a key and creating the shared memeory based on the key. 
  Key = ftok("./",'h');
  sprintf(buff, "*** MAIN: shared memory key = %d \n", Key);
  write(1, buff, strlen(buff));
  ShmID = shmget( Key, sizeof(int) * sizeish, IPC_CREAT | 0666);
  sprintf(buff, "*** MAIN: shared memory created\n");
  write(1, buff, strlen(buff));

  if(ShmID < 0)
    {
      exit(1); /*The memory wasn't allocated, so the rest doesn't run*/ 
    }
  array = (int*) shmat(ShmID, NULL, 0);
  sprintf(buff, "*** MAIN: shared memory is attached and is ready to use\n\n");
  write(1, buff, strlen(buff));


  i = 0;
  while(scanf("%d",  &array[i] ) > 0)
    {
      i++; /*Putting all of the values from the file into the the shared memory*/
    }

  sprintf(buff, "Input array for mergesort has %d elements: \n", sizeish);
  write(1, buff, strlen(buff));
  //Printing the file the first time
  strcpy(buff, "");
  for(i =0 ; i < sizeish ; i++)
    {
      sprintf(printing, "%4d", array[i]);
      strcat(buff,printing);
    }

  strcat(buff, "\n");
  write(1, buff, strlen(buff));


  sprintf(buff, "*** MAIN: about to spawn the mergesort process \n");
  write(1, buff, strlen(buff));

  //Creating the argv for the execvp() command
  firstIndex = 0;
  lastIndex = sizeish - 1;
  FirstLast[0] = "./merge";
  FirstLast[1] = "0";
  sprintf(buf, "%d", lastIndex);
  FirstLast[2] = buf;
  sprintf(buf1, "%d", ShmID);
  FirstLast[3] = buf1;
  sprintf(buff, "%d", sizeish);
  FirstLast[4] = buff;
  FirstLast[5] = '\0';

  first = fork(); /*Forking the first child to start*/
  if(first < 0 )
    {
      exit(1);
    }

  else if(first == 0)
    {
      if( execvp("./merge", FirstLast) ) //Start running merge.c
	{
	  printf("Execvp didnt work\n");
	}
	
    }
  
  wait(NULL);// Main waits for the child to finish so it can close the big 

  //After the child returns with the sorted array, print it

  sprintf(buff, "*** MAIN: merged array: \n");
  write(1, buff, strlen(buff));

  for(i =firstIndex +1 ; i <= lastIndex ; i++)
    {
      strcpy(buff, "");
      sprintf(buff, "%4d", array[i]);
      strcat(printing,buff);
    }

  strcat(printing, "\n");
  write(1, printing, strlen(printing));
  
  //End the main shared memory of the sorted array.
  strcpy(buff,"");
  shmdt( (void*) array); //Memory detaches
  sprintf(buff, "*** MAIN: shared memory successfully detached\n");
  write(1, buff, strlen(buff));

  shmctl(ShmID, IPC_RMID, NULL); //Deletes the shared memory
  sprintf(buff, "*** MAIN: shared memory successfully removed\n");
  write(1, buff, strlen(buff));


  sprintf(buff, "*** MAIN: exits\n");
  write(1, buff, strlen(buff));//Program is over, and announces it. 

  return 0;
}
