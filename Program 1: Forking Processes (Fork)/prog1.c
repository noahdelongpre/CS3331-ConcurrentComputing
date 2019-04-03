/*----------------------------------------------------------*/
/* NAME: Noah de Longpre'                 User ID: nkdelong */
/* DUE DATE: 09/29/2017                                     */
/* PROGRAM ASSIGNMENT 1                                     */
/* FILE NAME: prog1.c                                       */
/* PROGRAM PURPOSE:                                         */
/*    This programs implements a bubble sort, Fibonacci     */
/*    number calculator, a Buffon's Needle probability      */
/*    calculator, and the estimated area of an Ellipse      */
/*    using 4 child processes using fork, wait, and exit.   */
/*----------------------------------------------------------*/



#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define M_PI 3.14159265358979323846

int bubbleSort(int m);
int Fibonacci(int n);
int BuffonsNeedle(int r);
int EllipseArea(int s, int a, int b);


/*----------------------------------------------------------*/
/* FUNCTION main :                                          */
/*     Creates the child processes used to calculate the    */
/*     bubbleSort, Fibonacci seq, buffonsNeedle, and the    */
/*     Area of the Ellipse. It also takes in the command    */
/*     line arguements, and sets them to variables.         */
/* PARAMETER USEAGE :                                       */
/*    argc : the count of command line args given.          */
/*    argv : the array of all of the command line args, used*/
/*  to fill the variables for later use.                    */
/* FUNCTION CALLED :                                        */
/*    bubbleSort(int m)                                     */
/*    Fibonacci(int n)                                      */
/*    BuffonsNeedle(int r)                                  */
/*    EllipseArea(int s, int a, int b);                     */
/*----------------------------------------------------------*/

int main(int argc, char *argv[] )
{
  pid_t bub, fibo, buff, elli;
  int m,n,r,s,a,b;
  char buf[100];

  if(argc < 7 || argc > 7)
    {
    return(0);
    }

  sprintf(buf, "Main Proccess Started\n");
  write(1, buf, strlen(buf));


  m = atoi(argv[1]);/*Bubble Sort amount*/
  n = atoi(argv[2]);/*Fibonacci number up to this amount of times*/
  r = atoi(argv[3]);/*Buffons Needle throwing needle this number of times*/

  /*Ellipse Stuffs*/
  s = atoi(argv[4]);/*Randomly picked s points in rectangle in Ellipse?*/
  a = atoi(argv[5]);/*Length of semi-major axis*/
  b = atoi(argv[6]);/*Length of semi-minor axis*/

  sprintf(buf, "Number of Random Numbers   = %d\n", m);
  write(1, buf, strlen(buf));
  sprintf(buf, "Fibonacci Input            = %d\n", n);
  write(1, buf, strlen(buf));
  sprintf(buf, "Buffon's Needle Iterations = %d\n", r);
  write(1, buf, strlen(buf));
  sprintf(buf, "Integration Iterations     = %d\n", s);
  write(1, buf, strlen(buf));
  sprintf(buf, "Semi-Major Axis Length     = %d\n", a);
  write(1, buf, strlen(buf));
  sprintf(buf, "Semi-Minor Axis Length     = %d\n", b);
  write(1, buf, strlen(buf));




  bub = fork();
  if(bub < 0)
    return 1;
  else if(bub == 0)
    {
      bubbleSort(m);
      return 0;
    }
  else
    {
      sprintf(buf, "Bubble Sort Proccess Created\n");
      write(1, buf, strlen(buf));
    }

  fibo = fork(); /* Creating the Process for fibonacci */
  if(fibo < 0)
    return 1; /* If it doesn't create, just don't run. */
  else if(fibo == 0)
    {
      Fibonacci(n); /* Child process starts this */
      return 0;
    }
  else
    {
      sprintf(buf, "Fibonacci Proccess Created\n");
      write(1, buf, strlen(buf)); /* Main prints this */
    }

  
  buff = fork();
  if(buff < 0)
    return 1;
  else if(buff == 0)
    {
      BuffonsNeedle(r);
      return 0;
    }
  else
    {
      sprintf(buf, "Buffon's Needle Process Created\n");
      write(1, buf, strlen(buf));
    }


  elli = fork();
  if(elli < 0)
    return 1;
  else if(elli == 0)
    {
      EllipseArea(s,a,b);
      return 0;
    }
  else
    {
      sprintf(buf, "Ellipse Area Proccess Created\n");
      write(1, buf, strlen(buf));
    }

  sprintf(buf, "Main Process Waits\n");
  write(1, buf, strlen(buf));

  while(wait(NULL) > -1){};

  sprintf(buf, "Main Process Exits\n");
  write(1, buf, strlen(buf));


  return(0);
}

