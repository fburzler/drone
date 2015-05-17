#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <list>

#include "gpslog.h"

std::list<POS> pos_list;
std::list<SPEED> speed_list;

double t0;

void read_file(char *filename)
{
	FILE 	*f;
	char	text[128];
	long	t1,t2;
	float	x,y,z;
	int		i,n;
	
	n=0;	// Line counter
	
	f=fopen(filename,"r");	
	
	if(!f)
	{
			printf("Error reading file %s",filename);
			return;
	}
	
	while(1)
	{
		i = fscanf(f,"%s %ld.%ld %f %f %f\n",text,&t1,&t2,&x,&y,&z);
		
		if(i!=6)
			break;
		
		n++;
		
		double t = t1 + t2/1000000.0;
		
		if(n == 1)
			t0 = t;	//Set start time

		if(!strcmp(text,"POS:"))
		{
			POS p;
			p.t = t-t0;
			p.x = x;
			p.y = y;
			p.z = z;
			
			pos_list.push_back(p);
		}

		if(!strcmp(text,"SPD:"))
		{
			SPEED s;
			s.t = t-t0;
			s.x = x;
			s.y = y;
			s.z = z;
			
			speed_list.push_back(s);
		}


	}
	
	fclose(f);
	
	printf("Info: %d lines evaluated ... \n",n);

/*	
	for(std::list<SPEED>::iterator i = speed_list.begin(); 
			i != speed_list.end(); i++)
	{
		std::cout<< "time: " << std::setprecision(12) << i->t <<
				" x: " << i->x << " y: " << i->y << " z: " << i->z << std::endl;
	}
*/
	 
}