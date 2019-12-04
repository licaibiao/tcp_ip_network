#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "NetTrafficStati.h"

int main()
{
	NetTrafficStati l_clsNetTS;
	
	printf("hello world begin \n");
	while(1)
	{
		l_clsNetTS.NTS_AddIpToIPTable();
		l_clsNetTS.NTS_UpdateFlowData();
		sleep(1);
	}

	printf("hello world end \n");
	
	return 0;
}