/*----------------------------------------------------------*/
/* FUNCTION bubbleSort :                                    */
/*    Takes in an int m and generates that many random      */
/*    values to sort using a bubble sort. Prints that       */
/*    it started, what the values are, then the sorted list.*/
/*                                                          */
/* PARAMETER USEAGE :                                       */
/*    int m : m is the amount of random numbers it should   */
/*    generate to sort in the bubble sort.                  */
/*                                                          */
/*                                                          */
/*----------------------------------------------------------*/
int bubbleSort(int m)
{
  int *array;
  int swap, rando;
  char buf[100];
  char num[5];
  int i = 0, x = 0, y = 0, index = 0;

  array = malloc(sizeof(int)*(m+1));

  sprintf(buf, "   Bubble Sort Process Started\n");
  write(1, buf, strlen(buf));

  srand(time(NULL));



  for (i=0 ; i < m ; i++)
    {
      rando = (int) rand() % 100; /*Generating the m amount of random numbers to sort*/
      array[i] = rando;
      
    }
  i = 0;



  sprintf(buf, "   Random Numbers Generated:\n");
  write(1, buf, strlen(buf));

  while(index < m) /*Printing syntax*/
    {
      strcpy(buf,"   ");
      for(i = index ; i < m && i < index+20 ; i++)
	{
	  sprintf(num,"%4d", array[i]);
	  strcat(buf, num);
	  
	}
      strcat(buf, "\n");
      write(1, buf, strlen(buf));
      index = index + 20;
    }
  i=0;
  index=0;


  for (x = 0 ; x < (m - 1) ; x++)
    {
      for(y = 0 ; y < m - x - 1 ; y++)
	{
	  if(array[y] > array[y+1])
	    {
	      swap = array[y];
	      array[y] = array[y+1]; /* Swapping and sorting */
	      array[y+1] = swap;
	    }
	}
    }
  x=0; 
  y=0;
  
  sprintf(buf, "   Sorted Sequence:\n");
  write(1, buf, strlen(buf));

  while(index < m) /*Printing syntax*/
    {
      strcpy(buf,"   "); /*Keeping the space buffer*/
      for(i = index ; i < m && i < index+20 ; i++)
	{
	  sprintf(num,"%4d", array[i]);
	  strcat(buf, num); /*Starts to print the sorted list*/
	  
	}
      strcat(buf, "\n");
      write(1, buf, strlen(buf));
      index = index + 20; /*Keeps 20 numbers to a line*/
    }
  i=0;

  sprintf(buf,"   Bubble Sort Process Exits\n");
  write(1, buf, strlen(buf));

  free(array);
  return(0);
}




/*----------------------------------------------------------*/
/* FUNCTION fib :                                           */
/*    The recursive calculation of a Fibonacci numbers      */
/* PARAMETER USEAGE :                                       */
/*    long x: Used for calculation of the fibonacci number  */
/*----------------------------------------------------------*/
long fib(long x)
{
  if(x == 1)
    return 1;/*Bases cases to return actual numbers*/
  if(x == 2)
    return 1;
  if(x == 0)
    return 0;

  /*Otherwise it calls itself again*/
  return fib(x-1) + fib(x-2);
}




