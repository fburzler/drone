/*
    Copyright (C) 2014 Parrot SA

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the 
      distribution.
    * Neither the name of Parrot nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
    OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/
/**
 * @file ihm.c
 * @brief This file contains sources about ncurses IHM used by arsdk example "BebopDroneDecodeStream"
 * @date 15/01/2015
 */

/*****************************************
 *
 *             include file :
 *
 *****************************************/

#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/joystick.h>

#define JOY_DEV "/dev/input/js0"

#include <libARSAL/ARSAL.h>

#include "ihm.h"

/*****************************************
 *
 *             define :
 *
 *****************************************/

#define HEADER_X 0
#define HEADER_Y 0

#define INSTRUCTION_X 0
#define INSTRUCTION_Y 2

#define BATTERY_X 0
#define BATTERY_Y 10

#define INFO_X 0
#define INFO_Y 12

#define JOY_X 0
#define JOY_Y 14

/*****************************************
 *
 *             private header:
 *
 ****************************************/
void *IHM_InputProcessing(void *data);

/*****************************************
 *
 *             implementation :
 *
 *****************************************/

static int joy_fd, num_of_axis=0, num_of_buttons=0;
static int *axis=NULL;
static char *button=NULL;
static FILE *flog;

int Init_Joystick(void)
{
	char name_of_joystick[80];

	if( ( joy_fd = open( JOY_DEV , O_RDONLY)) == -1 )
	{
		printf( "Couldn't open joystick\n" );
		return -1;
	}

	ioctl( joy_fd, JSIOCGAXES, &num_of_axis );
	ioctl( joy_fd, JSIOCGBUTTONS, &num_of_buttons );
	ioctl( joy_fd, JSIOCGNAME(80), &name_of_joystick );

	axis = (int *) calloc( num_of_axis, sizeof( int ) );
	button = (char *) calloc( num_of_buttons, sizeof( char ) );

	printf("Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n"
		, name_of_joystick
		, num_of_axis
		, num_of_buttons );

	fcntl( joy_fd, F_SETFL, O_NONBLOCK );	/* use non-blocking mode */
	
	return 0;
}

void Read_Joystick(void)
{
	struct js_event js;
	int ret;

	do
	{
		ret = read(joy_fd, &js, sizeof(struct js_event));
		
		/* see what to do with the event */
		switch (js.type & ~JS_EVENT_INIT)
		{
			case JS_EVENT_AXIS:
				axis   [ js.number ] = js.value;
				break;
			case JS_EVENT_BUTTON:
				button [ js.number ] = js.value;
				break;
		}
	
	}
	while(ret == sizeof(struct js_event));

}


IHM_t *IHM_New (IHM_onInputEvent_t onInputEventCallback)
{
    int failed = 0;
    IHM_t *newIHM = NULL;
    
    // check parameters
    if (onInputEventCallback == NULL)
    {
        failed = 1;
    }
	
	//FB: Joystick init
	Init_Joystick();
	flog = fopen("gps.log","a");
    
    if (!failed)
    {
        //  Initialize IHM
        newIHM = malloc(sizeof(IHM_t));
        if (newIHM != NULL)
        {
            //  Initialize ncurses
            newIHM->mainWindow = initscr();
            newIHM->inputThread = NULL;
            newIHM->run = 1;
            newIHM->onInputEventCallback = onInputEventCallback;
            newIHM->customData = NULL;
        }
        else
        {
            failed = 1;
        }
    }
    
    if (!failed)
    {
        //raw();                  // Line buffering disabled
        cbreak();
        keypad(stdscr, TRUE);
        noecho();               // Don't echo() while we do getch
        timeout(100);
        
        refresh();
    }
    
    if (!failed)
    {
        //start input thread
        if(ARSAL_Thread_Create(&(newIHM->inputThread), IHM_InputProcessing, newIHM) != 0)
        {
            failed = 1;
        }
    }
    
    if (failed)
    {
        IHM_Delete (&newIHM);
    }

    return  newIHM;
}

void IHM_Delete (IHM_t **ihm)
{
    //  Clean up

    if (ihm != NULL)
    {
        if ((*ihm) != NULL)
        {
            (*ihm)->run = 0;
            
            if ((*ihm)->inputThread != NULL)
            {
                ARSAL_Thread_Join((*ihm)->inputThread, NULL);
                ARSAL_Thread_Destroy(&((*ihm)->inputThread));
                (*ihm)->inputThread = NULL;
            }
            
            delwin((*ihm)->mainWindow);
            (*ihm)->mainWindow = NULL;
			
			//FB: End ncurses
			nocbreak();
			echo();
			
            refresh();
			endwin();
			fclose(flog);
			
			printf("endwin() called -> Back on tty ...\n");

            free (*ihm);
            (*ihm) = NULL;
        }
    }
}

void IHM_setCustomData(IHM_t *ihm, void *customData)
{
    if (ihm != NULL)
    {
        ihm->customData = customData;
    }
}

#define JOYSTICK 1000	//set joystick parameter

