//------------------------------------------------------//
// NAME: Noah de Longpre'             User ID: nkdelong //
// DUE DATE: 10/20/2017                                 //
// PROGRAM ASSIGNMENT 2                                 //
// FILE NAME: merge.c                                   //
// PROGRAM PURPOSE :                                    //
//   This program is ment to be used as a child process //
//   to the accompanying main.c included. This part of  //
//   the program takes the input handed down to it,     //
//   splits it down into 2 other processes, that        //
//   continues to break it down into sortable pieces    //
//   until it binary sorts the partitions together.     //
//------------------------------------------------------//
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

//---------------------------------------------------------//
// FUNCTION binarySearch :                                 //
//    Slightly modified binarySearch that returns the      //
//    position of where a value should be, instead of      //
//    finding the position of something in the list.       //
// PARAMETER USEAGE :                                      //
//    value : The value that its looking to place in a     //
//      position.                                          //
//    left : The lowest bound/index for the search         //
//    right : The highest bound/index for the search       //
//    array : A pointer to the shared memory array that it //
//      searches through                                   //
// FUNCTION CALLED :                                       //
//    binarySearch : Calls itself recursively to find the  //
//      position of the value by cutting the search area   //
//      in half every iteration.                           //
//---------------------------------------------------------//
int binarySearch(int value, int left, int right, int* array)
{
  int mid = 0;
  mid = left + (right - left) / 2;
      
  if(left == right){ //You're at the last value
    if (value < array[left]){
      return left; //It needs to be here
    } else {
      return left +1; // Or it's larger. 
    }
  }

  if(value < array[mid]) // Keep going on the left side
    return binarySearch(value, left, mid, array);

  return binarySearch(value, mid + 1, right, array);
  // If the value is larger then cut in half and continue on the right.     
  
  

  return -1;
}


