
#include <list>

typedef struct
{
	double 	t;
	float 	x,y,z;
} POS;

typedef struct
{
	double 	t;
	float	x,y,z;
} SPEED;

extern std::list<POS> pos_list;
extern std::list<SPEED> speed_list;

void read_file(char *filename);