void *IHM_InputProcessing(void *data)
{
    IHM_t *ihm = (IHM_t *) data;
    int key = 0;
    char text[256];
	
    if (ihm != NULL)
    {
        while (ihm->run)
        {
            key = getch();
			
			Read_Joystick();	// FB: Update the Joystick data !!!
			sprintf(text,"Axis: %d %d %d Buttons: %d %d",axis[0],axis[1],axis[2],button[0],button[1]);
			IHM_PrintJoy(ihm,text);
            
            if (key == 27) // escape character
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_EXIT, ihm->customData);
                }
            }
            else if((key == KEY_UP) || (axis[1] <= -JOYSTICK) )
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_FORWARD, ihm->customData);
                }
            }
            else if((key == KEY_DOWN) || (axis[1] >= JOYSTICK))
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_BACK, ihm->customData);
                }
            }
            else if((key == KEY_LEFT) || (axis[0] <= -JOYSTICK))
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_LEFT, ihm->customData);
                }
            }
            else if((key == KEY_RIGHT) || (axis[0] >= JOYSTICK))
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_RIGHT, ihm->customData);
                }
            }
            else if(key == 'm')
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_EMERGENCY, ihm->customData);
                }
            }
            else if((key == 'z') || (button[3] > 0))
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_UP, ihm->customData);
                }
            }
            else if((key == 's') || (button[2] > 0))
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_DOWN, ihm->customData);
                }
            }
            else if((key == 'q') || (axis[2] <= -JOYSTICK))
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_YAW_LEFT, ihm->customData);
                }
            }
            else if((key == 'd') || (axis[2] >= JOYSTICK))
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_YAW_RIGHT, ihm->customData);
                }
            }
            else if((key == 'i') || (axis[5] >= JOYSTICK))
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_CAM_UP, ihm->customData);
                }
            }
            else if((key == 'k') || (axis[5] <= -JOYSTICK))
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_CAM_DOWN, ihm->customData);
                }
            }
            else if((key == 'j') || (axis[4] <= -JOYSTICK))
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_CAM_LEFT, ihm->customData);
                }
            }
            else if((key == 'l') || (axis[4] >= JOYSTICK))
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_CAM_RIGHT, ihm->customData);
                }
            }
            else if((key == ' ') || (button[1] > 0))
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_TAKEOFF_LANDING, ihm->customData);
                }
            }
            else
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_NONE, ihm->customData);
                }
            }
            
            usleep(10);
        }
    }
    
    return NULL;
}

void IHM_PrintHeader(IHM_t *ihm, char *headerStr)
{
    if (ihm != NULL)
    {
        move(HEADER_Y, 0);   // move to begining of line
        clrtoeol();          // clear line
        mvprintw(HEADER_Y, HEADER_X, headerStr);
    }
}

void IHM_PrintInstruction(IHM_t *ihm, char *instructionStr)
{
    if (ihm != NULL)
    {
        move(INSTRUCTION_Y, 0);   // move to begining of line
        clrtoeol();          // clear line
        mvprintw(INSTRUCTION_Y, INSTRUCTION_X, instructionStr);
    }
}

void IHM_PrintInfo(IHM_t *ihm, char *infoStr)
{
    if (ihm != NULL)
    {
        move(INFO_Y, 0);    // move to begining of line
        clrtoeol();         // clear line
        mvprintw(INFO_Y, INFO_X, infoStr);
    }
}

void IHM_PrintBattery(IHM_t *ihm, uint8_t percent)
{
    if (ihm != NULL)
    {
        move(BATTERY_Y, 0);     // move to begining of line
        clrtoeol();             // clear line
        mvprintw(BATTERY_Y, BATTERY_X, "Battery: %d", percent);
    }
}

//FB:
void IHM_PrintJoy(IHM_t *ihm, char *infoStr)
{
    if (ihm != NULL)
    {
        move(JOY_Y, 0);    // move to begining of line
        clrtoeol();         // clear line
        mvprintw(JOY_Y, JOY_X, infoStr);
    }
}

void IHM_PrintAttitude(IHM_t *ihm, float roll, float pitch, float yaw)
{
	struct timeval t;
	
    if (ihm != NULL)
    {
        move(JOY_Y+1, 0);    // move to begining of line
        clrtoeol();         // clear line
        mvprintw(JOY_Y+1, JOY_X, "Attitude: %f %f %f",roll,pitch,yaw);
		gettimeofday(&t, NULL);
		fprintf(flog,"ATT: %ld.%ld %f %f %f\n",t.tv_sec,t.tv_usec,roll,pitch,yaw);
		fflush(flog);
    }
}

void IHM_PrintPosition(IHM_t *ihm, double lat, double lon, double alt)
{
	struct timeval t;
    
	if (ihm != NULL)
    {
        move(JOY_Y+2, 0);    // move to begining of line.
        clrtoeol();         // clear line
        mvprintw(JOY_Y+2, JOY_X, "Position: %f %f %f",lat,lon,alt);
 		gettimeofday(&t, NULL);
        fprintf(flog, "POS: %ld.%ld %f %f %f\n",t.tv_sec,t.tv_usec,lat,lon,alt);
		fflush(flog);
    }
}

void IHM_PrintSpeed(IHM_t *ihm, float speedx, float speedy, float speedz)
{
	struct timeval t;
	
	if (ihm != NULL)
    {
        move(JOY_Y+3, 0);    // move to begining of line
        clrtoeol();         // clear line
        mvprintw(JOY_Y+3, JOY_X, "Speed: %f %f %f",speedx,speedy,speedz);
		gettimeofday(&t, NULL);
        fprintf(flog, "SPD: %ld.%ld %.10f %.10f %.10f\n",t.tv_sec,t.tv_usec,speedx,speedy,speedz);
		fflush(flog);
    }
}
