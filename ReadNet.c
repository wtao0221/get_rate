#include <stdio.h>
#include <string.h>
#include <unistd.h> // sleep
#include <sys/time.h> // gettimeofday

#define WORD_SIZE 80

int main(int argc, char **argv)
{
	FILE *fin;
	char buf[WORD_SIZE];
	char nic[80];
	int inter;
	int total;
	struct timeval time;
	struct timespec slptime;
	char *pc;
	int tmp, i, found;
	long long int recBytes, transBytes;

	// get parameters
	strcpy(nic, argv[1]);
	sscanf(argv[2], "%d", &inter);
	sscanf(argv[3], "%d", &total);
	slptime.tv_sec = 0;
	slptime.tv_nsec = inter*1000000;

	while (total > 0)
	{
		// open file; record time
		if ( (fin = fopen("/proc/net/dev", "r")) == NULL)
		{
			printf("error: open file failed");
			return -1;
		}
		gettimeofday(&time, NULL);

		// find nic
		found = 0;
		while (fscanf(fin, "%s", buf) != EOF)
		{
			if ( (pc = strchr(buf, ':')) == NULL)
				continue;
			*pc = 0;
			if (strcmp(buf, nic) == 0)
			{
				found = 1;
				break;
			}
		}

		// extract info
		if (found == 1)
		{
			fscanf(fin, "%lld", &recBytes);
			for (i = 0; i < 7; i ++)
				fscanf(fin, "%d", &tmp);
			fscanf(fin, "%lld", &transBytes);
		}
		else
		{
			printf("error: no nic %s", nic);
			return -1;
		}

		fclose(fin);

		printf("%ld.%06ld\t%lld\t%lld\n", time.tv_sec, time.tv_usec, recBytes, transBytes);

		nanosleep(&slptime, NULL);

		--total;
	}

	return 0;
}

