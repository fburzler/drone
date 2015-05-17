#include <stdio.h>

extern int main_drone (int argc, char *argv[]);

int main(int argc, char **argv)
{
	printf("The drones ...\n");
	main_drone(argc,argv);
	return 0;
}