/*----------------------------------------------------------*/
/* FUNCTION Fibonacci :                                     */
/*    Prints that the process starts, what number was passed*/
/*    and the fibonacci number at that point in the sequence*/
/*                                                          */
/* PARAMETER USEAGE :                                       */
/*    int n : int passed in to pass to fib() to generate the*/
/*    fibonacci number.                                     */
/* FUNCTION CALLED :                                        */
/*    fib() : Actually generates the fibonacci number.      */
/*----------------------------------------------------------*/
int Fibonacci(int n)
{ /*6 spaces over*/

  char buf[100];

  /*Just writes things out to the buffer*/
  sprintf(buf, "      Fibonacci Process Started\n");
  write(1, buf, strlen(buf));
  sprintf(buf, "      Input Number %d\n", n);
  write(1, buf, strlen(buf));
  sprintf(buf, "      Fibonacci Number f(%d) is %ld\n", n, fib(n));
  write(1, buf, strlen(buf));
  sprintf(buf, "      Fibonacci Process Exits\n");
  write(1, buf, strlen(buf));

  return(0); 
}


/*----------------------------------------------------------*/
/* FUNCTION BuffonsNeedle :                                 */
/*    Using the int r that's passed in, it generates the    */
/*    probabilty of Buffons needle with random throws, r    */
/*    times.                                                */
/* PARAMETER USEAGE :                                       */
/*    int r: The amount of random needle throws.            */
/*----------------------------------------------------------*/
int BuffonsNeedle(int r)
{
 int i, count;
 float d,a,buffon;
 char buf[100];
 double approx;
 srand(time(NULL));
 count = 0;

  sprintf(buf, "         Buffon's Needle Process Started\n");
  write(1, buf, strlen(buf));
  sprintf(buf, "         Input Number %d\n", r);
  write(1, buf, strlen(buf));
 

for( i = 0 ; i < r ; i++)
  {
    d = ((float) rand()) / RAND_MAX;
    a = (((double) rand()) / RAND_MAX) * 2 * M_PI; /*Generates Random nums*/
    buffon = d + sin(a); /*Throws the pin*/
    if(buffon < 0 || buffon > 1) /*If it crosses a line, count it*/
      count++;
  }

 approx =((double) count) / r; /*Finds the approximate probability that it crosses a line.*/ 


 sprintf(buf, "         Estimated Probability is %.5f\n", approx);
 write(1, buf, strlen(buf));
 sprintf(buf, "         Buffon's Needle Process Exits\n");
 write(1, buf, strlen(buf));

return 0;
}



/*----------------------------------------------------------*/
/* FUNCTION EllipseArea :                                   */
/*    Calculates the estimated area of an Ellipse by        */
/*    generating random points that have to be in the rect  */
/*    and the 1/4 of the ellipse.                           */
/* PARAMETER USEAGE :                                       */
/*    int s: Amount of random points that are generated     */
/*    int a: Semi-Major Axis length                         */
/*    int b: Semi-Minor Axis length                         */
/*----------------------------------------------------------*/
int EllipseArea(int s, int a, int b)
{
  float x,y,approx,actual;
  char buf[100];
  int i,count;

  count = 0;

  sprintf(buf, "            Ellipse Area Process Started\n");
  write(1, buf, strlen(buf));
  sprintf(buf, "            Input Number %d\n", s);
  write(1, buf, strlen(buf));
  sprintf(buf, "            Semi-Major Axis Length %d\n", a);
  write(1, buf, strlen(buf));
  sprintf(buf, "            Semi-Minor Axis Length %d\n", b);
 
  srand(time(NULL));

  
  for(i = 0; i < s ; i++)
    {
      x = (((float) rand()) / RAND_MAX) * a; /*random positions for the points*/
      y = (((float) rand()) / RAND_MAX) * b;
      if( ( (x*x) / ( ( (float) a) * a ) ) + (( y * y ) / ( b * b )) <= 1)
	{
	  count++; /*If its in the Ellipse and the rectangle, count it*/
	}
    }

  approx = (((float) count) / s) * a *((float) b) * 4 ;
  actual = (float) M_PI * a * b;

  sprintf(buf, "            Total Hits %d\n", count);
  write(1, buf, strlen(buf));
  sprintf(buf, "            Estimated area is %.5f\n", approx);
  write(1, buf, strlen(buf));
  sprintf(buf, "            Actual Area is %.5f\n", actual);
  write(1, buf, strlen(buf));
  sprintf(buf, "            Ellipse Area Process Exits\n");
  write(1, buf, strlen(buf));

  

  return 0;  
}
