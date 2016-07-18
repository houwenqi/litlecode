#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>

using namespace std;

int main()
{
	int i;
	char tmp[256];
	char *ptr;
	char msgoption[2048] = "2500222,2501874,200062,2501875,2500220";



	i = sprintf(tmp,"%u",2501874);
	for(ptr=(char *)msgoption;ptr!=NULL && *ptr;)
	{
		ptr = strstr(ptr,tmp);
		printf("ptr is %s\n",ptr );

		//printf("prt[i] is %s\n", ptr[i] );
		if((unsigned)ptr[i] > '9' || (unsigned)ptr[i] < '0' )
		{
			printf("what fuck\n");
		}
		ptr+=i;
		ptr++;
		printf("Last ptr is %s\n",ptr );
	}

	return 0;
}
