#include <time.h>
#include "head.h"

int boolean = 0;
int deb, fin;
int total;

void timer (char * str)
{
	if(boolean == 0)
	{
		boolean = 1;
		deb = clock();
	}
	else
	{
		fin = clock();
		total = (fin - deb );
		printf("TIMER : Fonction : %s\n", str);
		printf("%lds%dms\n", total / CLOCKS_PER_SEC, total);
		boolean = 0;
	}
}