//--------------------------------------------------------//
// FUNCTION main :                                        //
//    Takes in shared memory, and the indecies it should  //
//   be looking between to merge sort. Left is the small  //
//   index, while right is the large index. If it has two //
//   or more characters orginally, it splits again. When  //
//   the children return, it begins sorting using the     //
//   binary search and a temp shared memory.              //
// PARAMETER USEAGE :                                     //
//    argv[1] : left index for merge sort                 //
//    argv[2] : right index for merge sort                //
//    argv[3] : Shared memory ID for the main array       //
//    argv[4] : One time pass down for the first process  //
//      Its the size of the whole shared memory array     //
// FUNCTION CALLED :                                      //
//   binarySearch : Called to find the position once the  //
//    children had returned with two sorted lists.        //
//--------------------------------------------------------//
int main (int argc, char* argv[] )
{
  int ShmID = atoi(argv[3]);
  int* array;
  int left = atoi(argv[1]);
  int right = atoi(argv[2]);
  int mid = 0;
  int sizeish = atoi(argv[4]);

  char buf[1000];
  char print[1000];
  char bufLeft[100]; 
  char bufRight[100];
  char sortBuf[100];
  char endBuf[1000];

  char* leftArr[5]; /*argv for the new children*/
  char* rightArr[5];
  pid_t lefty; //PID for the left and right children being made
  pid_t righty;

  int temp=0;
  int i = 0; //used as an index in for loops
  key_t key; //temp shared memory key
  int ShmIDTEMP; //Shared memory ID for the temp shared memory. 
  int* tempArr; //Temp shared memory array between the sorting processes
  pid_t tempPro;
  int offset; //Offset if its on the left or the right of the sorted array
  int binaryvalue; //Value given back from the binary search
  int bin = 0; //Actual placement value in big array

  pid_t self = getpid();
  pid_t parent = getppid();


  
  array = (int*) shmat(ShmID, NULL, 0 ); /*Attach the shared mem*/
  if(ShmID < 0 )
    {
      printf("Didn't attach to the shared memory\n");
      exit(1);
    }

  if( left + right + 1 == sizeish) //First time special case print
    {
      sprintf(print, "   ### M-PROC(%d): entering with a[%d..%d]\n", self,left, right );
      write(1, print, strlen(print));
    }
  else 
    { //Everyone else's print
      sprintf(print, "   ### M-PROC(%d) created by M-PROC(%d): entering with a[%d..%d]\n   ",
	      self,parent, left, right );

      for(i =left ; i <= right ; i++)
	{
	  sprintf(buf, "%4d", array[i]);
	  strcat(print,buf); //Printing all the digits the process has control over
	}

      strcat(print, "\n");
      write(1, print, strlen(print));

    }
  
  
  if( right - left > 1 )
    {
      /*Size is not 2, keep splitting*/

      mid = 0;
      mid = ((right-left) / 2) + left;
      
      leftArr[0] = "./merge";
      leftArr[1] = argv[1];
      sprintf(bufLeft,"%d", mid);
      leftArr[2] = bufLeft; //Left Split Argv
      leftArr[3] = argv[3];
      leftArr[4] = "0";
      leftArr[5] = '\0';

      rightArr[0] = "./merge";
      sprintf(bufRight, "%d", (mid +1) );
      rightArr[1] = bufRight;
      rightArr[2] = argv[2]; //Right Split Argv
      rightArr[3] = argv[3];
      rightArr[4] = "0";
      rightArr[5] = '\0';

      /*forking for new kids to split further*/
      lefty = fork(); //Left fork
      if( lefty < 0)
	{
	  printf("Lefty broke fork\n");
	}
      else if(lefty ==0)
	{
	  if( execvp("./merge", leftArr ) )
	    {
	      printf("Left: ExecVP didnt run in merge\n");
	      exit(1); //Error and quit if it doesnt execvp
	    }
	}

      righty = fork(); //Right fork
      if( righty < 0 )
	{
	  printf("Righty broke fork\n");
	}
      else if( righty == 0 ) 
	{
	  if ( execvp( "./merge" , rightArr ) )
	    {
	      printf("Right: ExecVP didnt run in merge\n");
	      exit(1); //Error and quit if it doesnt execvp
	    }
	}

      wait(NULL); /* Wait for lefty and righty */
      wait(NULL);

      /*Time to merge-em*/
      i = 0;
      
      key = ftok("./", 'Q' + left); //Temp shared memory key
      sprintf(print, "   *** MERGE: shared memory key = %d \n", key);
      write(1, print, strlen(print));

      ShmIDTEMP = shmget(key, sizeof(int) * (right-left+1), IPC_CREAT | 0666);
      sprintf(print, "   *** MERGE: shared memory created\n");
      write(1, print, strlen(print)); // Temp shared memory ID

      tempArr = (int*) shmat(ShmIDTEMP, NULL, 0);
      sprintf(print, "   *** MERGE: shared memory is attached and is ready to use for the purpose of sorting\n");
      write(1, print, strlen(print)); //Attaching to the temp shared memory

      for(i = left ; i <= right ; i++) //Merging for loop
	{
	  tempPro = fork(); //Breaking all the indecies into children to find their place
	  if(tempPro == 0)
	    { //If you're the child
	      self = getpid();
	      parent = getppid();
	      sprintf(sortBuf, "      $$$ B-PROC(%d): created by M-PROC(%d) for a[%d] = %d is created\n", self, parent, i, array[i]);
	      write(1, sortBuf, strlen(sortBuf)); //Print who you are and what you're doing

	      if( i < mid + 1) //lower half of the current array bounds
		{
		  offset = i - left;
		  binaryvalue = binarySearch(array[i], mid+1 , right, array); //Search upper half
		  bin = binaryvalue - (mid+1) + offset;
		  if(binaryvalue == mid+1)
		    { // Was the largest thing in the array it searched
		      sprintf(sortBuf, "      $$$ B-PROC(%d): a[%d] = %d is larger than a[%d] = %d and is written to temp[%d] \n", self, i, array[i], binaryvalue, array[binaryvalue], bin ); 
		      write(1, sortBuf, strlen(sortBuf));
		    }
		  else if(binaryvalue == left)
		    { //Was the smallest thing in the array it searched
		      sprintf(sortBuf, "      $$$ B-PROC(%d): a[%d] = %d is smaller than a[%d] = %d and is written to temp[%d] \n", self, i, array[i], binaryvalue, array[binaryvalue], bin );
		      write(1, sortBuf, strlen(sortBuf));
		    }
		  else
		    { //Was between
		      sprintf(sortBuf, "      $$$ B-PROC(%d): a[%d] = %d is between a[%d] = %d and a[%d] = %d is written to temp[%d] \n", self, i, array[i], binaryvalue -1, array[binaryvalue-1],
			      binaryvalue, array[binaryvalue], bin );
		      write(1, sortBuf, strlen(sortBuf));
		    }

		  tempArr[bin] = array[i]; //Place it in the temp array
		  
		}
	      else //Upper half of current array
		{
		  offset = i-mid-1;
		  binaryvalue = binarySearch(array[i], left, mid, array); //Search the lower half for larger values
		  bin =  binaryvalue - left + offset;

		  if(binaryvalue == right +1)
		    { // Was the largest thing in the array it searched
		      sprintf(sortBuf, "      $$$ B-PROC(%d): a[%d] = %d is larger than a[%d] = %d and is written to temp[%d] \n", self, i, array[i], binaryvalue, array[binaryvalue], bin );
		      write(1, sortBuf, strlen(sortBuf));
		    }
		  else if(binaryvalue == mid + 1)
		    { //Was the smallest thing in the array it searched
		      sprintf(sortBuf, "      $$$ B-PROC(%d): a[%d] = %d is smaller than a[%d] = %d and is written to temp[%d] \n", self, i, array[i], binaryvalue, array[binaryvalue], bin );
		      write(1, sortBuf, strlen(sortBuf));
		    }
		  else
		    { //Was between
		      sprintf(sortBuf, "      $$$ B-PROC(%d): a[%d] = %d is between a[%d] = %d and a[%d] = %d is written to temp[%d] \n", self, i, array[i], binaryvalue -1, array[binaryvalue-1],
			      binaryvalue, array[binaryvalue], bin );
		      write(1, sortBuf, strlen(sortBuf));
		    }


		  tempArr[bin] = array[i]; //Place it in the temp array
		}
	      
	      sprintf(sortBuf, "      $$$ B-PROC(%d): created by M-PROC(%d) for a[%d] = %d is terminated\n", self, parent, i, array[i]);
	      write(1, sortBuf, strlen(sortBuf));
	      //Print that your child process is terminated
	      return 0;
	      
	    }

	}

      i =0; /*Wait for the processes to complete their thoughts*/
      for(i = left ; i <= right ; i++)
	wait(NULL);
      
      //They've found their positions!
      
      i = 0;
      for(i = left ; i <= right ; i++)
	{
	  array[i] = tempArr[i-left]; //Temp copying into the main array
	}


     
      strcpy(print, "");
      strcpy(buf, "");
      sprintf(print, "   ### M-PROC(%d) created by M-PROC(%d): merge sort a[%d..%d] completed: \n   ",
	      self, parent, left, right);

      
      i = 0;
      for(i = left ; i <= right ; i++)
	{
	  strcpy(buf, "");
	  sprintf(buf, "%4d", array[i]); //Printing the currently sorted list to be passed up
	  strcat(print,buf);
	}

      strcat(print, "\n");
      write(1, print, strlen(print));

      
      shmdt( (void*) tempArr); //Detach the memory
      sprintf(endBuf, "   *** MERGE: sorting shared memory successfully detached\n");
      write(1, endBuf, strlen(endBuf));
      
      shmctl(ShmIDTEMP, IPC_RMID, NULL); //Delete the memory when done
      sprintf(endBuf, "   *** MERGE: sorting shared memory successfully removed\n");
      write(1, endBuf, strlen(endBuf));
      
    }
  else
    {
      /*Size of the given array is 2, what do I do now*/
      temp = 0;
      if(array[left] > array[right]) /*Swap if left is bigger*/
	{
	  temp = array[left];
	  array[left] = array[right]; //Swap so the lists are known to be sorted
	  array[right] = temp;
	}

      sprintf(print, "   ### M-PROC(%d) created by M-PROC(%d): entering with a[%d..%d] -- sorted\n   ",
	      self,parent, left, right );

      for(i =left ; i <= right ; i++)
	{
	  sprintf(buf, "%4d", array[i]); //Announce the sorted characters you have
	  strcat(print,buf);
	}

      strcat(print, "\n");
      write(1, print, strlen(print));


      exit(1);
    }
  


  return 1;

}
