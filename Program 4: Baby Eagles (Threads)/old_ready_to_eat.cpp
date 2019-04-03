int ready_to_eat(int tMax, int pots, int who, int *eating, int *eaten, int *feeds, int *waiting, Mutex *Mu, Semaphore *Mom, Semaphore *Food ){
	

	Mu->Lock();  //!!!!!!!!!!!!!!!!!!!!!!!!
	
	char buf[1000];
	char space[100];

	strcpy(space, "");

	for (int i = 0 ; i < who ; i++ )
	{
		strcat(space, " ");
	}
	
	
	sprintf(buf, "%sBaby Eagle %d is ready to eat.\n",space, who);
	write(1, buf, strlen(buf));
 	//I am Ready to eat!


	if( ( (*feeds) == -1  || (*feeds) == -2 ) && (*eaten) == pots) {
		Mu->Unlock();
		//Mom left and there isn't any food left
		return -1;
	}

	if(tMax == (*feeds) || (*eaten) == pots  )
	{ //If you have to wait
		if(tMax == (*feeds) )
		{
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!FIRST TIME ONLY!!!!!!!!!!!!!!!!!!!\n");
			(*waiting)++; // You are the very first child to wake up
			(*feeds)--;
			
			sprintf(buf , "%sBaby eagle %d sees all feeding pots are empty and wakes up the mother.\n",space, who, (*eaten) ); //Print for the ones leaving the 
			write(1, buf, strlen(buf));

			Mom->Signal(); //Tell mom to make food

			
			sprintf(buf, "Mother Eagle is awoke by baby eagle %d and starts preparing food. \n", who );
			write(1, buf, strlen(buf));

			Mu->Unlock();
			Food->Wait(); //Wait for her to tell you its ready. 
		}
		else
		{
			(*waiting)++; //Not enough food for me, wait
			Mu->Unlock();
			Food->Wait();
			
		}

		Mu->Lock();

		if((*waiting) > 0 && (*feeds) == -2 && (*eaten) == pots ) //If you are in the waiting list when mom leaves and no food is left. 
		{
			(*waiting)--;
			//printf("%d Left because was in waiting and there's no food left\n", who);
			Mu->Unlock();
			Food->Signal();
			return -1;
		}
		else
		{
			(*eaten)++; //Increment own counter
			(*eating)++;

			//printf("Before %d leave to eat, what is eating? = %d\n", who, (*eating) );

			sprintf(buf, "%sBaby eagle %d is eating using feeding pot %d\n", space, who, (*eaten) ); //Print for the ones leaving the 
			write(1, buf, strlen(buf));
			//printf("Left from waiting Queue\n");
			Mu->Unlock();
			return 1;

		}

	}
	else
	{
		//printf("\n\nEaten here = %d\n\n", (*eaten) );
		/*
		if((*eaten) >= pots)
		{
			if((*eaten) > pots){
				Mu->Unlock();
				return 1;
			}
			printf("%d I should be waiting, but I didn't\n", who);
			(*waiting)++;
			Mu->Unlock();
			Food->Wait();
			
			
			//Mu->Lock(); //
		}
		
		*/
		

		(*eaten)++;  //Food is ready & Availible
		(*eating)++;

		//printf("Before %d leave to eat, what is eating? = %d\n", who, (*eating) );
		printf("Last else\n");
		sprintf(buf, "%sBaby eagle %d is eating using feeding pot %d\n", space, who, (*eaten) ); //Print for the ones leaving the 
		write(1, buf, strlen(buf));
		//printf("Left because there was an open spot, eaten == %d \n", (*eaten) );
		Mu->Unlock();
		return 1;
	}

	//Go eat
	return 1;
}