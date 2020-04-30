#include <stdlib.h>
#include <stdio.h>

const int	 TEXT_LEN	= 64;

void	 obtainFloat	(float* fPtr)

{
	char text [TEXT_LEN];
do
{
	printf("Please enter a floating point number (0 - 65535): ");
	fgets(text,TEXT_LEN,stdin);
	*fPtr  = atof(text);
}
while ( ((*fPtr) < 0)|| ((*fPtr) > 65535 ));
}

float		squareRoot	(float	number,
				 int	maxIters,
				 int*	 numItersPtr
						    )
{
  float	estimate	= 1.0;

for (*numItersPtr; *numItersPtr <  maxIters; (*numItersPtr)++)
	{
		if(estimate*estimate == number)
	{break;}
	{
		estimate = 0.5 * (estimate + number/estimate);	
	}
     }
  return(estimate);
}


int		main		()
{
  float	f;
  float	ans;
  int	numIters	= 0;

  obtainFloat(&f);
  ans	= squareRoot(f,100,&numIters);
  printf("squareRoot(%g) approx. equals %g (found in %d iterations).\n",
         f,ans,numIters
	);
  return(EXIT_SUCCESS);
}


