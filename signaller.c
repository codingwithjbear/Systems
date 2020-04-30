#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<signal.h>
#include	<unistd.h>


int maxSignalTime;
int shouldContinue = 1;

void alarmReset(int sig)
{
	alarm(rand() % maxSignalTime + 1);
	kill(getppid(),SIGUSR1);
	printf("signaller %d signalling parent\n", getpid());
}

void shouldC (int sig)
{
	shouldContinue = 0;
	printf("signaller %d stopping\n", getpid());
}

int main (int argc, char* argv[]){


if(argc < 2)
{
	printf("Your parameters(s) are less than 2\n");
	exit(EXIT_FAILURE);
}

maxSignalTime = strtol(argv[1], NULL, 10);

const int MIN_TIME_IN_SECS = 2; 

if(maxSignalTime < MIN_TIME_IN_SECS)
{
	printf("resetTime must be 2 or greater\n");
	exit(EXIT_FAILURE);
}

struct sigaction act;

memset(&act,'\0',sizeof(act));
act.sa_handler	 = alarmReset;
sigaction(SIGALRM,&act,NULL);
act.sa_handler = shouldC;
sigaction(SIGINT,&act,NULL);

srand(getpid());
alarm(rand() % maxSignalTime + 1);


 while  (shouldContinue)
  {
    sleep(1);
  }
  
return(EXIT_SUCCESS);


}
