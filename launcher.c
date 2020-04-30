#include	<stdlib.h>
#include	<stdio.h>
#include	<signal.h>
#include	<string.h>
#include	<unistd.h>

int counter = 0;
int shouldContinue = 1;
int resetTimeInSec;
int signalTimeInSec;

void sigAlarmHandler(int sig){
	counter = 0;
	alarm(resetTimeInSec);
	printf("Resetting\n");


}

void signalTime(int sig){
	counter++;
	printf("got %d\n", counter);
	if(counter >= 3)
	{
		shouldContinue = 0;
		printf("Have reached the limit!\n");
	}
}

int main (int argc, char* argv[]){

	const int MIN_TIME_IN_SECS = 2;

	if(argc < 3){
		printf("Your parameters(s) are less than 3\n");
		exit(EXIT_FAILURE);
	}


	resetTimeInSec = strtol(argv[1], NULL, 10);
	signalTimeInSec = strtol(argv[2], NULL, 10);

	if((resetTimeInSec < MIN_TIME_IN_SECS) || (signalTimeInSec < MIN_TIME_IN_SECS))
	{
		printf("resetTime must be 2 or greater\n");
		exit(EXIT_FAILURE);
	}

	struct sigaction act;

	memset(&act,'\0',sizeof(act));
	act.sa_handler	 = sigAlarmHandler;
	sigaction(SIGALRM,&act,NULL);
	act.sa_handler	= signalTime;
	sigaction(SIGUSR1,&act,NULL);
	 
	const int       BUFFER_LEN              = 64;

	char  buffer[BUFFER_LEN];

	snprintf(buffer,BUFFER_LEN,"%d",signalTimeInSec);
 	
	const int N = 3;
 	pid_t pid[N];
 	int   i;
  	for(i = 0; i < N; i++)
  	{
  		if ((pid[i] = fork()) == 0)
		{
			execl("./signaller","signaller",buffer,NULL);
		
		}
  	}	 

	alarm(resetTimeInSec); 

	while  (shouldContinue)
  	{
    		sleep(1);
  	}
  
	int j;
	for(j = 0; j < 3; j++)
 	{
		kill(pid[j],SIGINT);
		wait(NULL);
 	}
	return(EXIT_SUCCESS);
}
